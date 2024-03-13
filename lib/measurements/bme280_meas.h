#ifndef BME280_MEAS_H
#define BME280_MEAS_H

#include <stdint.h>
#include <avr/delay.h>
#include "i2c_trans.h"
#include "uart_trans.h"

#define MAX_TRIES 3

// Address if SDO pin set to ground
#define BME_ADDRESS 0x76
// Address if SDO pin set to V+
// #define BME_ADDRESS_2 0x77

// Goes until 0xA1 (26 Bytes)
#define COMPENSATE_BEGIN_1 0x88
#define COMPENSATE_REG_1_LEN 26
// Goes until 0xE7 (8 Bytes)
#define COMPENSATE_BEGIN_2 0xE1
#define COMPENSATE_REG_2_LEN 8

#define CONTROL_MEAS 0xF4
#define CONFIG 0xF5
#define RESET_REG 0xE0
#define RESET_VAL 0xB6

#define TEMP_REG 0xFA
#define TEMP_LEN 3



volatile uint16_t dig_T1;
volatile int16_t dig_T2;
volatile int16_t dig_T3;

volatile uint16_t dig_P1;
volatile int16_t dig_P2;
volatile int16_t dig_P3;
volatile int16_t dig_P4;
volatile int16_t dig_P5;
volatile int16_t dig_P6;
volatile int16_t dig_P7;
volatile int16_t dig_P8;
volatile int16_t dig_P9;

volatile uint8_t dig_H1;
volatile int16_t dig_H2;
volatile uint8_t dig_H3;
volatile int16_t dig_H4;
volatile int16_t dig_H5;
volatile int8_t dig_H6;


uint8_t init_sensor(uint8_t os_temp, uint8_t os_hum, uint8_t os_pres);
int32_t get_temp();

#endif // BME280_MEAS_H