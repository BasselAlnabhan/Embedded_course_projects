#ifndef MANAGING_ENTERED_VALUE_H
#define MANAGING_ENTERED_VALUE_H

#include <text_menus.h>
#include <eeprom_control.h>

#define DEFAULT_MIN_FIELD_POS {24, 116}
#define DEFAULT_MAX_FIELD_POS {24, 166}
#define DEFAULT_OVERWRIE_FIELD_POS \
    {                         \
        24, 139               \
    }

#define MAX_NO_LENGHT 4
#define ONE_STEP_MOVE 12
#define ENTER_KEY '#'
#define DELETE_CANCEL_KEY '*'

//#include <canbus.h>

displays_t cal_tar_values_input_window(components_t sensor, displays_t cal_or_tar, position_t start_pos);
void set_act_to_auto(components_t component);
void set_act_to_on_open(components_t component);
void set_act_to_off_close(components_t component);

#endif /* MANAGING_ENTERED_VALUE_H */
