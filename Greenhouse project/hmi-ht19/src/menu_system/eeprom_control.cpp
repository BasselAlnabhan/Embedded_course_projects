/**
 * @file eeprom_control.cpp
 * @author Wael Al Henawe (wael.henawe@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <eeprom_control.h>
#include <eeprom_driver.h>
#include <managing_entered_value.h>

#define EEPROM_BASE 0x00U
#define TEMP_CAL_MIN_ADR EEPROM_BASE                                                                  // The minimum calibration value of temperature     | [0, 50]
#define TEMP_CAL_MAX_ADR (TEMP_CAL_MIN_ADR + sizeof(((temperature_t *)NULL)->calibration.min.value))  // The maximum calibration value of temperature     | [0, 50]
#define TEMP_TAR_MIN_ADR (TEMP_CAL_MAX_ADR + sizeof(((temperature_t *)NULL)->calibration.max.value))  // The ambient minmum target temperature            | [0, 50]
#define TEMP_TAR_MAX_ADR (TEMP_TAR_MIN_ADR + sizeof(((temperature_t *)NULL)->target.min.value))       // The ambient maximum target temperature           | [0, 50]
#define HUMI_CAL_MIN_ADR (TEMP_TAR_MAX_ADR + sizeof(((temperature_t *)NULL)->target.max.value))       // The minimum calibration value of humidity        | [0, 100]
#define HUMI_CAL_MAX_ADR (HUMI_CAL_MIN_ADR + sizeof(((humidity_t *)NULL)->calibration.min.value))     // The maximum calibration value of humidity        | [0, 100]
#define HUMI_TAR_MIN_ADR (HUMI_CAL_MAX_ADR + sizeof(((humidity_t *)NULL)->calibration.max.value))     // The minmum target humidity                       | [0, 100]
#define HUMI_TAR_MAX_ADR (HUMI_TAR_MIN_ADR + sizeof(((humidity_t *)NULL)->target.min.value))          // The maximum target humidity                      | [0, 100]
#define LIGH_INT_TAR_ADR (HUMI_TAR_MAX_ADR + sizeof(((humidity_t *)NULL)->target.max.value))          // The maximum target light intensity               | [0, 100]
#define WAT_LEV_TAR_MIN_ADR (LIGH_INT_TAR_ADR + sizeof(((lamp_t *)NULL)->shininess.value))            // The minmum target water level                    | [0, 100]
#define WAT_LEV_TAR_MAX_ADR (WAT_LEV_TAR_MIN_ADR + sizeof(((water_level_t *)NULL)->target.min.value)) // The maximum target water level                   | [0, 100]
#define SOI_MOI_TAR_MIN_ADR (WAT_LEV_TAR_MAX_ADR + sizeof(((water_level_t *)NULL)->target.max.value)) // The minimum target soil moisture                 | [0, 100]
#define SOI_MOI_TAR_MAX_ADR (SOI_MOI_TAR_MIN_ADR + sizeof(((moisture_t *)NULL)->target.min.value))    // The maximum target soil moisture                 | [0, 100]
#define WAT_TAR_VOL_MAX_ADR (SOI_MOI_TAR_MAX_ADR + sizeof(((moisture_t *)NULL)->target.max.value))    // The maximum target volume of consumed water in every watering | [ 500, 2000 ]

#define USED_MEM_SIZE (WAT_TAR_VOL_MAX_ADR + sizeof(((water_pump_t *)NULL)->calibration.value))

#define MIN_VAL_INDEX 0U
#define MAX_VAL_INDEX 1U

#define DEF_TEMP_TAR_MIN 15U
#define DEF_TEMP_TAR_MAX 30U
#define DEF_HUMI_TAR_MIN 30U
#define DEF_HUMI_TAR_MAX 80U
#define DEF_LIGH_INT_TAR 50U
#define DEF_WAT_LEV_TAR_MIN 50U
#define DEF_WAT_LEV_TAR_MAX 90U
#define DEF_SOI_MOI_TAR_MIN 30U
#define DEF_SOI_MOI_TAR_MAX 60U
#define DEF_WAT_TAR_VOL_MAX 1800U

#define MAX_FOR_EIGHT_BITS 255


const static uint16_t cal_address_map[2][2] = {{TEMP_CAL_MIN_ADR,
                                                TEMP_CAL_MAX_ADR},
                                               {HUMI_CAL_MIN_ADR,
                                                HUMI_CAL_MAX_ADR}};

const static uint16_t tar_address_map[6][2] = {{TEMP_TAR_MIN_ADR,
                                                TEMP_TAR_MAX_ADR},
                                               {HUMI_TAR_MIN_ADR,
                                                HUMI_TAR_MAX_ADR},
                                               {0U, LIGH_INT_TAR_ADR},
                                               {0U, WAT_TAR_VOL_MAX_ADR},
                                               {SOI_MOI_TAR_MIN_ADR,
                                                SOI_MOI_TAR_MAX_ADR},
                                               {WAT_LEV_TAR_MIN_ADR,
                                                WAT_LEV_TAR_MAX_ADR}};

static uint8_t cal_data[2][2] = {0};
static uint16_t tar_data[6][2] = {0};

static volatile uint16_t min_val = 0;
static volatile uint16_t max_val = 0;

static void parse_buffer(char *buffer);

bool validate_input_values(components_t *component, displays_t *cal_or_tar_or_overwrite, char *buffer)
{
    bool statue = false;
    parse_buffer(buffer);
    if (*component == LIGHT_INTENSITY || *component == WATERING_AMOUNT || *component == LAMP || *component == HEATER)
    {
        max_val = min_val;
        min_val = 0;
    }
    if (min_val > max_val)
    {
        return statue;
    }

    if(*component != WATERING_AMOUNT)
    {
        if(max_val > MAX_FOR_EIGHT_BITS)
        {
            return statue;
        }
    }

    if (*cal_or_tar_or_overwrite == CAL_SET_WINDOW)
    {

        if (*component == TEMPERATURE)
        {
            if (set_temperature_calibration_min(min_val))
            {
                statue = set_temperature_calibration_max(max_val);
            }
        }
        if (*component == HUMIDITY)
        {
            if (set_humidity_calibration_min(min_val))
            {
                statue = set_humidity_calibration_max(max_val);
            }
        }
    }
    if (*cal_or_tar_or_overwrite == TAR_SET_WINDOW)
    {
        if (*component == TEMPERATURE)
        {
            if (min_val > cal_data[TEMPERATURE][1] ||
                min_val < cal_data[TEMPERATURE][0] ||
                max_val > cal_data[TEMPERATURE][1] ||
                max_val < cal_data[TEMPERATURE][0])
            {
                statue = false;
                return statue;
            }
            if (set_temperature_target_min(min_val))
            {
                statue = set_temperature_target_max(max_val);
            }
        }
        if (*component == HUMIDITY)
        {
            if (min_val > cal_data[HUMIDITY][1] ||
                min_val < cal_data[HUMIDITY][0] ||
                max_val > cal_data[HUMIDITY][1] ||
                max_val < cal_data[HUMIDITY][0])
            {
                statue = false;
                return statue;
            }
            if (set_humidity_target_min(min_val))
            {
                statue = set_humidity_target_max(max_val);
            }
        }
        if (*component == LIGHT_INTENSITY)
        {
            statue = set_light_intensity_target_min(max_val);
        }

        if (*component == SOIL_MOISTURE)
        {
            if (set_soil_moisture_target_min(min_val))
            {
                statue = set_soil_moisture_target_max(max_val);
            }
        }
        if (*component == FLUID_LEVEL)
        {
            statue = set_water_level_target_min(max_val);
        }
        if (*component == WATERING_AMOUNT)
        {
            statue = set_water_volume_calibration_max(max_val);
        }
    }

    if (*cal_or_tar_or_overwrite == OVERWRITE_ACT_WINDOW_VALUE_PART)
    {
        if (*component == LAMP)
        {
            overwrite_lamp_shininess(YES);
            statue = set_lamp_shininess(max_val);
            Serial.printf("state %d\n", statue);
            Serial.printf("value %d\n", max_val);
        }
        if (*component == HEATER)
        {
            overwrite_heater_warmness(YES);
            statue = set_heater_warmness(max_val);
            Serial.printf("state %d\n", statue);
            Serial.printf("value %d\n", max_val);
        }
    }

    return statue;
}

static void parse_buffer(char *buffer)
{
    min_val = 0;
    max_val = 0;
    uint8_t counter = 0;
    for (uint8_t x = 0; buffer[x] != '\0'; x++)
    {
        if (buffer[x] != ',')
        {
            if (counter == 0)
            {
                min_val = (min_val * 10) + (buffer[x] - '0');
            }
            if (counter == 1)
            {
                max_val = (max_val * 10) + (buffer[x] - '0');
            }
        }
        else
        {
            counter++;
        }
    }
}

uint8_t update_eeprom(components_t *component, displays_t *cal_or_tar)
{

    uint8_t statue = false;

    switch (*cal_or_tar)
    {
    case CAL_SET_WINDOW:
    {
        cal_data[*component][MIN_VAL_INDEX] = min_val;
        cal_data[*component][MAX_VAL_INDEX] = max_val;

        PRINTF("%d - %d\n", min_val, max_val);
        PRINTF("%d - %d\n", cal_address_map[*component][MIN_VAL_INDEX], cal_address_map[*component][MAX_VAL_INDEX]);

        statue = eeprom_driver_write(cal_address_map[*component][MIN_VAL_INDEX], (uint8_t *)&min_val, 1);
        statue = eeprom_driver_write(cal_address_map[*component][MAX_VAL_INDEX], (uint8_t *)&max_val, 1);
    }
    break;

    case TAR_SET_WINDOW:
    {
        if (*component == WATERING_AMOUNT)
        {
            tar_data[*component][MAX_VAL_INDEX] = max_val;
            statue = eeprom_driver_write(tar_address_map[*component][MIN_VAL_INDEX], (uint8_t *)&max_val, 2);
        }
        else
        {
            if (*component == LIGHT_INTENSITY)
            {
                tar_data[*component][MAX_VAL_INDEX] = max_val;
                statue = eeprom_driver_write(tar_address_map[*component][MIN_VAL_INDEX], (uint8_t *)&max_val, 1);
            }
            else
            {

                tar_data[*component][MIN_VAL_INDEX] = min_val;
                tar_data[*component][MAX_VAL_INDEX] = max_val;
                statue = eeprom_driver_write(tar_address_map[*component][MIN_VAL_INDEX], (uint8_t *)&min_val, 1);
                statue = eeprom_driver_write(tar_address_map[*component][MAX_VAL_INDEX], (uint8_t *)&max_val, 1);
            }
        }
    }
    break;

    default:
        break;
    }
#ifdef TARGET
    uint16_t x = 0;
    for (uint8_t i = 0; i < (USED_MEM_SIZE - 1); i++)
    {
        uint8_t len = (i == USED_MEM_SIZE - 2) ? 2 : 1;
        eeprom_driver_read(i, (uint8_t *)&x, len);
        PRINTF("%d ", x);
    }
#endif
    return statue;
}

static void config_cal_tar_val_helper(bool (*set)(uint8_t), components_t comp, uint8_t map_index, displays_t cal_or_tar)
{
    uint8_t value = 0;
    switch (cal_or_tar)
    {
    case CAL_SET_WINDOW:
        eeprom_driver_read(cal_address_map[comp][map_index], &value, sizeof(value));
        PRINTF("CAL: cal_or_tar: %d, ADD: %d , Comp: %d, index: %d, value: %d\n", cal_or_tar, (uint8_t)cal_address_map[comp][map_index], comp, map_index, value);
        cal_data[comp][map_index] = value;
        set(value);
        break;
    case TAR_SET_WINDOW:
        eeprom_driver_read(tar_address_map[comp][map_index], &value, sizeof(value));
        PRINTF("TAR: cal_or_tar: %d, ADD: %d , Comp: %d, index: %d, value: %d\n", cal_or_tar, (uint8_t)cal_address_map[comp][map_index], comp, map_index, value);

        tar_data[comp][map_index] = value;
        set(value);
        break;

    default:
        break;
    }
}

void config_cal_tar_val(void)
{
    config_cal_tar_val_helper(set_temperature_calibration_min, TEMPERATURE, MIN_VAL_INDEX, CAL_SET_WINDOW);
    config_cal_tar_val_helper(set_temperature_calibration_max, TEMPERATURE, MAX_VAL_INDEX, CAL_SET_WINDOW);
    config_cal_tar_val_helper(set_temperature_target_min, TEMPERATURE, MIN_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_temperature_target_max, TEMPERATURE, MAX_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_humidity_calibration_min, HUMIDITY, MIN_VAL_INDEX, CAL_SET_WINDOW);
    config_cal_tar_val_helper(set_humidity_calibration_max, HUMIDITY, MAX_VAL_INDEX, CAL_SET_WINDOW);
    config_cal_tar_val_helper(set_humidity_target_min, HUMIDITY, MIN_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_humidity_target_max, HUMIDITY, MAX_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_light_intensity_target_min, LIGHT_INTENSITY, MAX_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_soil_moisture_target_min, SOIL_MOISTURE, MIN_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_soil_moisture_target_max, SOIL_MOISTURE, MAX_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_water_level_target_min, FLUID_LEVEL, MIN_VAL_INDEX, TAR_SET_WINDOW);
    config_cal_tar_val_helper(set_water_level_target_max, FLUID_LEVEL, MAX_VAL_INDEX, TAR_SET_WINDOW);
    uint16_t watring_value = 0;
    eeprom_driver_read(tar_address_map[WATERING_AMOUNT][MAX_VAL_INDEX], (uint8_t *)&watring_value, sizeof(watring_value));
    tar_data[WATERING_AMOUNT][MAX_VAL_INDEX] = watring_value;
    set_water_volume_calibration_max(watring_value);
    PRINTF("TAR: cal_or_tar: %d, ADD: %d , Comp: %d, index: %d, value: %d\n", TAR_SET_WINDOW, (uint8_t)cal_address_map[WATERING_AMOUNT][MAX_VAL_INDEX], WATERING_AMOUNT, MAX_VAL_INDEX, watring_value);
}

void config_eeprom(bool first_start)
{
    if (first_start)
    {
        cal_data[TEMPERATURE][0] = TEMPERATURE_CALIBRATION_MIN_LOWER;
        eeprom_driver_write(TEMP_CAL_MIN_ADR, (uint8_t *)&cal_data[TEMPERATURE][0], sizeof(uint8_t));
        set_temperature_calibration_min(cal_data[TEMPERATURE][0]);
        cal_data[TEMPERATURE][1] = TEMPERATURE_CALIBRATION_MAX_UPPER;
        eeprom_driver_write(TEMP_CAL_MAX_ADR, (uint8_t *)&cal_data[TEMPERATURE][1], sizeof(uint8_t));
        set_temperature_calibration_max(cal_data[TEMPERATURE][1]);
        tar_data[TEMPERATURE][0] = DEF_TEMP_TAR_MIN;
        eeprom_driver_write(TEMP_TAR_MIN_ADR, (uint8_t *)&tar_data[TEMPERATURE][0], sizeof(uint8_t));
        set_temperature_target_min(tar_data[TEMPERATURE][0]);
        tar_data[TEMPERATURE][1] = DEF_TEMP_TAR_MAX;
        eeprom_driver_write(TEMP_TAR_MAX_ADR, (uint8_t *)&tar_data[TEMPERATURE][1], sizeof(uint8_t));
        set_temperature_target_max(tar_data[TEMPERATURE][1]);

        cal_data[HUMIDITY][0] = HUMIDITY_CALIBRATION_MIN_LOWER;
        eeprom_driver_write(HUMI_CAL_MIN_ADR, (uint8_t *)&cal_data[HUMIDITY][0], sizeof(uint8_t));
        set_humidity_calibration_min(cal_data[HUMIDITY][0]);
        cal_data[HUMIDITY][1] = HUMIDITY_CALIBRATION_MAX_UPPER;
        eeprom_driver_write(HUMI_CAL_MAX_ADR, (uint8_t *)&cal_data[HUMIDITY][1], sizeof(uint8_t));
        set_humidity_calibration_max(cal_data[HUMIDITY][1]);
        tar_data[HUMIDITY][0] = DEF_HUMI_TAR_MIN;
        eeprom_driver_write(HUMI_TAR_MIN_ADR, (uint8_t *)&tar_data[HUMIDITY][0], sizeof(uint8_t));
        set_humidity_target_min(tar_data[HUMIDITY][0]);
        tar_data[HUMIDITY][1] = DEF_HUMI_TAR_MAX;
        eeprom_driver_write(HUMI_TAR_MAX_ADR, (uint8_t *)&tar_data[HUMIDITY][1], sizeof(uint8_t));
        set_humidity_target_max(tar_data[HUMIDITY][1]);

        tar_data[LIGHT_INTENSITY][1] = DEF_LIGH_INT_TAR;
        eeprom_driver_write(LIGH_INT_TAR_ADR, (uint8_t *)&tar_data[LIGHT_INTENSITY][1], sizeof(uint8_t));
        set_light_intensity_target_min(tar_data[LIGHT_INTENSITY][1]);

        tar_data[FLUID_LEVEL][0] = DEF_WAT_LEV_TAR_MIN;
        eeprom_driver_write(WAT_LEV_TAR_MIN_ADR, (uint8_t *)&tar_data[FLUID_LEVEL][0], sizeof(uint8_t));
        set_water_level_target_min(tar_data[FLUID_LEVEL][0]);
        tar_data[FLUID_LEVEL][1] = DEF_WAT_LEV_TAR_MAX;
        eeprom_driver_write(WAT_LEV_TAR_MAX_ADR, (uint8_t *)&tar_data[FLUID_LEVEL][1], sizeof(uint8_t));
        set_water_level_target_max(tar_data[FLUID_LEVEL][1]);

        tar_data[SOIL_MOISTURE][0] = DEF_SOI_MOI_TAR_MIN;
        eeprom_driver_write(SOI_MOI_TAR_MIN_ADR, (uint8_t *)&tar_data[SOIL_MOISTURE][0], sizeof(uint8_t));
        set_soil_moisture_target_min(tar_data[SOIL_MOISTURE][0]);
        tar_data[SOIL_MOISTURE][1] = DEF_SOI_MOI_TAR_MAX;
        eeprom_driver_write(SOI_MOI_TAR_MAX_ADR, (uint8_t *)&tar_data[SOIL_MOISTURE][1], sizeof(uint8_t));
        set_soil_moisture_target_max(tar_data[SOIL_MOISTURE][1]);

        tar_data[WATERING_AMOUNT][1] = DEF_WAT_TAR_VOL_MAX;
        eeprom_driver_write(WAT_TAR_VOL_MAX_ADR, (uint8_t *)&tar_data[WATERING_AMOUNT][1], sizeof(uint16_t));
        set_water_volume_calibration_max(tar_data[WATERING_AMOUNT][1]);

        for (uint8_t x = 0; x < 6; x++)
        {
            for (uint8_t y = 0; y < 2; y++)
            {
                PRINTF("%d %d tar data %d \n", x, y, tar_data[x][y]);
            }
        }

        for (uint8_t x = 0; x < 2; x++)
        {
            for (uint8_t y = 0; y < 2; y++)
            {
                PRINTF("%d %d tar data %d \n", x, y, cal_data[x][y]);
            }
        }

#ifdef DEVELOPMENT
        uint16_t xx = 0;
        for (uint8_t i = 0; i < USED_MEM_SIZE; i++)
        {
            uint8_t len = (i == USED_MEM_SIZE - 2) ? 2 : 1;
            eeprom_driver_read(i, (uint8_t *)&xx, len);
            PRINTF("%d ", xx);
            if (i % 2 != 0)
            {
                PRINTF("%s\n", "  ");
            }
        }
#endif
    }
}

cal_tar_data_t get_cal_tar_values(void)
{
    cal_tar_data_t temp;
    temp.temp_cal_min = cal_data[TEMPERATURE][0];
    temp.temp_cal_max = cal_data[TEMPERATURE][1];
    temp.temp_tar_min = tar_data[TEMPERATURE][0];
    temp.temp_tar_max = tar_data[TEMPERATURE][1];
    temp.hum_cal_min = cal_data[HUMIDITY][0];
    temp.hum_cal_max = cal_data[HUMIDITY][1];
    temp.hum_tar_min = tar_data[HUMIDITY][0];
    temp.hum_tar_max = tar_data[HUMIDITY][1];
    temp.light_tar_min = tar_data[LIGHT_INTENSITY][1];
    temp.soi_tar_min = tar_data[SOIL_MOISTURE][0];
    temp.soi_tar_max = tar_data[SOIL_MOISTURE][1];
    temp.wat_lev_tar_min = tar_data[FLUID_LEVEL][0];
    temp.wat_lev_tar_max = tar_data[FLUID_LEVEL][1];
    temp.watering_tar_max = tar_data[WATERING_AMOUNT][1];

    return temp;
}






// static void check_and_update_cal_val_helper(uint8_t (*get)(void), uint8_t (*validate)(void), components_t comp, uint8_t map_index, displays_t cal_or_tar)
// {
//     uint8_t received_value = get();
//     switch (cal_or_tar)
//     {
//     case CAL_SET_WINDOW:
//         if (received_value != cal_data[comp][map_index] &&
//             validate() == YES)
//         {
//             cal_data[comp][map_index] = received_value;
//             eeprom_driver_write(cal_address_map[comp][map_index], (uint8_t *)&received_value, sizeof(received_value));
//         }
//         break;
//     case TAR_SET_WINDOW:
//         if (received_value != tar_data[comp][map_index] &&
//             validate() == YES)
//         {
//             cal_data[comp][map_index] = received_value;
//             eeprom_driver_write(cal_address_map[comp][map_index], (uint8_t *)&received_value, sizeof(received_value));
//         }
//         break;
//     default:
//         break;
//     }
// }

// void chech_and_update_cal_val(void)
// {
//     check_and_update_cal_val_helper(get_temperature_calibration_min, validate_temperature_calibration_min, TEMPERATURE, MIN_VAL_INDEX, CAL_SET_WINDOW);
//     check_and_update_cal_val_helper(get_temperature_calibration_max, validate_temperature_calibration_max, TEMPERATURE, MAX_VAL_INDEX, CAL_SET_WINDOW);
//     check_and_update_cal_val_helper(get_temperature_target_min, validate_temperature_target_min, TEMPERATURE, MIN_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_temperature_target_max, validate_temperature_target_max, TEMPERATURE, MAX_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_humidity_calibration_min, validate_humidity_calibration_min, HUMIDITY, MIN_VAL_INDEX, CAL_SET_WINDOW);
//     check_and_update_cal_val_helper(get_humidity_calibration_max, validate_humidity_calibration_max, HUMIDITY, MAX_VAL_INDEX, CAL_SET_WINDOW);
//     check_and_update_cal_val_helper(get_humidity_target_min, validate_humidity_target_min, HUMIDITY, MIN_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_humidity_target_max, validate_humidity_target_max, HUMIDITY, MAX_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_light_intensity_target_min, validate_light_intensity_target_min, LIGHT_INTENSITY, MAX_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_soil_moisture_target_min, validate_soil_moisture_target_min, SOIL_MOISTURE, MIN_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_soil_moisture_target_max, validate_soil_moisture_target_max, SOIL_MOISTURE, MIN_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_water_level_target_min, validate_water_level_target_min, FLUID_LEVEL, MIN_VAL_INDEX, TAR_SET_WINDOW);
//     check_and_update_cal_val_helper(get_water_level_target_max, validate_water_level_target_max, FLUID_LEVEL, MAX_VAL_INDEX, TAR_SET_WINDOW);
//     uint16_t watring_value = get_water_volume_calibration_max();
//     if (watring_value != tar_data[WATERING_AMOUNT][MAX_VAL_INDEX] &&
//         validate_water_volume_calibration_max() == YES)
//     {
//         tar_data[WATERING_AMOUNT][MAX_VAL_INDEX] = watring_value;
//         eeprom_driver_write(tar_address_map[WATERING_AMOUNT][MAX_VAL_INDEX], (uint8_t *)&watring_value, sizeof(watring_value));
//     }
// }