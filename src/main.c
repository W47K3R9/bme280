#include <util/delay.h>
#include <stdio.h>
#include "ls_macros.h"
#include "i2c_trans.h"
#include "uart_trans.h"
#include "bme280_meas.h"

#define PORTB DEREF_8BIT(0x25)
#define DDRB DEREF_8BIT(0x24)
#define BME280_CHIP_ID 0xD0

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
    int state = init_sensor(1, 0, 0);
    send_string("\nState of initialization: ");
    send_signed_decimal(state);
    while (1)
    {
        _delay_ms(500);
        int32_t temperature = get_temp();
        float fine_res = temperature / 100.0;
        send_string("\nMeasuring Temp: ");
        send_float(fine_res, 2);
        send_string(" °C\n");
    }
    



    state = master_transmit_byte(0x76, BME280_CHIP_ID);
    send_string("\nReceiving chip ID state: ");
    send_unsigned_decimal(state);
    uint8_t chip_id = 0;
    _delay_ms(1);
    state = master_receive_byte(0x76, &chip_id);
    send_string("\nChip ID is: ");
    send_unsigned_decimal(chip_id);
    
    return 0;
}

