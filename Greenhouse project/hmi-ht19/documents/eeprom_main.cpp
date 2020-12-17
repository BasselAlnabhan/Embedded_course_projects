#include <bsp.h>
#include <eeprom_driver.h>

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
    }

    uint8_t address = 0;
    eeprom_driver_init(true);

    uint32_t data = 0x12345678U;
    eeprom_driver_write(address, (uint8_t *)&data, sizeof(data));
    address += sizeof(data);

    data = 0x87654321U;
    eeprom_driver_write(address, (uint8_t *)&data, sizeof(data));
    address += sizeof(data);

    data = 0;
    eeprom_driver_read(0, (uint8_t *)&data, sizeof(data));
    Serial.println(data, HEX);

    data = 0;
    eeprom_driver_read(sizeof(data), (uint8_t *)&data, sizeof(data));
    Serial.println(data, HEX);

    float value = 23.56f;
    eeprom_driver_write(address, (uint8_t *)&value, sizeof(value));

    value = 0.0f;
    eeprom_driver_read(address, (uint8_t *)&value, sizeof(value));
    Serial.println(value);
}

void loop()
{
}