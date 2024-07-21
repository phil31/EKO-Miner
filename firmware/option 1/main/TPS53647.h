#ifndef TPS53647_H_
#define TPS53647_H_

//#define TPS53647_I2CADDR         0x71  //< TPS53647 i2c address
#define TPS53647_I2CADDR_DEFAULT 0x71  //< TPS53647 i2c address
#define TPS53647_MANUFACTURER_ID 0xFE  //< Manufacturer ID
#define TPS53647_REVISION        0xFF  //< Chip revision

/*-------------------------*/
/* These are the inital values for the voltage regulator configuration */
/* when the config revision stored in the TPS546 doesn't match, these values are used */


#define TPS53647_INIT_ON_OFF_CONFIG 0x18 /* use ON_OFF command to control power */
#define OPERATION_OFF 0x00
#define OPERATION_ON  0x80

#define TPS53647_INIT_FREQUENCY 650  /* KHz */

/* vin voltage */
#define TPS53647_INIT_VIN_ON  4.8  /* V */
#define TPS53647_INIT_VIN_OFF 4.5  /* V */
#define TPS53647_INIT_VIN_UV_WARN_LIMIT 5.8 /* V */
#define TPS53647_INIT_VIN_OV_FAULT_LIMIT 6.0 /* V */
#define TPS53647_INIT_VIN_OV_FAULT_RESPONSE 0xB7  /* retry 6 times */

  /* vout voltage */
#define TPS53647_INIT_SCALE_LOOP 0.25  /* Voltage Scale factor */
#define TPS53647_INIT_VOUT_MAX 3 /* V */
#define TPS53647_INIT_VOUT_OV_FAULT_LIMIT 1.25 /* %/100 above VOUT_COMMAND */
#define TPS53647_INIT_VOUT_OV_WARN_LIMIT  1.1 /* %/100 above VOUT_COMMAND */
#define TPS53647_INIT_VOUT_MARGIN_HIGH 1.1 /* %/100 above VOUT */
#define TPS53647_INIT_VOUT_COMMAND 1.2  /* V absolute value */
#define TPS53647_INIT_VOUT_MARGIN_LOW 0.90 /* %/100 below VOUT */
#define TPS53647_INIT_VOUT_UV_WARN_LIMIT 0.90  /* %/100 below VOUT_COMMAND */
#define TPS53647_INIT_VOUT_UV_FAULT_LIMIT 0.75 /* %/100 below VOUT_COMMAND */
#define TPS53647_INIT_VOUT_MIN 1 /* v */

  /* iout current */
#define TPS53647_INIT_IOUT_OC_WARN_LIMIT  25.00 /* A */
#define TPS53647_INIT_IOUT_OC_FAULT_LIMIT 30.00 /* A */
#define TPS53647_INIT_IOUT_OC_FAULT_RESPONSE 0xC0  /* shut down, no retries */

  /* temperature */
// It is better to set the temperature warn limit for TPS546 more higher than Ultra 
#define TPS53647_INIT_OT_WARN_LIMIT  105 /* degrees C */
#define TPS53647_INIT_OT_FAULT_LIMIT 145 /* degrees C */
#define TPS53647_INIT_OT_FAULT_RESPONSE 0xFF /* wait for cooling, and retry */

  /* timing */
#define TPS53647_INIT_TON_DELAY 0
#define TPS53647_INIT_TON_RISE 3
#define TPS53647_INIT_TON_MAX_FAULT_LIMIT 0
#define TPS53647_INIT_TON_MAX_FAULT_RESPONSE 0x3B
#define TPS53647_INIT_TOFF_DELAY 0
#define TPS53647_INIT_TOFF_FALL 0

#define INIT_STACK_CONFIG 0x0000
#define INIT_SYNC_CONFIG 0x0010
#define INIT_PIN_DETECT_OVERRIDE 0x0000

/*-------------------------*/

/* PMBUS_ON_OFF_CONFIG initialization values */
#define ON_OFF_CONFIG_PU 0x10   // turn on PU bit
#define ON_OFF_CONFIG_CMD 0x08  // turn on CMD bit
#define ON_OFF_CONFIG_CP 0x00   // turn off CP bit
#define ON_OFF_CONFIG_POLARITY 0x00 // turn off POLARITY bit
#define ON_OFF_CONFIG_DELAY 0x00 // turn off DELAY bit


/* public functions */
int   TPS53647_init(void);
void  TPS53647_read_mfr_info(uint8_t *);
void  TPS53647_set_mfr_info(void);
void  TPS53647_write_entire_config(void);
int   TPS53647_get_frequency(void);
void  TPS53647_set_frequency(int);
int   TPS53647_get_temperature(void);
float TPS53647_get_vin(void);
float TPS53647_get_iout(void);
float TPS53647_get_vout(void);
void  TPS53647_set_vout(float volts);
void  TPS53647_show_voltage_settings(void);

#endif /* TPS53647_H_ */
