
#ifndef _MAX6615_H_
    #define _MAX6615_H_


    #include "i2c_master.h" 

    // MAX6615                              https://www.analog.com/media/en/technical-documentation/data-sheets/MAX6615-MAX6616.pdf
    //     WR       0x30    0x34    0x98
    //     RD       0x31    0x35    0x99

        #define MAX6615_I2CADDR_DEFAULT             0x4C             ///< The default I2C address


        #define MAX6615_TEMP_CH_1                   0x00
        #define MAX6615_TEMP_CH_2                   0x01
        #define MAX6615_CONF_BYTE                   0x02   // MAX6615_REG_CONFIG 
        #define MAX6615_TEMP_CH_1_OT_LIMIT          0x03
        #define MAX6615_TEMP_CH_2_OT_LIMIT          0x04
        #define MAX6615_OT_STATUS                   0x05
        #define MAX6615_OT_MASK                     0x06
        #define MAX6615_PWM_1_START_DC              0x07
        #define MAX6615_PWM_2_START_DC              0x08
        #define MAX6615_PWM_1_MAX_DC                0x09
        #define MAX6615_PWM_2_MAX_DC                0x0A
        #define MAX6615_PWM_1_TARGET_DC             0x0B
        #define MAX6615_PWM_2_TARGET_DC             0x0C
        #define MAX6615_PWM_1_INSTA_DC              0x0D
        #define MAX6615_PWM_2_INSTA_DC              0x0E
        #define MAX6615_CH_1_FAN_START_TEMP         0x0F
        #define MAX6615_CH_2_FAN_START_TEMP         0x10
        #define MAX6615_FAN_CONF                    0x11   //MAX6615_FAN_CONFIG
        #define MAX6615_DUTY_CYCLE_RATE_CHANGE      0x12
        #define MAX6615_DUTY_CYCLE_STEP_SIZE        0x13
        #define MAX6615_PWM_FREQ_SEL                0x14
        #define MAX6615_GPIO_FUNCTION               0x15
        #define MAX6615_GPIO_VALUE                  0x16
        #define MAX6615_THERMISTOR_OFFSET           0x17
        #define MAX6615_TACH_1_VAL                  0x18
        #define MAX6615_TACH_2_VAL                  0x19
        #define MAX6615_TACH_1_LIMIT                0x1A
        #define MAX6615_TACH_2_LIMIT                0x1B
        #define MAX6615_FAN_STATUS                  0x1C
        #define MAX6615_TEMP_CH_1_LSB               0x1E
        #define MAX6615_TEMP_CH_2_LSB               0x1F
        #define MAX6615_DEVICE_REV                  0xFD
        #define MAX6615_DEVICE_ID                   0xFE
        #define MAX6615_MANUFACTURER_ID             0xFF

        #define MAX6615_TEMP_FAULT_OPEN_CIRCUIT    0x3F8   // TBC
        #define MAX6615_TEMP_FAULT_SHORT           0x3FF   // TBC
        

        void     MAX6615_init(bool);
        void     MAX6615_set_fan_speed(float);
        uint16_t MAX6615_get_fan_speed(void);
        float    MAX6615_get_external_temp(void);
        uint8_t  MAX6615_get_internal_temp(void);

        //uint16_t MAX6615_get_IG(void);




        // // MAX6615 new_MAX6615                 (void);
        // uint8_t  MAX6615_Write8( uint8_t reg, uint8_t val);
        // uint8_t  MAX6615_Read8( uint8_t reg, uint8_t* val);

        // uint8_t  MAX6615_PWM_EnableAutomatic(uint8_t channel, uint8_t fanStartDC, uint8_t fanStartTemp);
        // uint8_t  MAX6615_PWM_EnableManual(uint8_t channel);
        // uint8_t  MAX6615_ReadTemperature(uint8_t channel, float* temp);
        // uint8_t  MAX6615_PWM_SetPWM( uint8_t channel, uint8_t percent);
        // uint8_t  MAX6615_SetTempOffset( uint8_t channel, int8_t degrees);



#endif