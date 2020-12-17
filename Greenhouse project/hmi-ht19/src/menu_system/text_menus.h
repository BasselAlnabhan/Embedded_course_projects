#ifndef TEXT_MENUS_H
#define TEXT_MENUS_H

#include <bsp.h>
#include <stdlib.h>
#include <common.h>
#include <candata.h>
#include <canbus.h>
#include <display.h>
#include <keypad_driver.h>
#include <buzzer_driver.h>

#define GRAY 0xBDF7

typedef enum
{
    TEMPERATURE,
    HUMIDITY,
    LIGHT_INTENSITY,
    WATERING_AMOUNT,
    SOIL_MOISTURE,
    FLUID_LEVEL,
    FLOW_METER,
    LAMP,
    HEATER,
    PUMP,
    VALAE,
    WINDOW,
    FANS
} components_t;

typedef enum
{
    MAIN_MENU,
    SYS_OVERVIEW,
    COMPONENTS_STATUS,
    SEN_STATUS,
    ACT_STATUS,
    HMI_COMMUNICATION_STATUS,
    SET_CAL_MENU,
    CAL_SET_WINDOW,
    SET_TAR_MENU,
    TAR_SET_WINDOW,
    OVERWRITE_ACT_MENU,
    OVERWRITE_ACT_WINDOW_STATE,
    OVERWRITE_ACT_WINDOW_VALUE_PART,
    ACT_MENU,
    ACT_STATE_MENU,
} displays_t;

typedef struct
{
    uint8_t temp_cal_min;
    uint8_t temp_cal_max;
    uint8_t temp_tar_min;
    uint8_t temp_tar_max;
    uint8_t hum_cal_min;
    uint8_t hum_cal_max;
    uint8_t hum_tar_min;
    uint8_t hum_tar_max;
    uint8_t light_tar_min;
    uint8_t soi_tar_min;
    uint8_t soi_tar_max;
    uint8_t wat_lev_tar_min;
    uint8_t wat_lev_tar_max;
    uint16_t watering_tar_max;
} cal_tar_data_t;

void main_menu_text(void);

void set_cal_menu_text(void);

void set_tar_menu_text(void);

void act_overwrite_menu_text(void);

displays_t act_overwrite_window_state_text(char choice, uint8_t chosen_act);

void write_cal_tar_window_text(components_t sensor, uint8_t cal_or_tar, bool *text_flage);

void default_state_window_text(uint8_t act_name_index);

void manual_auto_text(uint8_t value, bool auto_f, bool manual_f);

void on_off_text(uint8_t value, bool auto_f, bool on_f, bool off_f);

void act_overwrite_value_part_text(components_t act);

void sys_info_page_one(bool *page_one_flag);

void sys_info_page_two(bool *page_two_flag);

void sys_info_page_three(bool *page_three_flag);

void draw_status_circle(uint8_t comp_status, position_t circle_pos, int16_t radian, bool sys_status_flag, bool esp_status_flag);

void comp_status_page_one(bool *page_one_flag);

void comp_status_page_two(bool *page_two_flag);

void comp_status_page_three(bool *page_three_flag);

void comp_status_page_four(bool *page_four_flag);

void sys_info_page_four(bool *page_four_flag, cal_tar_data_t data);

#endif /* TEXT_MENUS_H */
