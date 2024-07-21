

// from https://github.com/eta-systems/MAX6615
// https://github.com/eta-systems/MAX6615/blob/master/src/max6615.c
//
//


#include "esp_log.h"
#include <stdio.h>

#include "MAX6615.h"

static const char * TAG = "MAX6615.c";

// run this first. sets up the config register
void MAX6615_init(bool invertPolarity)
{
    // set the TACH input
    ESP_ERROR_CHECK(i2c_master_register_write_byte(MAX6615_I2CADDR_DEFAULT, MAX6615_CONF_BYTE, 0x04));

    if (invertPolarity) {
        ESP_ERROR_CHECK(i2c_master_register_write_byte(MAX6615_I2CADDR_DEFAULT, MAX6615_FAN_CONF, 0b00100011));
    }
}

// takes a fan speed percent
void MAX6615_set_fan_speed(float percent)
{
    uint8_t speed;

    speed = (uint8_t) (63.0 * percent);
    ESP_LOGI(TAG, "*** TODO *** Finalyse MAX6615_set_fan_speed()");
    // ESP_ERROR_CHECK(i2c_master_register_write_byte(MAX6615_I2CADDR_DEFAULT, MAX6615_REG_FAN_SETTING, speed));
}

// RPM = 5400000/reading
uint16_t MAX6615_get_fan_speed(void)
{
    uint8_t tach_lsb, tach_msb;


    ESP_LOGI(TAG, "*** TODO *** Finalyse MAX6615_get_fan_speed()");
    // ESP_ERROR_CHECK(i2c_master_register_read(MAX6615_I2CADDR_DEFAULT, MAX6615_TACH_LSB, &tach_lsb, 1));
    // ESP_ERROR_CHECK(i2c_master_register_read(MAX6615_I2CADDR_DEFAULT, MAX6615_TACH_MSB, &tach_msb, 1));
    ESP_LOGI(TAG, "Raw Fan Speed = %02X %02X", tach_msb, tach_lsb);

    uint16_t reading = tach_lsb | (tach_msb << 8);
    uint16_t RPM = 5400000 / reading;

    ESP_LOGI(TAG, "Fan Speed = %d RPM", RPM);
    if (RPM == 82) {
        return 0;
    }
    return RPM;
}

float MAX6615_get_external_temp(void)
{
    uint8_t temp_msb, temp_lsb;
    

    ESP_LOGI(TAG, "*** TODO *** Finalyse MAX6615_get_external_temp()");
    // ESP_ERROR_CHECK(i2c_master_register_read(MAX6615_I2CADDR_DEFAULT, MAX6615_EXTERNAL_TEMP_MSB, &temp_msb, 1));
    // ESP_ERROR_CHECK(i2c_master_register_read(MAX6615_I2CADDR_DEFAULT, MAX6615_EXTERNAL_TEMP_LSB, &temp_lsb, 1));
    
    uint16_t reading = temp_lsb | (temp_msb << 8);
    reading >>= 5;

    if (reading == MAX6615_TEMP_FAULT_OPEN_CIRCUIT) {
        ESP_LOGE(TAG, "MAX6615 TEMP_FAULT_OPEN_CIRCUIT");
    }
    if (reading == MAX6615_TEMP_FAULT_SHORT) {
        ESP_LOGE(TAG, "MAX6615 TEMP_FAULT_SHORT");
    }

    float result = (float) reading / 8.0;
    return result;
}

uint8_t MAX6615_get_internal_temp(void)
{
    uint8_t temp = 0;
    ESP_LOGI(TAG, "*** TODO *** Finalyse MAX6615_get_internal_temp()");
    // ESP_ERROR_CHECK(i2c_master_register_read(MAX6615_I2CADDR_DEFAULT, MAX6615_INTERNAL_TEMP, &temp, 1));
    return temp;
}


