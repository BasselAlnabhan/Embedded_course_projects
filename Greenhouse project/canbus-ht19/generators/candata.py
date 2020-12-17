import os
import sys
import json
import copy
import shutil
from datetime import datetime

topics = []
payloads = 0
COM_NODE = 'com'


# Write to file
def write_file(filename, text):
    with open(filename, 'w+') as file:
        try:
            file.write(text.strip())
        except:
            print("Failed to write to {0}".format(filename))
            exit(1)


def get_signal_type(messages, name):
    temp = 'uint8_t'
    for message in messages:
        for signal in message['signals']:
            if name == signal['name']:
                temp = signal['type']
    return temp


def generate_candata_header(node, nodes, messages):
    text = ("/**\n"
            " * @file candata.h\n"
            " * @author Faroch Mehri(faroch.mehri@ya.se)\n"
            " * @brief This module is used to structure and collect all data on the CAN bus.\n"
            " * @version 0.1\n"
            " * @date {0}\n"
            " *\n"
            " * @ copyright Copyright(c) 2020\n"
            " *\n"
            " */\n\n").format(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

    text += '#ifndef CAN_DATA_H\n'
    text += '#define CAN_DATA_H\n\n'
    text += '#include <stdint.h>\n'
    text += '#include <stdbool.h>\n\n'
    text += '#define PAYLOADS_LENGTH ({0}U)\n\n'.format(payloads)

    for n in nodes:
        for key in nodes[n]:
            if key != 'name':
                text += 'typedef struct {\n'
                for param in nodes[n][key]:
                    if param != 'text':
                        if param == 'datetime':
                            text += '\tchar datetime[20];\n'
                        elif isinstance(nodes[n][key][param], dict) == True:
                            text += '\tstruct {\n'
                            for index in nodes[n][key][param]:
                                if isinstance(nodes[n][key][param][index], dict) == True:
                                    text += '\t\tstruct {\n'
                                    for k in nodes[n][key][param][index]:
                                        text += '\t\t\t{0} {1};\n'.format(get_signal_type(
                                            messages, nodes[n][key][param][index]), k)
                                    text += '\t\t} '+'{0};\n'.format(index)
                                else:
                                    text += '\t\t{0} {1};\n'.format(get_signal_type(
                                        messages, nodes[n][key][param][index]), index)
                            text += '\t} '+'{0};\n'.format(param)
                        else:
                            text += '\t{0} {1};\n'.format(
                                get_signal_type(messages, nodes[n][key][param]), param)
                text += '} '+'{0}_t;\n\n'.format(key)

    for n in nodes:
        text += 'typedef struct {\n'
        for name in nodes[n]:
            if name != 'name':
                text += '\t{0}_t {0};\n'.format(name)
        text += '} '+'{0}_t;\n\n'.format(nodes[n]['name'].lower())

    text += 'typedef struct {\n'
    for n in nodes:
        text += '\t{0}_t {0};\n'.format(nodes[n]['name'].lower())
    text += '} data_t;\n\n'

    text += 'data_t get_candata(void);\n\n'

    if node == COM_NODE:
        text += 'void get_payloads(char *payloads);\n\n'

        for n in nodes:
            for name in nodes[n]:
                if name != 'name':
                    text += 'bool is_{0}_changed({1}_t previous, {1}_t current);\n'.format(
                        name[0] + name[1:], name)
                    text += 'void get_{0}_text(char *text, {0}_t {0});\n\n'.format(name)

    text += '#endif /* CAN_DATA_H */'

    return text


def get_status_text(name, messages):
    for message in messages:
        for signal in message['signals']:
            if signal['name'] == name:
                if 'values' in signal:
                    return signal['values']
    return name


def generate_candata_implementation(node, nodes, messages):
    text = '#include <stdio.h>\n'
    text += '#include <string.h>\n'
    text += '#include <canbus.h>\n'
    text += '#include <common.h>\n'
    text += '#include <candata.h>\n\n'

    text += 'data_t get_candata(void)\n{\n'
    text += '\tdata_t data = {};\n\n'

    for n in nodes:
        for key in nodes[n]:
            if key != 'name':
                for param in nodes[n][key]:
                    if param != 'text':
                        if param == 'datetime':
                            text += '\tsprintf(data.{0}.{1}.{2}, "%04d-%02d-%02d %02d:%02d:%02d", '.format(
                                nodes[n]['name'].lower(), key, param)
                            for value in nodes[n][key][param]:
                                text += 'get_{0}(), '.format(value)
                            text = text.strip()[:-1]
                            text += ');\n'
                        elif isinstance(nodes[n][key][param], dict) == True:
                            for index in nodes[n][key][param]:
                                if isinstance(nodes[n][key][param][index], dict) == True:
                                    for m in nodes[n][key][param][index]:
                                        text += '\tdata.{0}.{1}.{2}.{3}.{4}'.format(
                                            nodes[n]['name'].lower(), key, param, index, m)
                                        if m == 'valid':
                                            text += ' = validate_{0}();\n'.format(
                                                nodes[n][key][param][index][m])
                                        else:
                                            text += ' = get_{0}();\n'.format(
                                                nodes[n][key][param][index][m])
                                else:
                                    text += '\tdata.{0}.{1}.{2}.{3}'.format(
                                        nodes[n]['name'].lower(), key, param, index)
                                    if index == 'overwritten':
                                        text += ' = is_{0}_overwritten();\n'.format(
                                            nodes[n][key][param][index])
                                    elif index == 'valid':
                                        text += ' = validate_{0}();\n'.format(
                                            nodes[n][key][param][index])
                                    else:
                                        text += ' = get_{0}();\n'.format(
                                            nodes[n][key][param][index])
                        else:
                            text += '\tdata.{0}.{1}.{2} = get_{3}();\n'.format(
                                nodes[n]['name'].lower(), key, param, nodes[n][key][param])
                text += '\n'

    text += '\treturn data;\n}\n\n'

    if node == COM_NODE:
        for key in nodes:
            for name in nodes[key]:
                if name != 'name':
                    text += 'bool is_{0}_changed({1}_t previous, {1}_t current)\n'.format(
                        name[0] + name[1:], name)

                    count = 0
                    temp = ''
                    for param in nodes[key][name]:
                        if param != 'text':
                            if param not in ['datetime']:
                                if isinstance(nodes[key][name][param], dict) == True:
                                    for index in nodes[key][name][param]:
                                        if isinstance(nodes[key][name][param][index], dict) == True:
                                            for m in nodes[key][name][param][index]:
                                                temp += '\t(current.{0}.{1}.{2} != previous.{0}.{1}.{2}) ||\n\t\t'.format(
                                                    param, index, m)
                                                count += 1
                                        else:
                                            temp += '\t(current.{0}.{1} != previous.{0}.{1}) ||\n\t\t'.format(
                                                param, index)
                                            count += 1
                                else:
                                    temp += '\t(current.{0} != previous.{0}) ||\n\t\t'.format(
                                        param)
                                count += 1
                    temp = temp.strip()
                    temp = temp[:(len(temp) - 3)]
                    if count == 1:
                        temp = temp[1:(len(temp) - 1)]

                    text += '{\n\treturn (' + temp + ');\n}\n\n'

        for key in nodes:
            for name in nodes[key]:
                if name != 'name':
                    for param in nodes[key][name]:
                        if param != 'text':
                            _values = []
                            for val in nodes[key][name]['text']['values']:
                                temp = {}
                                value = val.split('.')
                                if len(value) == 1:
                                    temp[val] = nodes[key][name][value[0]]
                                elif len(value) == 2:
                                    temp[val] = nodes[key][name][value[0]][value[1]]
                                elif len(value) == 3:
                                    temp[val] = nodes[key][name][value[0]
                                                                 ][value[1]][value[2]]
                                _values.append(temp)

                            for i, value in enumerate(_values):
                                for index in value:
                                    if '.valid' in index:
                                        _values[i] = {
                                            index: ["UNINITIALIZED", "OKAY"]}
                                    elif '.overwritten' in index:
                                        _values[i] = {
                                            index: ["NO", "YES"]}
                                    else:
                                        _values[i] = {index: get_status_text(
                                            value[index], messages)}

                            nodes[key][name]['text']['temp'] = _values

        for key in nodes:
            for name in nodes[key]:
                if name != 'name':
                    nodes[key][name]['text']['values'] = nodes[key][name]['text']['temp']
                    del nodes[key][name]['text']['temp']

        for key in nodes:
            for name in nodes[key]:
                if name != 'name':
                    frmt = ''
                    variables = ''
                    text += 'void get_{0}_text(char *text, {0}_t {0})\n'.format(name)
                    for param in nodes[key][name]:
                        if param == 'text':
                            frmt = nodes[key][name][param]['string'] + '\\n'
                            for index, value in enumerate(nodes[key][name][param]['format']):
                                frmt = frmt.replace('{'+str(index)+'}', value)
                            for value in nodes[key][name][param]['values']:
                                for index in value:
                                    if isinstance(value[index], list) == True:
                                        for k in value[index][:-1]:
                                            variables += '\t\t{0}.{1} == {2} ? {2}_MSG :\n'.format(
                                                name, index, k)
                                        variables = '{0} '.format(
                                            variables[:-1])
                                        variables += value[index][-1] + \
                                            '_MSG,\n'
                                    else:
                                        variables += '\t\t{0}.{1},\n'.format(
                                            name, index)
                    text += '{\n\tsprintf(text, "' + \
                        frmt + '",\n' + variables[:-2] + '\n'
                    text += '\t);\n}\n\n'

        text += 'void get_payloads(char *payloads)\n{\n'
        text += '\tdata_t data = get_candata();\n'

        for n in nodes:
            for key in nodes[n]:
                if key != 'name':
                    for param in nodes[n][key]:
                        if param == 'text':
                            for i, value in enumerate(nodes[n][key][param]['values']):
                                k = list(value.keys())[0]
                                v = list(value.values())[0]

                                topic = '{0}.{1}'.format(
                                    nodes[n]['name'].lower(), key)

                                if isinstance(value[k], list) == True:
                                    global payloads
                                    if payloads < len(max(v, key=len)):
                                        payloads += len(max(v, key=len))

                                    temp = k
                                    if 'valid' in k:
                                        temp = k.replace('valid', 'value')

                                    if key == 'rtc':
                                        temp = 'datetime'
                                    topic += '.{0}'.format(temp)

                                    if len(v) == 2:
                                        if 'valid' in k:
                                            temp = '{0}.{1}.{2}'.format(
                                                nodes[n]['name'].lower(), key, k)
                                            text += '\tif(data.{0} == {1})\n'.format(temp,
                                                                                     v[0])
                                            text += '\t{\n'
                                            text += '\t\tsprintf(payloads + strlen(payloads), "%s|", "{0}");\n'.format(
                                                v[0])
                                            text += '\t}\n'
                                            text += '\telse\n'
                                            text += '\t{\n'
                                            text += '\t\tsprintf(payloads + strlen(payloads), "{0}|", data.{1});\n'.format(
                                                nodes[n][key][param]['format'][i + 1], topic)
                                            text += '\t}\n\n'
                                        elif 'overwritten' in k:
                                            text += '\tsprintf(payloads + strlen(payloads), "%s|", (data.{0} == {1}) ? "{1}" : "{2}");\n\n'.format(
                                                topic, v[0], v[1])
                                        elif key == 'rtc':
                                            temp = '{0}.{1}.{2}'.format(
                                                nodes[n]['name'].lower(), key, k)
                                            text += '\tif(data.{0} == {1})\n'.format(temp,
                                                                                     v[0])
                                            text += '\t{\n'
                                            text += '\t\tsprintf(payloads + strlen(payloads), "%s|", "{0}");\n'.format(
                                                v[0])
                                            text += '\t}\n'
                                            text += '\telse\n'
                                            text += '\t{\n'
                                            text += '\t\tsprintf(payloads + strlen(payloads), "{0}|", data.{1});\n'.format(
                                                nodes[n][key][param]['format'][i], topic)
                                            text += '\t}\n\n'
                                        else:
                                            text += '\tsprintf(payloads + strlen(payloads), "%s|", (data.{0} == {1}) ? "{1}" : "{2}");\n\n'.format(
                                                topic, v[0], v[1])
                                    else:
                                        text += '\tswitch(data.{0}.{1}.{2})\n'.format(
                                            nodes[n]['name'].lower(), key, k)
                                        text += '\t{\n'
                                        for s in v:
                                            if s != 'OKAY':
                                                text += '\tcase {}:\n'.format(s)
                                                text += '\t\tsprintf(payloads + strlen(payloads), "%s|", "{0}");\n'.format(
                                                    s)
                                                text += '\t\tbreak;\n'
                                        text += '\tdefault:\n'
                                        if 'valid' in k:
                                            text += '\t\tsprintf(payloads + strlen(payloads), "{0}|", {1});\n'.format(
                                                nodes[n][key][param]['format'][i + 1], 'data.' + topic)
                                        else:
                                            text += '\t\tsprintf(payloads + strlen(payloads), "{0}|", "OKAY");\n'.format(
                                                nodes[n][key][param]['format'][i])
                                        text += '\t\tbreak;\n'
                                        text += '\t}\n\n'
                                    topics.append(topic)

                                elif 'calibration' not in k and 'target' not in k:
                                    topic += '.{0}'.format(k)
                                    text += '\tsprintf(payloads + strlen(payloads), "{0}|", {1});\n\n'.format(
                                        nodes[n][key][param]['format'][i], 'data.' + topic)
                                    topics.append(topic)
        text = text[:-1] + '}\n\n'

    payloads = len(topics) * int(payloads / 2)
    payloads = 64 * int(payloads / 64)

    return text


def generate_esp32_common_h(defines):
    text = ('/**\n'
            ' * @file common.h\n'
            ' * @author Faroch Mehri(faroch.mehri@ya.se)\n'
            ' * @brief Generated macros and topics.\n'
            ' * @version 0.1\n'
            ' * @date {0}\n'
            ' *\n'
            ' * @copyright Copyright(c) 2020\n'
            ' *\n'
            ' */\n\n').format(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    text += '#ifndef COMMON_H\n'
    text += '#define COMMON_H\n\n'

    text += '#define TIME_OFFSET (1 * 60 * 60)\n'
    text += '#define TIME_DAYLIGHT (1 * 60 * 60)\n'
    text += '#define NTP_ADDRESS "0.se.pool.ntp.org"\n\n'

    text += '#define PORT (15154U)\n'
    text += '#define CLIENT_ID "com-node"\n'
    text += '#define USERNAME "rdgxxhuu"\n'
    text += '#define PASSWORD "HtRsIozmfKrQ"\n'
    text += '#define SERVER "m24.cloudmqtt.com"\n\n'

    for key, value in enumerate(defines['esp32']):
        text += '#define {} ({}U)\n'.format(value, key)

    text += '\n#define PAYLOADS_LENGTH ({}U)\n'.format(payloads)
    text += '#define TOPICS_NUMBER ({}U)\n'.format(len(topics))
    text += '#define TOPIC_LENGTH ({}U)\n\n'.format(
        len(max(topics, key=len)) + 2)

    text += 'static char topics[TOPICS_NUMBER][TOPIC_LENGTH] = {\n'
    for topic in topics:
        text += '\t"/{0}",\n'.format(topic.replace('.', '/'))
    text = text.strip()[:-1]
    text += '\n};\n\n'

    text += '\n#endif /* COMMON_H */'
    return text


def generate_canbus_data(node, path, nodes, defines, messages):
    nodes = copy.deepcopy(nodes)
    messages = copy.deepcopy(messages)

    for message in messages:
        for signal in message['signals']:
            if 'values' in signal:
                signal['values'] = defines[signal['values']]

    text = generate_candata_implementation(node, nodes, messages)
    write_file('{0}/candata.cpp'.format(path), text)

    text = generate_candata_header(node, nodes, messages)
    write_file('{0}/candata.h'.format(path), text)

    if node == COM_NODE:
        text = generate_esp32_common_h(defines)
        write_file('../esp32/include/common.h', text)
