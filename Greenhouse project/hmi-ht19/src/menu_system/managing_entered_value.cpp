#include <managing_entered_value.h>
#include <pitches.h>

typedef enum
{
    MIN_VALUE_PLACE,
    MAX_VALUE_PLACE,
    DONE
} value_input_stages_t;

static char buffer[10];
static uint8_t buffer_index = 0;
static displays_t output_display;
static char last_pressed_key;
static uint8_t enter_counter;
static uint8_t writing_lenght;
static char value;
static position_t writing_pos;
static components_t component;

static void draw_writing_signal(position_t pos)
{
    static bool active = false;
    if (active)
    {
        display_draw_rect({pos, 1, 15, BLACK});
        active = false;
    }
    else
    {
        display_draw_rect({pos, 1, 15, GRAY});
        active = true;
    }
}

static void draw_right_sign()
{
    display_draw_rect({{70, 100}, 90, 90, BLACK});
    display_draw_rect({{71, 101}, 88, 88, WHITE});
    display_draw_text({{85, 110}, "DONE", BLACK, WHITE, 14});

    for (uint8_t x = 0; x < 70; x++)
    {
        display_draw_triangle({{(int16_t)(85 + (x / 4)), (int16_t)(150 + (x / 2))}, {85, 150}, {(int16_t)(85 + (x / 4)), (int16_t)(150 + (x / 4))}, GREEN});
    }

    buzzer_driver_tone(NOTE_DS6, 400);


    for (uint8_t x = 0; x < 160; x++)
    {
        display_draw_triangle({{(int16_t)(102 + (x / 3)), (int16_t)(167 - (x / 3))}, {102, 167}, {102, 184}, GREEN});
    }
}

static void draw_wrong_sign()
{
    display_draw_rect({{70, 100}, 90, 90, BLACK});
    display_draw_rect({{71, 101}, 88, 88, WHITE});
    display_draw_text({{83, 110}, "WRONG", BLACK, WHITE, 13});

    for (uint8_t x = 0; x < 120; x++)
    {
        display_draw_triangle({{(int16_t)(85 + ((x) / 2)), (int16_t)(150 + (x / 3))}, {85, 150}, {(int16_t)(85 + (x / 2)), (int16_t)(150 + ((x / 3) - 10))}, RED});
    }
    buzzer_driver_tone(NOTE_AS1, 400);

    for (uint8_t x = 0; x < 120; x++)
    {
        display_draw_triangle({{(int16_t)(144 - ((x) / 2)), (int16_t)(150 + (x / 3))}, {144, 150}, {(int16_t)(144 - (x / 2)), (int16_t)(150 + ((x / 3) - 10))}, RED});
    }
}

static void write_and_move_one_step_forward()
{
    char temp_buffer[2] = {value, '\0'};

    if (writing_lenght < MAX_NO_LENGHT)
    {
        display_draw_text({writing_pos, temp_buffer, BLACK, GRAY, 14});
        writing_pos.x += ONE_STEP_MOVE;
        buffer[buffer_index] = value;
        buffer_index += 1;
        writing_lenght += 1;
    }
}

static void delete_and_move_one_step_backward()
{
    if (writing_lenght > 0 && enter_counter == MAX_VALUE_PLACE)
    {
        writing_pos.x -= ONE_STEP_MOVE;
        display_draw_rect({writing_pos, ONE_STEP_MOVE + 2, 15, GRAY});
        writing_lenght -= 1;
        buffer[buffer_index - 1] = '\0';
        buffer_index--;
    }
    else if (writing_lenght > 0 && enter_counter == MIN_VALUE_PLACE)
    {
        writing_pos.x -= ONE_STEP_MOVE;
        display_draw_rect({writing_pos, ONE_STEP_MOVE + 2, 15, GRAY});
        writing_lenght -= 1;
        buffer[buffer_index] = '\0';
        buffer_index--;
    }
    if (enter_counter == MAX_VALUE_PLACE && writing_lenght == 0)
    {
        display_draw_rect({writing_pos, 1, 15, GRAY});
        writing_pos = DEFAULT_MIN_FIELD_POS;
        writing_lenght = buffer_index - 1;
        writing_pos.x += writing_lenght * ONE_STEP_MOVE;
        enter_counter -= 1;
        buffer[buffer_index] = '\0';
        buffer_index--;
    }
}

static void reset_input_window_values(bool *ca_tar_text_flag, bool *overwrite_text_flag)
{
    enter_counter = MIN_VALUE_PLACE;
    writing_lenght = 0;
    last_pressed_key = '\0';
    switch (output_display)
    {
    case CAL_SET_WINDOW:
        output_display = SET_CAL_MENU;
        break;
    case TAR_SET_WINDOW:
        output_display = SET_TAR_MENU;
        break;
    case OVERWRITE_ACT_WINDOW_VALUE_PART:
        output_display = OVERWRITE_ACT_MENU;
        break;
    default:
        break;
    }
    *ca_tar_text_flag = true;
    *overwrite_text_flag = true;
    memset(buffer, '\0', sizeof(buffer));
    buffer_index = 0;
}

static void evaluating_input()
{
    switch (value)
    {
    case ENTER_KEY:
        if (writing_lenght != 0 && enter_counter == MIN_VALUE_PLACE)
        {
            display_draw_rect({writing_pos, 1, 15, GRAY});
            writing_pos = DEFAULT_MAX_FIELD_POS;
            buffer[buffer_index] = ',';
            writing_lenght = 0;
            buffer_index++;
            Serial.printf("\ninput is %d \n", component);
            if (component != LIGHT_INTENSITY && component != WATERING_AMOUNT && component != LAMP && component != HEATER)
            {
                enter_counter = MAX_VALUE_PLACE;
            }
            else
            {
                enter_counter = DONE;
            }
        }
        else if (writing_lenght != 0 && enter_counter == MAX_VALUE_PLACE)
        {
            enter_counter = DONE;
        }
        break;
    case DELETE_CANCEL_KEY:
        delete_and_move_one_step_backward();
        break;
    default:
        write_and_move_one_step_forward();
        break;
    }
}

displays_t cal_tar_values_input_window(components_t sensor, displays_t cal_or_tar_or_overwrite, position_t start_pos)
{
    static bool sen_cal_write_window_flag = true;
    static bool act_overwrite_value_part_flag = true;
    component = sensor;
    output_display = cal_or_tar_or_overwrite;
    if (sen_cal_write_window_flag && cal_or_tar_or_overwrite != OVERWRITE_ACT_WINDOW_VALUE_PART)
    {
        writing_pos = start_pos;
        write_cal_tar_window_text(sensor, cal_or_tar_or_overwrite, &sen_cal_write_window_flag);
    }
    sen_cal_write_window_flag = false;

    if (act_overwrite_value_part_flag && cal_or_tar_or_overwrite == OVERWRITE_ACT_WINDOW_VALUE_PART)
    {
        writing_pos = start_pos;
        act_overwrite_value_part_text(sensor);
    }
    act_overwrite_value_part_flag = false;

    draw_writing_signal(writing_pos);
    value = keypad_driver_get_key();
    if (value != '\0' || enter_counter == DONE)
    {
        last_pressed_key = value;
        switch (enter_counter)
        {
        case MIN_VALUE_PLACE:
        case MAX_VALUE_PLACE:
            evaluating_input();
            break;
        case DONE:
            if (validate_input_values(&component, &output_display, buffer))
            {
                if (component == HEATER || component == LAMP)
                {
                    draw_right_sign();
                    reset_input_window_values(&sen_cal_write_window_flag, &act_overwrite_value_part_flag);
                }
                else
                {
                    draw_right_sign();
                    update_eeprom(&component, &output_display);
                    reset_input_window_values(&sen_cal_write_window_flag, &act_overwrite_value_part_flag);
                }
            }
            else
            {
                draw_wrong_sign();
                reset_input_window_values(&sen_cal_write_window_flag, &act_overwrite_value_part_flag);
            }
            break;
        }
    }
    if (keypad_driver_get_state() == KEY_HOLD && last_pressed_key == DELETE_CANCEL_KEY)
    {
        reset_input_window_values(&sen_cal_write_window_flag, &act_overwrite_value_part_flag);
    }
    return output_display;
}

void set_act_to_auto(components_t component)
{
    switch (component)
    {
    case LAMP:
        overwrite_lamp_shininess(NO);
        
        break;
    case HEATER:
        overwrite_heater_warmness(NO);
        break;
    case PUMP:
        overwrite_water_pump_state(NO);
        break;
    case VALAE:
        overwrite_water_valve_state(NO);
        break;
    case WINDOW:
        overwrite_window_state(NO);
        break;
    case FANS:
        overwrite_fans_state(NO);
        break;
    default:
        break;
    }
    draw_right_sign();
}

void set_act_to_on_open(components_t component)
{
    switch (component)
    {
    case PUMP:
        overwrite_water_pump_state(YES);
        set_water_pump_state(ON);
        break;
    case VALAE:
        overwrite_water_valve_state(YES);
        set_water_valve_state(ON);
        break;
    case WINDOW:
        overwrite_window_state(YES);
        set_window_state(OPEN);
        break;
    case FANS:
        overwrite_fans_state(YES);
        set_fans_state(ON);
        break;
    default:
        break;
    }
    draw_right_sign();
}

void set_act_to_off_close(components_t component)
{
    switch (component)
    {
    case PUMP:
        overwrite_water_pump_state(YES);
        set_water_pump_state(OFF);
        break;
    case VALAE:
        overwrite_water_valve_state(YES);
        set_water_valve_state(OFF);
        break;
    case WINDOW:
        overwrite_window_state(YES);
        set_window_state(CLOSE);
        break;
    case FANS:
        overwrite_fans_state(YES);
        set_fans_state(OFF);
        break;
    default:
        break;
    }
    draw_right_sign();
}
