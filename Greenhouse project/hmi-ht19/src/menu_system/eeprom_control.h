#ifndef EEPROM_CONTROL_H
#define EEPROM_CONTROL_H

#include <stdint.h>
#include <managing_entered_value.h>

void config_cal_tar_val(void);

void config_eeprom(bool first_start);
bool validate_input_values(components_t *component, displays_t *cal_or_tar_or_overwrite, char *buffer);
uint8_t update_eeprom(components_t *component, displays_t *cal_or_tar);
cal_tar_data_t get_cal_tar_values(void);

#endif /* EEPROM_CONTROL_H */
