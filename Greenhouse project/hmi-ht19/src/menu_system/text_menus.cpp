
#include <text_menus.h>

#define DISPLAY_PART_WIDTH 240U
#define DISPLAY_PART_LENGHT 270U
#define HAED_SUBJECT_POS \
    {                    \
        10, 9            \
    }
#define MESSAGE_POS \
    {               \
        17, 70      \
    }
#define FIRST_LINE_POS \
    {                  \
        10, 40         \
    }
#define SECOND_LINE_POS \
    {                   \
        10, 70          \
    }
#define THIRD_LINE_POS \
    {                  \
        10, 100        \
    }
#define FORTH_LINE_POS \
    {                  \
        10, 130        \
    }
#define FIFTH_LINE_POS \
    {                  \
        10, 160        \
    }
#define SIXTH_LINE_POS \
    {                  \
        10, 190        \
    }
#define SEVENTH_LINE_POS \
    {                    \
        10, 220          \
    }

#define FLASH_MENU_PART                                        \
    {                                                          \
        {0, 0}, DISPLAY_PART_WIDTH, DISPLAY_PART_LENGHT, WHITE \
    }

#define AUTO 3U

static char cal_menues_names[6][17] = {"Temperature",
                                       "Humidity",
                                       "Light Intensity",
                                       "Watering Amount",
                                       "Soil Moisture",
                                       "Tank Water Level"};

static char act_overwrite_menu_names[6][16] = {"Lamp Shininess",
                                               "Heater Warmness",
                                               "Water Pump",
                                               "Water Valve",
                                               "Window",
                                               "Fans"};

static bool auto_flag = true;
static bool manual_flag = true;

void main_menu_text(void)
{
    display_draw_rect(FLASH_MENU_PART);

    display_draw_text({HAED_SUBJECT_POS, "MAIN MENU", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});

    display_draw_text({FIRST_LINE_POS, "1. System Overview", BLACK, WHITE, 14});
    display_draw_text({SECOND_LINE_POS, "2. Components Status", BLACK, WHITE, 14});
    display_draw_text({THIRD_LINE_POS, "3. Set Calibration Values", BLACK, WHITE, 14});
    display_draw_text({FORTH_LINE_POS, "4. Set Target Values", BLACK, WHITE, 14});
    display_draw_text({FIFTH_LINE_POS, "5. Control Actuators", BLACK, WHITE, 14});
}

void set_cal_menu_text(void)
{
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "CALIBRATION MENU", RED, WHITE, 14});
    display_draw_line({{0, 30}, 240, BLACK});
    display_draw_line({{0, 32}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "1. Temperature Sensor", BLACK, WHITE, 14});
    display_draw_text({SECOND_LINE_POS, "2. Humidity Sensor", BLACK, WHITE, 14});
    display_draw_text({THIRD_LINE_POS, "0. Back", BLACK, WHITE, 14});
}

void set_tar_menu_text(void)
{
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "TARGET MENU", RED, WHITE, 14});
    display_draw_line({{0, 30}, 240, BLACK});
    display_draw_line({{0, 32}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "1. Temperature", BLACK, WHITE, 14});
    display_draw_text({SECOND_LINE_POS, "2. Humidity", BLACK, WHITE, 14});
    display_draw_text({THIRD_LINE_POS, "3. Light Intensity", BLACK, WHITE, 14});
    display_draw_text({FORTH_LINE_POS, "4. Watering Amount", BLACK, WHITE, 14});
    display_draw_text({FIFTH_LINE_POS, "5. Soil Moisture", BLACK, WHITE, 14});
    display_draw_text({SIXTH_LINE_POS, "6. Tank Water Level", BLACK, WHITE, 14});

    display_draw_text({SEVENTH_LINE_POS, "0. Back", BLACK, WHITE, 14});
}

void act_overwrite_menu_text(void)
{
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "CONTROL ACTUATORS", RED, WHITE, 13});
    display_draw_line({{0, 30}, 240, BLACK});
    display_draw_line({{0, 32}, 240, BLACK});

    display_draw_text({FIRST_LINE_POS, "1. Lamp Shininess", BLACK, WHITE, 14});
    display_draw_text({SECOND_LINE_POS, "2. Heater Warmness", BLACK, WHITE, 14});
    display_draw_text({THIRD_LINE_POS, "3. Water Pump", BLACK, WHITE, 14});
    display_draw_text({FORTH_LINE_POS, "4. Water Valve", BLACK, WHITE, 14});
    display_draw_text({FIFTH_LINE_POS, "5. Window", BLACK, WHITE, 14});
    display_draw_text({SIXTH_LINE_POS, "6. Fans", BLACK, WHITE, 14});
    display_draw_text({SEVENTH_LINE_POS, "0. Back", BLACK, WHITE, 14});
}

void write_cal_tar_window_text(components_t sensor, uint8_t cal_or_tar_or_overwrite, bool *text_flage)
{
    display_draw_rect({{0, 0}, DISPLAY_PART_WIDTH, 25, WHITE});
    display_draw_text({HAED_SUBJECT_POS, cal_menues_names[sensor], RED, WHITE, 14});
    display_draw_rect({{10, 33}, 200, 220, BLACK});
    display_draw_rect({{15, 38}, 190, 210, WHITE});
    uint16_t max = 0;
    uint16_t min = 0;
    char window_title[16];
    if (cal_or_tar_or_overwrite == CAL_SET_WINDOW)
    {
        sprintf(window_title, "Set Calibration");

        if (sensor == TEMPERATURE)
        {
            max = TEMPERATURE_CALIBRATION_MAX_UPPER;
            min = TEMPERATURE_CALIBRATION_MIN_LOWER;
        }
        if (sensor == HUMIDITY)
        {
            max = HUMIDITY_CALIBRATION_MAX_UPPER;
            min = HUMIDITY_CALIBRATION_MIN_LOWER;
        }
    }
    if (cal_or_tar_or_overwrite == TAR_SET_WINDOW)
    {
        sprintf(window_title, "Set Target");

        if (sensor == TEMPERATURE)
        {
            max = TEMPERATURE_TARGET_MAX_UPPER;
            min = TEMPERATURE_TARGET_MIN_LOWER;
        }
        if (sensor == HUMIDITY)
        {
            max = HUMIDITY_TARGET_MAX_UPPER;
            min = HUMIDITY_TARGET_MIN_LOWER;
        }
        if (sensor == LIGHT_INTENSITY)
        {
            max = LIGHT_INTENSITY_TARGET_MIN_UPPER;
            min = LIGHT_INTENSITY_TARGET_MIN_LOWER;
        }
        if (sensor == SOIL_MOISTURE)
        {
            max = SOIL_MOISTURE_TARGET_MAX_UPPER;
            min = SOIL_MOISTURE_TARGET_MIN_LOWER;
        }
        if (sensor == FLUID_LEVEL)
        {
            max = WATER_LEVEL_TARGET_MAX_UPPER;
            min = WATER_LEVEL_TARGET_MIN_LOWER;
        }
        if (sensor == WATERING_AMOUNT)
        {
            max = WATER_VOLUME_CALIBRATION_MAX_UPPER;
            min = WATER_VOLUME_CALIBRATION_MAX_LOWER;
        }
    }
    char message[23];
    sprintf(message, "Value Between [%d-%d]:", min, max);
    display_draw_text({MESSAGE_POS, message, BLACK, WHITE, 11});
    display_draw_text({{40, 41}, window_title, BLUE, WHITE, 14});

    display_draw_rect({{10, 60}, 200, 5, BLACK});
    if (sensor == LIGHT_INTENSITY)
    {
        display_draw_text({{17, 96}, "Taregt Value", BLACK, WHITE, 13});
        display_draw_rect({{20, 110}, 90, 28, BLACK});
        display_draw_rect({{21, 111}, 88, 26, GRAY});
    }
    else if (sensor == WATERING_AMOUNT)
    {
        display_draw_text({{17, 96}, "Taregt Value", BLACK, WHITE, 13});
        display_draw_rect({{20, 110}, 90, 28, BLACK});
        display_draw_rect({{21, 111}, 88, 26, GRAY});
    }

    else
    {
        display_draw_text({{25, 96}, "Min Value", BLACK, WHITE, 13});
        display_draw_rect({{20, 110}, 90, 28, BLACK});
        display_draw_rect({{21, 111}, 88, 26, GRAY});

        display_draw_text({{25, 146}, "Max Value", BLACK, WHITE, 13});
        display_draw_rect({{20, 160}, 90, 28, BLACK});
        display_draw_rect({{21, 161}, 88, 26, GRAY});
    }

    display_draw_rect({{20, 191}, 181, 55, BLACK});
    display_draw_text({{23, 194}, "Press (  ) To Delete", WHITE, BLACK, 10});
    display_draw_text({{69, 197}, "*", RED, BLACK, 14});
    display_draw_text({{23, 212}, "Hold (  ) To Cancel", WHITE, BLACK, 10});
    display_draw_text({{60, 215}, "*", RED, BLACK, 14});
    display_draw_text({{23, 230}, "Press (  ) To Enter", WHITE, BLACK, 10});
    display_draw_text({{68, 231}, "#", RED, BLACK, 9});
}

void on_off_text(uint8_t value, bool auto_f, bool on_f, bool off_f)
{
    display_draw_rect({{100, 69}, 100, 28, BLACK});
    if (auto_f && value == 0)
    {
        display_draw_rect({{103, 72}, 36, 23, GRAY});
        display_draw_text({{108, 78}, "Auto", GREEN, GRAY, 10});
        display_draw_text({{143, 78}, "ON", RED, BLACK, 10});
        display_draw_text({{168, 78}, "OFF", RED, BLACK, 10});
    }

    if (on_f && value == 1)
    {
        display_draw_rect({{140, 72}, 27, 23, GRAY});
        display_draw_text({{108, 78}, "Auto", RED, BLACK, 10});
        display_draw_text({{143, 78}, "ON", GREEN, GRAY, 10});
        display_draw_text({{168, 78}, "OFF", RED, BLACK, 10});
    }

    if (off_f && value == 2)
    {
        display_draw_rect({{168, 72}, 29, 23, GRAY});
        display_draw_text({{108, 78}, "Auto", RED, BLACK, 10});
        display_draw_text({{143, 78}, "ON", RED, BLACK, 10});
        display_draw_text({{168, 78}, "OFF", GREEN, GRAY, 10});
    }
}

void manual_auto_text(uint8_t value, bool auto_f, bool manual_f)
{
    if (auto_f && value == 0)
    {
        display_draw_rect({{100, 69}, 100, 28, BLACK});
        display_draw_rect({{104, 72}, 40, 23, GRAY});
        display_draw_text({{108, 78}, "Auto", GREEN, GRAY, 10});
        display_draw_text({{148, 78}, "Manual", RED, BLACK, 10});
    }
    auto_f = false;

    if (manual_f && value == 1)
    {
        display_draw_rect({{100, 69}, 100, 28, BLACK});
        display_draw_rect({{144, 72}, 53, 23, GRAY});
        display_draw_text({{108, 78}, "Auto", RED, BLACK, 10});
        display_draw_text({{148, 78}, "Manual", GREEN, GRAY, 10});
    }
    auto_f = false;
}

displays_t act_overwrite_window_state_text(char choice, uint8_t chosen_act)
{

    static uint8_t state = ON;
    static uint8_t click_conter = 0;
    static uint8_t enter_counter = 0;
    uint8_t min = 0;
    uint8_t max = 0;
    components_t component = (components_t)(chosen_act + 7);
    static bool act_overwrite_window_text_flag = true;

    if (act_overwrite_window_text_flag)
    {
        display_draw_rect({{10, 33}, 200, 220, BLACK});
        display_draw_rect({{15, 38}, 190, 210, WHITE});

        char window_title[20];

        sprintf(window_title, act_overwrite_menu_names[(choice - '0') - 2]);
        display_draw_text({{40, 41}, act_overwrite_menu_names[chosen_act], BLUE, WHITE, 14});
        display_draw_rect({{10, 60}, 200, 5, BLACK});
        display_draw_text({{18, 75}, "1. State", BLACK, WHITE, 14});
    }

    if (click_conter == 0 && auto_flag)
    {
        display_draw_text({{18, 100}, "0. Back", BLACK, WHITE, 14});
        display_draw_rect({{20, 191}, 181, 55, WHITE});
        display_draw_rect({{20, 208}, 181, 37, BLACK});
        display_draw_text({{23, 212}, "Press (   ) To Change", WHITE, BLACK, 10});
        display_draw_text({{68, 210}, "1", RED, BLACK, 14});
        display_draw_text({{23, 230}, "Press (  ) To Enter", WHITE, BLACK, 10});
        display_draw_text({{68, 231}, "#", RED, BLACK, 9});
    }
    if (click_conter == 1 && enter_counter == 1)
    {
        click_conter = 0;
        if (state != AUTO && (component == LAMP || component == HEATER))
        {
            if (component == LAMP)
            {
                max = LAMP_SHININESS_UPPER;
                min = LAMP_SHININESS_LOWER;
            }
            if (component == HEATER)
            {
                max = HEATER_WARMNESS_UPPER;
                min = HEATER_WARMNESS_LOWER;
            }
            char message[23];
            display_draw_rect({{18, 100}, 60, 20, WHITE});
            sprintf(message, "Value Between [%d-%d]:", min, max);
            display_draw_text({{18, 103}, message, BLACK, WHITE, 11});

            display_draw_text({{18, 118}, "Percentage", BLACK, WHITE, 13});
            display_draw_rect({{20, 133}, 90, 28, BLACK});
            display_draw_rect({{21, 134}, 88, 26, GRAY});
            display_draw_rect({{20, 191}, 181, 55, BLACK});
            display_draw_text({{23, 194}, "Press (  ) To Delete", WHITE, BLACK, 10});
            display_draw_text({{69, 197}, "*", RED, BLACK, 14});
            display_draw_text({{23, 212}, "Hold (  ) To Cancel", WHITE, BLACK, 10});
            display_draw_text({{60, 215}, "*", RED, BLACK, 14});
            display_draw_text({{23, 230}, "Press (  ) To Enter", WHITE, BLACK, 10});
            display_draw_text({{68, 231}, "#", RED, BLACK, 9});
        }
    }
    act_overwrite_window_text_flag = false;
    switch (choice)
    {
    case '1':
        if (click_conter == 0 && (component == LAMP || component == HEATER))
        {
            click_conter++;
            manual_flag = true;
            break;
        }
        if (click_conter == 1 && (component == LAMP || component == HEATER))
        {
            click_conter = 0;
            manual_flag = false;
            auto_flag = true;
        }
        break;
    case '#':
        enter_counter++;
        break;
    case '0':
        click_conter = 0;
        auto_flag = true;
        act_overwrite_window_text_flag = true;
        return OVERWRITE_ACT_MENU;
        break;

    default:
        break;
    }
    return OVERWRITE_ACT_WINDOW_STATE;
}

void default_state_window_text(uint8_t act_name_index)
{
    display_draw_rect({{10, 33}, 200, 220, BLACK});
    display_draw_rect({{15, 38}, 190, 210, WHITE});
    display_draw_text({{40, 41}, act_overwrite_menu_names[act_name_index], BLUE, WHITE, 14});
    display_draw_rect({{10, 60}, 200, 5, BLACK});
    display_draw_text({{18, 75}, "1. State", BLACK, WHITE, 14});
    display_draw_text({{18, 100}, "0. Back", BLACK, WHITE, 14});
    display_draw_rect({{20, 208}, 181, 37, BLACK});
    display_draw_text({{23, 212}, "Press (   ) To Change", WHITE, BLACK, 10});
    display_draw_text({{68, 210}, "1", RED, BLACK, 14});
    display_draw_text({{23, 230}, "Press (  ) To Enter", WHITE, BLACK, 10});
    display_draw_text({{68, 231}, "#", RED, BLACK, 9});
}

void act_overwrite_value_part_text(components_t act)
{
    uint8_t min = 0;
    uint8_t max = 0;
    if (act == LAMP)
    {
        max = LAMP_SHININESS_UPPER;
        min = LAMP_SHININESS_LOWER;
    }
    if (act == HEATER)
    {
        max = HEATER_WARMNESS_UPPER;
        min = HEATER_WARMNESS_LOWER;
    }
    char message[23];
    display_draw_rect({{18, 100}, 60, 20, WHITE});
    sprintf(message, "Value Between [%d-%d]:", min, max);
    display_draw_text({{18, 103}, message, BLACK, WHITE, 11});

    display_draw_text({{18, 118}, "Percentage", BLACK, WHITE, 13});
    display_draw_rect({{20, 133}, 90, 28, BLACK});
    display_draw_rect({{21, 134}, 88, 26, GRAY});
    display_draw_rect({{20, 191}, 181, 55, BLACK});
    display_draw_text({{23, 194}, "Press (  ) To Delete", WHITE, BLACK, 10});
    display_draw_text({{69, 197}, "*", RED, BLACK, 14});
    display_draw_text({{23, 212}, "Hold (  ) To Cancel", WHITE, BLACK, 10});
    display_draw_text({{60, 215}, "*", RED, BLACK, 14});
    display_draw_text({{23, 230}, "Press (  ) To Enter", WHITE, BLACK, 10});
    display_draw_text({{68, 231}, "#", RED, BLACK, 9});
}

void comp_status_page_one(bool *page_one_flag)
{
    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "SENSORS", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "Temperature Sensor", BLACK, WHITE, 13});
    draw_status_circle(system_data.sensor.temperature.status, {200, 40}, 7, false, false);
    display_draw_text({SECOND_LINE_POS, "Humidity Sensor", BLACK, WHITE, 13});
    draw_status_circle(system_data.sensor.humidity.status, {200, 70}, 7, false, false);
    display_draw_text({THIRD_LINE_POS, "Light Sensor", BLACK, WHITE, 13});
    draw_status_circle(system_data.sensor.light.status, {200, 100}, 7, false, false);

    display_draw_text({FORTH_LINE_POS, "Flow Meter Sensor", BLACK, WHITE, 13});
    draw_status_circle(system_data.sensor.flow_meter.status, {200, 130}, 7, false, false);

    display_draw_text({FIFTH_LINE_POS, "Soil Moisture Sensor", BLACK, WHITE, 13});
    draw_status_circle(system_data.sensor.moisture.status, {200, 160}, 7, false, false);

    display_draw_text({SIXTH_LINE_POS, "Water Level Sensor", BLACK, WHITE, 13});
    draw_status_circle(system_data.sensor.water_level.status, {200, 190}, 7, false, false);
    display_draw_line({{0, 246}, 240, BLACK});
    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{178, 250}, "Next(1)", BLACK, WHITE, 13});
    *page_one_flag = false;
}

void comp_status_page_two(bool *page_two_flag)
{
    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "ACTUATORS", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "Lamp", BLACK, WHITE, 13});
    draw_status_circle(system_data.actuator.lamp.status, {200, 40}, 7, false, false);
    display_draw_text({SECOND_LINE_POS, "Heater", BLACK, WHITE, 13});
    draw_status_circle(system_data.actuator.heater.status, {200, 70}, 7, false, false);
    display_draw_text({THIRD_LINE_POS, "Water Pump", BLACK, WHITE, 13});
    draw_status_circle(system_data.actuator.water_pump.status, {200, 100}, 7, false, false);
    display_draw_text({FORTH_LINE_POS, "Water Valve", BLACK, WHITE, 13});
    draw_status_circle(system_data.actuator.water_valve.status, {200, 130}, 7, false, false);
    display_draw_text({FIFTH_LINE_POS, "Window", BLACK, WHITE, 13});
    draw_status_circle(system_data.actuator.window.status, {200, 160}, 7, false, false);
    display_draw_text({SIXTH_LINE_POS, "Fans", BLACK, WHITE, 13});
    draw_status_circle(system_data.actuator.fans.status, {200, 190}, 7, false, false);
    display_draw_line({{0, 246}, 240, BLACK});
    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{83, 250}, "(#)Home", BLACK, WHITE, 13});
    display_draw_text({{178, 250}, "Next(1)", BLACK, WHITE, 13});
    *page_two_flag = false;
}

void comp_status_page_three(bool *page_three_flag)
{
    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "COMMUNICATION", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "Terminal", BLACK, WHITE, 13});
    draw_status_circle(system_data.communication.terminal.status, {190, 40}, 7, false, false);
    display_draw_text({{10, 85}, "ESP-32 ", BLACK, WHITE, 13});
    draw_status_circle(system_data.communication.esp32.status, {190, 85}, 7, false, true);
    display_draw_text({{10,130}, "RTC", BLACK, WHITE, 13});
    draw_status_circle(system_data.communication.rtc.status, {190, 130}, 7, false, false);
    display_draw_text({{10, 175}, "SD Card", BLACK, WHITE, 13});
    draw_status_circle(system_data.communication.sdcard.status, {190, 175}, 7, false, false);

    display_draw_line({{0, 246}, 240, BLACK});
    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{83, 250}, "(#)Home", BLACK, WHITE, 13});
    display_draw_text({{178, 250}, "Next(1)", BLACK, WHITE, 13});
    *page_three_flag = false;
}

void comp_status_page_four(bool *page_four_flag)
{
    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "HMI", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "LCD", BLACK, WHITE, 13});
    draw_status_circle(system_data.hmi.display.status, {200, 40}, 7, false, false);
    display_draw_text({SECOND_LINE_POS, "EEPROM", BLACK, WHITE, 13});
    draw_status_circle(system_data.hmi.eeprom.status, {200, 70}, 7, false, false);
    display_draw_text({THIRD_LINE_POS, "Buzzer", BLACK, WHITE, 13});
    draw_status_circle(system_data.hmi.buzzer.status, {200, 100}, 7, false, false);
    display_draw_text({FORTH_LINE_POS, "Keypad", BLACK, WHITE, 13});
    draw_status_circle(system_data.hmi.keypad.status, {200, 130}, 7, false, false);
    display_draw_text({FIFTH_LINE_POS, "RGB", BLACK, WHITE, 13});
    draw_status_circle(system_data.hmi.rgb.status, {200, 160}, 7, false, false);

    display_draw_line({{0, 246}, 240, BLACK});
    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{170, 250}, "(#)Home", BLACK, WHITE, 13});
    *page_four_flag = false;
}

void sys_info_page_one(bool *page_one_flag)
{
    char temp_value[8] = {'\0'};
    char float_to_string[5] = {'\0'};

    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "SENSORS", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "Temperature", BLACK, WHITE, 13});
    snprintf(float_to_string, sizeof(float_to_string), "%f", system_data.sensor.temperature.value);
    sprintf(temp_value, "%s C", float_to_string);
    display_draw_text({{175, 40}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    memset(float_to_string, '\0', sizeof(float_to_string));

    display_draw_text({SECOND_LINE_POS, "Humidity", BLACK, WHITE, 13});
    sprintf(temp_value, "%d %%", system_data.sensor.humidity.value);
    display_draw_text({{175, 70}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({THIRD_LINE_POS, "Light Intensity", BLACK, WHITE, 13});
    sprintf(temp_value, "%d %%", system_data.sensor.light.intensity);
    display_draw_text({{175, 100}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({FORTH_LINE_POS, "Flow Meter", BLACK, WHITE, 13});
    sprintf(temp_value, "%d ml/s", system_data.sensor.flow_meter.value);
    display_draw_text({{175, 130}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({FIFTH_LINE_POS, "Soil Moisture", BLACK, WHITE, 13});
    sprintf(temp_value, "%d %%", system_data.sensor.moisture.value);
    display_draw_text({{175, 160}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({SIXTH_LINE_POS, "Tank Water Level", BLACK, WHITE, 13});
    sprintf(temp_value, "%d %%", system_data.sensor.water_level.value);
    display_draw_text({{175, 190}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_line({{0, 246}, 240, BLACK});

    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{175, 250}, "Next(1)", BLACK, WHITE, 13});

    *page_one_flag = false;
}

void sys_info_page_two(bool *page_two_flag)
{
    char temp_value[8] = {'\0'};
    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "ACTUATORS", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "Lamp Shininess", BLACK, WHITE, 14});
    sprintf(temp_value, "%d %%", system_data.actuator.lamp.shininess.value);
    if (system_data.actuator.lamp.shininess.overwritten)
    {
        display_draw_text({{175, 40}, temp_value, RED, WHITE, 13});
    }
    else
    {
        display_draw_text({{175, 40}, temp_value, BLACK, WHITE, 13});
    }
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({SECOND_LINE_POS, "Heater Warmness", BLACK, WHITE, 14});
    sprintf(temp_value, "%d %%", system_data.actuator.heater.warmness.value);
    if (system_data.actuator.heater.warmness.overwritten)
    {
        display_draw_text({{175, 70}, temp_value, RED, WHITE, 13});
    }
    else
    {
        display_draw_text({{175, 70}, temp_value, BLACK, WHITE, 13});
    }
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({THIRD_LINE_POS, "Water Pump", BLACK, WHITE, 14});
    if (system_data.actuator.water_pump.state.overwritten)
    {
        if (system_data.actuator.water_pump.state.value == ON)
        {
            display_draw_text({{175, 100}, "ON", RED, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 100}, "OFF", RED, WHITE, 13});
        }
    }
    else
    {
        if (system_data.actuator.water_pump.state.value == ON)
        {
            display_draw_text({{175, 100}, "ON", BLACK, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 100}, "OFF", BLACK, WHITE, 13});
        }
    }

    display_draw_text({FORTH_LINE_POS, "Water Valve", BLACK, WHITE, 14});
    if (system_data.actuator.water_valve.state.overwritten)
    {
        if (system_data.actuator.water_valve.state.value == ON)
        {
            display_draw_text({{175, 130}, "ON", RED, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 130}, "OFF", RED, WHITE, 13});
        }
    }
    else
    {
        if (system_data.actuator.water_valve.state.value == ON)
        {
            display_draw_text({{175, 130}, "ON", BLACK, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 130}, "OFF", BLACK, WHITE, 13});
        }
    }

    display_draw_text({FIFTH_LINE_POS, "Window", BLACK, WHITE, 14});
    if (system_data.actuator.window.state.overwritten)
    {
        if (system_data.actuator.window.state.value == OPEN)
        {
            display_draw_text({{175, 160}, "OPEN", RED, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 160}, "CLOSE", RED, WHITE, 13});
        }
    }
    else
    {
        if (system_data.actuator.window.state.value == OPEN)
        {
            display_draw_text({{175, 160}, "OPEN", BLACK, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 160}, "CLOSE", BLACK, WHITE, 13});
        }
    }

    display_draw_text({SIXTH_LINE_POS, "Fans", BLACK, WHITE, 14});
    if (system_data.actuator.fans.state.overwritten)
    {
        if (system_data.actuator.fans.state.value == ON)
        {
            display_draw_text({{175, 190}, "ON", RED, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 190}, "OFF", RED, WHITE, 13});
        }
    }
    else
    {
        if (system_data.actuator.fans.state.value == ON)
        {
            display_draw_text({{175, 190}, "ON", BLACK, WHITE, 13});
        }
        else
        {
            display_draw_text({{175, 190}, "OFF", BLACK, WHITE, 13});
        }
    }

    display_draw_text({{5, 230}, "Values in        are overwritten", BLACK, WHITE, 10});
    display_draw_text({{65, 230}, "RED", RED, WHITE, 9});

    display_draw_line({{0, 246}, 240, BLACK});
    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{83, 250}, "(#)Home", BLACK, WHITE, 13});
    display_draw_text({{178, 250}, "Next(1)", BLACK, WHITE, 13});

    *page_two_flag = false;
}

void sys_info_page_three(bool *page_three_flag)
{
    char temp_value[8] = {'\0'};
    data_t system_data = get_candata();
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "SYSTEM INFO", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({FIRST_LINE_POS, "Internet", BLACK, WHITE, 14});
    if (system_data.communication.esp32.status == OKAY)
    {
        display_draw_text({{20, 60}, "CONNECTED", GREEN, WHITE, 13});
    }
    else
    {
        display_draw_text({{20, 60}, "DISCONNECTED", RED, WHITE, 13});
    }

    display_draw_text({THIRD_LINE_POS, "Memory Free Space", BLACK, WHITE, 14});
    sprintf(temp_value, "%d KB", system_data.communication.sdcard.free_space);
    display_draw_text({{20, 120}, temp_value, BLACK, WHITE, 13});

    display_draw_text({FIFTH_LINE_POS, "Time", BLACK, WHITE, 14});
    sprintf(temp_value, "%s ", system_data.communication.rtc.datetime);
    display_draw_text({{20, 180}, temp_value, BLACK, WHITE, 13});

    display_draw_line({{0, 246}, 240, BLACK});

    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});
    display_draw_text({{83, 250}, "(#)Home", BLACK, WHITE, 13});
    display_draw_text({{178, 250}, "Next(1)", BLACK, WHITE, 13});

    *page_three_flag = false;
}

void sys_info_page_four(bool *page_four_flag, cal_tar_data_t data)
{
    char temp_value[5] = {'\0'};
    display_draw_rect(FLASH_MENU_PART);
    display_draw_text({HAED_SUBJECT_POS, "Cal & Tar Values", RED, WHITE, 14});
    display_draw_line({{0, 29}, 240, BLACK});
    display_draw_line({{0, 31}, 240, BLACK});
    display_draw_text({{65, 40}, "C_Min", BLACK, WHITE, 9});
    display_draw_text({{110, 40}, "C_Max", BLACK, WHITE, 9});
    display_draw_text({{155, 40}, "T_Min", BLACK, WHITE, 9});
    display_draw_text({{195, 40}, "T_Max", BLACK, WHITE, 9});
    display_draw_text({{5, 70}, "Tempe", BLACK, WHITE, 12});

    //display_draw_rect{}
    sprintf(temp_value, "%d", data.temp_cal_min);
    display_draw_text({{75, 70}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.temp_cal_max);
    display_draw_text({{120, 70}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.temp_tar_min);
    display_draw_text({{165, 70}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.temp_tar_max);
    display_draw_text({{200, 70}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({{5, 100}, "Humidity", BLACK, WHITE, 12});
    sprintf(temp_value, "%d", data.hum_cal_min);
    display_draw_text({{75, 100}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.hum_cal_max);
    display_draw_text({{120, 100}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.hum_tar_min);
    display_draw_text({{165, 100}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.hum_tar_max);
    display_draw_text({{200, 100}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({{5, 130}, "Light", BLACK, WHITE, 12});
    sprintf(temp_value, "%d", data.light_tar_min);
    display_draw_text({{165, 130}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({{8, 160}, "Watering", BLACK, WHITE, 10});
    sprintf(temp_value, "%d", data.watering_tar_max);
    display_draw_text({{200, 160}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({{5, 190}, "Moisture", BLACK, WHITE, 12});
    sprintf(temp_value, "%d", data.soi_tar_min);
    display_draw_text({{165, 190}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.soi_tar_max);
    display_draw_text({{200, 190}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_text({{5, 220}, "Water_Level", BLACK, WHITE, 11});
    sprintf(temp_value, "%d", data.wat_lev_tar_min);
    display_draw_text({{165, 220}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));
    sprintf(temp_value, "%d", data.wat_lev_tar_max);
    display_draw_text({{200, 220}, temp_value, BLACK, WHITE, 13});
    memset(temp_value, '\0', sizeof(temp_value));

    display_draw_line({{0, 246}, 240, BLACK});
    display_draw_text({{170, 250}, "(#)Home", BLACK, WHITE, 13});
    display_draw_text({{5, 250}, "(0)Back", BLACK, WHITE, 13});

    *page_four_flag = false;
}

void draw_status_circle(uint8_t comp_status, position_t circle_pos, int16_t radian, bool sys_status_flag, bool esp_status_flag)
{
    int16_t x = 0;
    int16_t y = 0;
    y = circle_pos.y + radian + 3U;
    if (comp_status == UNINITIALIZED)
    {
        x = circle_pos.x - ((radian * 2) + 1U);
        if (!sys_status_flag)
        {
            circle_pos.x += 6U;
        }
        char warning[] = UNINITIALIZED_MSG;
        display_draw_circle({circle_pos, radian, YELLOW});
        if (sys_status_flag)
        {
            display_draw_rect({{185, 300}, 52, 13, BLACK});
            display_draw_text({{x, y}, "WARNING", YELLOW, BLACK, 8});
        }
        else
        {
            display_draw_text({{x, y}, warning, BLACK, WHITE, 8});
        }
    }
    else if(comp_status == OKAY)
    {
        char okay[] = OKAY_MSG;
        x = circle_pos.x - (radian + 4U);
        if (!sys_status_flag)
        {
            circle_pos.x += 3U;
        }
        display_draw_circle({circle_pos, radian, GREEN});
        if (sys_status_flag)
        {
            display_draw_rect({{185, 300}, 52, 13, BLACK});
            display_draw_text({{x, y}, "OKAY", GREEN, BLACK, 8});
        }
        else
        {
            display_draw_text({{x, y}, okay, BLACK, WHITE, 8});
        }
    }
    else
    {
        char error[] = "ERROR";
        x = circle_pos.x - (radian + 5U);
        if (!sys_status_flag)
        {
            circle_pos.x += 4U;
        }
        display_draw_circle({circle_pos, radian, RED});
        if (sys_status_flag)
        {
            display_draw_rect({{185, 300}, 52, 13, BLACK});
            display_draw_text({{x, y}, "ERROR", RED, BLACK, 8});
        }
        else
        {
            if (esp_status_flag)
            {
                int16_t y_second = y + 10;
                if (I2C_ERROR == comp_status)
                {
                    display_draw_text({{x, y}, "I2C Error", BLACK, WHITE, 8});
                }
                if (NTP_ERROR == comp_status)
                {
                    display_draw_text({{x, y}, "NTP Error", BLACK, WHITE, 8});
                }
                if (WIFI_DISCONNECTED == comp_status)
                {
                    display_draw_text({{x, y}, "WiFi", BLACK, WHITE, 8});
                    display_draw_text({{x, y_second}, "Disconnected", BLACK, WHITE, 8});
                }
                if (MQTT_DISCONNECTED == comp_status)
                {
                    display_draw_text({{x, y}, "MQTT", BLACK, WHITE, 8});
                    display_draw_text({{x, y_second}, "Disconnected", BLACK, WHITE, 8});
                }
                if (MQTT_PUBLISH_ERROR == comp_status)
                {
                    display_draw_text({{x, y}, "MQTT", BLACK, WHITE, 8});
                    display_draw_text({{x, y_second}, "Publish Error", BLACK, WHITE, 8});
                }

            }

            else
            {
                display_draw_text({{x, y}, error, BLACK, WHITE, 8});
            }
        }
    }
}
