#include <util/delay.h>
#include <stdio.h>
#include "ls_macros.h"
#include "i2c_trans.h"
#include "uart_trans.h"

#define PORTB DEREF_8BIT(0x25)
#define DDRB DEREF_8BIT(0x24)
#define BME280_ADDRESS 0x76 // Or 0x77
#define TEMP_CONST_SIZE 6

void db_blnk(void)
{
    for(volatile int i = 0; i < 6; ++i)
    {
        PORTB ^= (1 << 4);
        _delay_ms(30);
    }
}

int main()
{
    // Max Speed for BME280 Sensor is 3.4 MHz
    // Arduino runs at 16 MHz
    // For I2C Clock of 250 kHz speed,
    // set value of 24 without prescaling.
    init_i2c(24, 0);
    init_uart_transmission((uint16_t) 9600);
    // Temperature compensation constant 1 to 3
    uint16_t dig_T1 = 0;
    int16_t dig_T2 = 0;
    int16_t dig_T3 = 0;
    DDRB = (1 << 4);

    // Storage for data on sensor
    uint8_t temperature_bytes[TEMP_CONST_SIZE] = 
        {0, 0, 0, 0, 0, 0};

    int state = master_transmit_byte(BME280_ADDRESS, 0x88);
    if (state == 0)
    {
        db_blnk();
        unsigned char chr = 'A';
        send_char(chr);
    }
    state = master_receive_nbytes(BME280_ADDRESS, 
        temperature_bytes, TEMP_CONST_SIZE);
    _delay_ms(500);
    if (state == 0)
    {
        send_string("Hello World!");
        db_blnk();
    }
    return 0;
}

