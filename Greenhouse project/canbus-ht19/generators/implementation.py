import copy
from datetime import datetime


def create_canbus_implementation(path, text):
    with open(path+"/canbus.cpp", 'w+') as f:
        try:
            f.write(text)
        except:
            print("Failed to write to {}/canbus.cpp".format(path))
        finally:
            f.close()


def generate_implementaion_head(float):
    text = ("/**\n"
            " * @file canbus.cpp\n"
            " * @author Jesper Mellquist & Bengt Cruz\n"
            " * @brief Generated implementation for canbus module.\n"
            " *        DO NOT EDIT THIS FILE BY HAND.\n"
            " * @version 0.1\n"
            " * @date {}\n"
            " *\n"
            " * @copyright Copyright (c) 2020\n"
            " *\n"
            " */\n\n"
            "#include <canbus.h>\n"
            "#include <common.h>\n"
            "#include <can_signal.h>\n\n").format(datetime.now().strftime("%Y-%m-%d"))
    if float == True:
        text += '#define PRECISION (0.1f)\n\n'

    return text


def generate_overwrite_implementation(node, mode, defines, messages):
    text = ""
    for index, message in enumerate(messages):
        for signal in message["signals"]:
            if "overwrite" in signal:
                if node == message['setter']:
                    temp = "\tcan_signal_write({0}, {1}, 1, (value == {2}) ? 1 : 0);\n".format(
                        index, signal['start'] + signal['length'] - 1, defines['noyes'][1])
                    text += ("void overwrite_{0}(uint8_t value)\n"
                             "{{\n{1}}}\n\n").format(signal["name"], temp)
                elif mode == 'test' and node in signal['getters']:
                    temp = "\tcan_signal_write({0}, {1}, 1, (value == {2}) ? 1 : 0);\n".format(
                        index, signal['start'] + signal['length'] - 1, defines['noyes'][1])
                    text += ("void overwrite_test_{0}(uint8_t value)\n"
                             "{{\n{1}}}\n\n").format(signal["name"], temp)
    return text


def generate_is_overwritten_implementation(node, defines, messages):
    text = ""
    for index, message in enumerate(messages):
        for signal in message["signals"]:
            if "overwrite" in signal and (node == signal['overwrite'] or node in signal['getters'] or node == message['setter']):
                temp = '\treturn (1 == can_signal_read({0}, {1}, 1)) ? {2} : {3};\n'.format(
                    index, signal['start'] + signal['length'] - 1, defines['noyes'][1], defines['noyes'][0])
                text += ("uint8_t is_{0}_overwritten(void)\n"
                         "{{\n{1}}}\n\n").format(signal["name"], temp)
    return text


def generate_set_implementation(node, mode, messages):
    text = ""
    for index, message in enumerate(messages):
        if node == message['setter']:
            for signal in message["signals"]:
                var = 'value'
                length = signal['length']
                if 'validate' in signal or 'overwrite' in signal:
                    length -= 1
                text += "bool set_{0}({1} value)\n".format(
                    signal['name'], signal['type']) + '{\n'

                text += '\tbool status = false;\n'
                if 'range' in signal:
                    text += '\tif ((value >= {}) && (value <= {}))\n'.format(
                        signal['range'][0], signal['range'][1])
                elif 'values' in signal:
                    values = ''
                    for value in signal['values']:
                        values += '(value == {}) || '.format(value)
                    text += '\tif ({0})\n'.format(values[:-4])

                text += '\t{\n\t\tstatus = true;\n'
                if signal["type"] == "float" or signal["type"][:3] == 'int':
                    var = 'temp'
                    if signal["type"] == "float":
                        text += '\t\tvalue /= PRECISION;\n'
                    if float(signal['range'][0]) < 0 or float(signal['range'][1]) < 0:
                        text += '\t\tuint64_t temp = (value < 0) ? twos_complement((uint64_t)(-value), {0}) : (uint64_t)value;\n'.format(
                            length)
                    else:
                        var = '(uint64_t)value'

                if 'validate' in signal:
                    text += '\t\tcan_signal_write({0}, {1}, {2}, {3} | {4});\n'.format(
                        index, signal['start'], length + 1, var, hex(1 << length))
                else:
                    text += '\t\tcan_signal_write({0}, {1}, {2}, {3});\n'.format(
                        index, signal['start'], length, var)

                text += '\t}\n\treturn status;\n}\n\n'

                if mode == 'test' and node not in signal['getters']:
                    length = signal['length']
                    if 'validate' in signal or 'overwrite' in signal:
                        length -= 1

                    text += '{0} get_test_{1}(void)\n'.format(
                        signal['type'], signal['name'])
                    text += '{\n'
                    if signal["type"] == "float" or signal["type"][:3] == 'int':
                        text += '\t{0} value = 0;\n'.format(signal['type'])
                        text += '\tuint64_t temp = can_signal_read({0}, {1}, {2});\n'.format(
                            index, signal['start'], length)
                        if float(signal['range'][0]) < 0 or float(signal['range'][1]) < 0:
                            text += '\n\tif(temp & ((uint64_t)1 << {0}))\n'.format(
                                length - 1)
                            text += '\t{\n'
                            text += '\t\tvalue = -({0})twos_complement(temp, {1});\n'.format(
                                signal["type"], length)
                            text += '\t}\n'
                            text += '\telse\n\t{\n'
                            text += '\t\tvalue = ({0})temp;\t}}\n'.format(
                                signal['type'])
                        else:
                            text += '\tvalue = ({0})temp;\n'.format(
                                signal['type'])

                        if signal["type"] == "float":
                            text += '\n\treturn (value * PRECISION);\n'
                        else:
                            text += '\n\treturn value;\n'
                    else:
                        text += '\treturn ({0})can_signal_read({1}, {2}, {3});\n'.format(
                            signal["type"], index, signal['start'], length)
                    text += '}\n\n'

    return text


def generate_get_implementation(node, mode, messages):
    text = ""
    for index, message in enumerate(messages):
        for signal in message["signals"]:
            if node in signal["getters"]:
                length = signal['length']
                if 'validate' in signal or 'overwrite' in signal:
                    length -= 1

                text += '{0} get_{1}(void)\n'.format(
                    signal['type'], signal['name'])
                text += '{\n'
                if signal["type"] == "float" or signal["type"][:3] == 'int':
                    text += '\t{0} value = 0;\n'.format(signal['type'])
                    text += '\tuint64_t temp = can_signal_read({0}, {1}, {2});\n'.format(
                        index, signal['start'], length)
                    if float(signal['range'][0]) < 0 or float(signal['range'][1]) < 0:
                        text += '\n\tif(temp & ((uint64_t)1 << {0}))\n'.format(
                            length - 1)
                        text += '\t{\n'
                        text += '\t\tvalue = -({0})twos_complement(temp, {1});\n'.format(
                            signal["type"], length)
                        text += '\t}\n'
                        text += '\telse\n\t{\n'
                        text += '\t\tvalue = ({0})temp;\n\t}}\n'.format(
                            signal['type'])
                    else:
                        text += '\tvalue = ({0})temp;\n'.format(signal['type'])

                    if signal["type"] == "float":
                        text += '\n\treturn (value * PRECISION);\n'
                    else:
                        text += '\n\treturn value;\n'
                else:
                    text += '\treturn ({0})can_signal_read({1}, {2}, {3});\n'.format(
                        signal["type"], index, signal['start'], length)
                text += '}\n\n'

                if mode == 'test' and node != message['setter']:
                    var = 'value'
                    length = signal['length']
                    if 'validate' in signal or 'overwrite' in signal:
                        length -= 1
                    text += "bool set_test_{0}({1} value)\n".format(
                        signal['name'], signal['type']) + '{\n'

                    text += '\tbool status = false;\n'
                    if 'range' in signal:
                        text += '\tif ((value >= {}) && (value <= {}))\n'.format(
                            signal['range'][0], signal['range'][1])
                    elif 'values' in signal:
                        values = ''
                        for value in signal['values']:
                            values += '(value == {}) || '.format(value)
                        text += '\tif ({0})\n'.format(values[:-4])

                    text += '\t{\n\t\tstatus = true;\n'
                    if signal["type"] == "float" or signal["type"][:3] == 'int':
                        var = 'temp'
                        if signal["type"] == "float":
                            text += '\t\tvalue /= PRECISION;\n'
                        if float(signal['range'][0]) < 0 or float(signal['range'][1]) < 0:
                            text += '\t\tuint64_t temp = (value < 0) ? twos_complement((uint64_t)(-value), {0}) : (uint64_t)value;\n'.format(
                                length)
                        else:
                            var = '(uint64_t)value'

                    if 'validate' in signal:
                        text += '\t\tcan_signal_write({0}, {1}, {2}, {3} | {4});\n'.format(
                            index, signal['start'], length + 1, var, hex(1 << length))
                    else:
                        text += '\t\tcan_signal_write({0}, {1}, {2}, {3});\n'.format(
                            index, signal['start'], length, var)

                    text += '\t}\n\treturn status;\n}\n\n'

    return text


def generate_validate_implementation(node, defines, messages):
    text = ""
    for index, message in enumerate(messages):
        for signal in message["signals"]:
            if "validate" in signal and node in signal["getters"]:
                temp = '\treturn (1 == can_signal_read({0}, {1}, 1)) ? {2} : {3};\n'.format(
                    index, signal['start'] + signal['length'] - 1, defines['valid'][1], defines['valid'][0])
                text += ("uint8_t validate_{0}(void)\n"
                         "{{\n{1}}}\n\n").format(signal["name"], temp)
    return text


def check_types(node, messages):
    temp = {"float": False, "negative": False}
    for message in messages:
        for signal in message["signals"]:
            if node == message["setter"] or node in signal["getters"]:
                if signal['type'] == 'float':
                    temp['float'] = True

                if (signal['type'] == 'float' or signal['type'][:3] == 'int') and 'range' in signal:
                    if float(signal['range'][0]) < 0 or float(signal['range'][1]) < 0:
                        temp['negative'] = True

                if temp['float'] == True and temp['negative'] == True:
                    return temp
    return temp


def generate_canbus_implementation(node, mode, path, defines, messages):
    messages = copy.deepcopy(messages)
    for message in messages:
        for signal in message['signals']:
            if 'values' in signal:
                signal['values'] = defines[signal['values']]

    temp = check_types(node, messages)
    text = generate_implementaion_head(temp['float'])
    if temp['negative'] == True:
        text += 'static inline uint64_t twos_complement(uint64_t value, uint8_t length) \n{\n'
        text += '\tuint64_t mask = ((uint64_t)1 << length) - (uint64_t)1;\n'
        text += '\treturn (~(value & mask) + (uint64_t)1) & mask;\n'
        text += '}\n\n'
    text += generate_get_implementation(node, mode, messages)
    text += generate_set_implementation(node, mode, messages)
    text += generate_validate_implementation(node, defines, messages)
    text += generate_overwrite_implementation(node, mode, defines, messages)
    text += generate_is_overwritten_implementation(node, defines, messages)
    create_canbus_implementation(path, text)