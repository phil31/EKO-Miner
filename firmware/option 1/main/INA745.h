#ifndef INA745_H_
  #define INA745_H_

    #include "i2c_master.h"

    bool  INA745_installed(void);
    float INA745_read_current(void);
    float INA745_read_voltage(void);
    float INA745_read_power(void);
    float INA745_read_dietemp(void);

    // INA745                               https://www.ti.com/lit/ds/symlink/ina745a.pdf?ts=1719074595759&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FINA745A%253Fbm-verify%253DAAQAAAAJ_____4f6xSWSNdpYjutN_uMG-gVHJn7r7lYa9C3p7cgfobV1ET7keZ0TO15nqWm0FlBb571BGah2RdndKHJ_ETPob8kgI8mqSfBqD-jaPDXKiFqk2q90UmV3oYb2IMlu9fQTa0pC-hhfXipAAwqkO29tECXWil5JkYYFFtySnyh-ncp2pEoBrOWVS73ndrCVGuu4ULTQUKVdkGW8TwLzOPiq_EPQxHP8BRH8gkj_Z-6QioZt-cO3VmvuTNMXquJEo7f5IWdG2zTLBgOuHRdWJvUKn3iORdTocdY7rLZFFh0RlPMeof8yevolcO0
    //     WR       0x80    0x82    0x88
    //     RD       0x81    0x83    0x89

    #define INA745_I2CADDR_DEFAULT 0x80 ///< INA75 default i2c address
    
    #define INA745_REG_CONFIG      0x00 ///< Configuration register
    #define INA745_REG_BUSVOLTAGE  0x05 ///< Bus voltage measurement register in mV
    #define INA745_REG_CURRENT     0x07 ///< Current measurement register (signed) in mA
    #define INA745_REG_POWER       0x08 ///< Power calculation register in mW
    #define INA745_REG_DIE_TEMP    0x06 ///< die temperature 12 bits signed 125m°/lsb
    #define INA745_REG_ENERGY      0x09 ///< 
//        #define INA745_REG_MASK_ENABLE 0x06 ///< Interrupt/Alert setting and checking register
    #define INA745_REG_DIAG_ALERT  0x0B ///< Alert limit value register

    #define INA745_REG_COL         0x0C ///< Alert limit value register
    #define INA745_REG_CUL         0x0D ///< Alert limit value register
    #define INA745_REG_BOVL        0x0E ///< Alert limit value register
    #define INA745_REG_BUVL        0x0F ///< Alert limit value register
    #define INA745_REG_TEMP_LIM    0x10 ///< Alert limit value register
    #define INA745_REG_PWR_LIM     0x11 ///< Alert limit value register

    #define INA745_REG_MFG_UID     0x3E     ///< Manufacturer ID Register  Reads back TI in ASCII (5449h).

    /**
     * @brief Mode options.
     *
     * Allowed values for setMode.
     */
    typedef enum _mode745
    {
      INA745_MODE_SHUTDOWN      = 0x00, /**< SHUTDOWN: Minimize quiescient current and turn off current into the device inputs. Set another mode to exit shutown mode **/
      INA745_MODE_TRIGGERED     = 0x07, /**< TRIGGERED: Trigger a one-shot measurement of current and bus voltage. Set the TRIGGERED mode again to take a new measurement **/
      INA745_MODE_CONTINUOUS    = 0x0F, /**< CONTINUOUS: (Default) Continuously update the current, bus voltage and power registers with new measurements **/
    } INA745_MeasurementMode;

    /**
     * @brief Conversion Time options.
     *
     * Allowed values for setCurrentConversionTime and setVoltageConversionTime.
     */
    typedef enum _conversion_time745
    {
      INA745_TIME_50_us,    ///< Measurement time: 50us
      INA745_TIME_84_us,    ///< Measurement time: 84us
      INA745_TIME_150_us,   ///< Measurement time: 150us
      INA745_TIME_280_us,   ///< Measurement time: 280us
      INA745_TIME_540_us,   ///< Measurement time: 540us
      INA745_TIME_1052_us,  ///< Measurement time: 1.052ms (Default)
      INA745_TIME_2074_us,  ///< Measurement time: 2.074ms
      INA745_TIME_4120_us,  ///< Measurement time: 4.120ms
    } INA745_ConversionTime;

    /**
     * @brief Averaging Count options.
     *
     * Allowed values forsetAveragingCount.
     */
    typedef enum _count745
    {
      INA745_COUNT_1,    ///< Window size: 1 sample (Default)
      INA745_COUNT_4,    ///< Window size: 4 samples
      INA745_COUNT_16,   ///< Window size: 16 samples
      INA745_COUNT_64,   ///< Window size: 64 samples
      INA745_COUNT_128,  ///< Window size: 128 samples
      INA745_COUNT_256,  ///< Window size: 256 samples
      INA745_COUNT_512,  ///< Window size: 512 samples
      INA745_COUNT_1024, ///< Window size: 1024 samples
    } INA745_AveragingCount;

    /**
     * @brief Alert trigger options.
     *
     * Allowed values for setAlertType.
     */
    typedef enum _alert_type745
    {
      INA745_ALERT_CONVERSION_READY = 0x1, ///< Trigger on conversion ready
      INA745_ALERT_OVERPOWER = 0x6,        ///< Trigger on power over limit
      INA745_ALERT_UNDERVOLTAGE = 0x8,     ///< Trigger on bus voltage under limit
      INA745_ALERT_OVERVOLTAGE = 0x10,     ///< Trigger on bus voltage over limit
      INA745_ALERT_UNDERCURRENT = 0x20,    ///< Trigger on current under limit
      INA745_ALERT_OVERCURRENT = 0x40,     ///< Trigger on current over limit
      INA745_ALERT_NONE = 0x0,             ///< Do not trigger alert pin (Default)
    } INA745_AlertType;

    /**
     * @brief Alert pin polarity options.
     *
     * Allowed values for setAlertPolarity.
     */
    typedef enum _alert_polarity745
    {
      INA745_ALERT_POLARITY_NORMAL = 0x0,   ///< Active high open-collector (Default)
      INA745_ALERT_POLARITY_INVERTED = 0x1, ///< Active low open-collector
    } INA745_AlertPolarity;

    /**
     * @brief Alert pin latch options.
     *
     * Allowed values for setAlertLatch.
     */
    typedef enum _alert_latch745
    {
      INA745_ALERT_LATCH_ENABLED = 0x1,     /**< Alert will latch until Mask/Enable register is read **/
      INA745_ALERT_LATCH_TRANSPARENT = 0x0, /**< Alert will reset when fault is cleared **/
    } INA745_AlertLatch;
        

#endif
/* INA745_H_ */
