#include "bme280_meas.h"

uint8_t init_sensor(uint8_t os_temp, uint8_t os_hum, uint8_t os_pres)
{
    // Read out the temperature data
    int cnt = 0;
    uint8_t state = master_transmit_byte(BME_ADDRESS, COMPENSATE_BEGIN_1);
    while (state != 0 && cnt <= MAX_TRIES)
    {
        state = master_transmit_byte(BME_ADDRESS, COMPENSATE_BEGIN_1);
        cnt++;
        send_unsigned_decimal(state);
    }
    cnt = 0;
    uint8_t compensation_array_1[COMPENSATE_REG_1_LEN] = {};
    state = master_receive_nbytes(BME_ADDRESS, compensation_array_1,
        COMPENSATE_REG_1_LEN);
    while (state != 0 && cnt <= MAX_TRIES)
    {
        state =  master_receive_nbytes(BME_ADDRESS, compensation_array_1,
            COMPENSATE_REG_1_LEN);
        cnt++;
        send_unsigned_decimal(state);
    }
    cnt = 0;
    // First Register Bits 7-0, Second Register Bits 15-8.
    // Temperature
    dig_T1 = compensation_array_1[0] | ((uint16_t)compensation_array_1[1] << 8);
    dig_T2 = compensation_array_1[2] | ((uint16_t)compensation_array_1[3] << 8);
    dig_T3 = compensation_array_1[4] | ((uint16_t)compensation_array_1[5] << 8);

    // Pressure
    dig_P1 = compensation_array_1[7] << 8;
    dig_P1 += compensation_array_1[6];
    dig_P2 = compensation_array_1[9] << 8;
    dig_P2 += compensation_array_1[8];
    dig_P3 = compensation_array_1[11] << 8;
    dig_P3 += compensation_array_1[10] & 0xFF;
    dig_P4 = compensation_array_1[13] << 8;
    dig_P4 += compensation_array_1[12] & 0xFF;
    dig_P5 = compensation_array_1[15] << 8;
    dig_P5 += compensation_array_1[14] & 0xFF;
    dig_P6 = compensation_array_1[17] << 8;
    dig_P6 += compensation_array_1[16] & 0xFF;
    dig_P7 = compensation_array_1[19] << 8;
    dig_P7 += compensation_array_1[18] & 0xFF;
    dig_P8 = compensation_array_1[21] << 8;
    dig_P8 += compensation_array_1[20] & 0xFF;
    dig_P9 = compensation_array_1[23] << 8;
    dig_P9 += compensation_array_1[22] & 0xFF;

    // Humidity
    dig_H1 = compensation_array_1[25] & 0xFF;
    state = master_transmit_byte(BME_ADDRESS, COMPENSATE_BEGIN_2);
    uint8_t compensation_array_2[COMPENSATE_REG_2_LEN] = {};
    state = master_receive_nbytes(BME_ADDRESS, compensation_array_2,
        COMPENSATE_REG_2_LEN);
    while (state != 0 && cnt <= MAX_TRIES)
    {
        state =  master_receive_nbytes(BME_ADDRESS, compensation_array_2,
            COMPENSATE_REG_2_LEN);
        cnt++;
        send_unsigned_decimal(state);
    }
    _delay_ms(1);
    state = master_transmit_2bytes(BME_ADDRESS, RESET_REG, RESET_VAL);
    _delay_ms(1);
    state = master_transmit_2bytes(BME_ADDRESS, CONFIG, (uint8_t)0);
    return state;
}


int32_t get_temp()
{
    int32_t adc_temp = 0;
    uint8_t temp_bytes[TEMP_LEN] = {};
    uint8_t state = master_transmit_2bytes(BME_ADDRESS, CONTROL_MEAS, 0b11100010);
    _delay_ms(50);
    state = master_transmit_byte(BME_ADDRESS, TEMP_REG);
    _delay_ms(1);
    state = master_receive_nbytes(BME_ADDRESS, temp_bytes, TEMP_LEN);
    adc_temp = ((int32_t)temp_bytes[0] << 12) | ((int32_t)temp_bytes[1] << 4) | ((int32_t)temp_bytes[2] >> 4);

    int32_t t_fine;
    int32_t var1;
    int32_t var2;
    int32_t temperature;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    // Takes forever...
    // var1 = (int32_t)((adc_temp/ 8) - ((int32_t)dig_T1 * 2));
    // var1 = (var1 * ((int32_t)dig_T2)) / 2048;
    // var2 = (int32_t)((temperature / 16) - ((int32_t)dig_T1));
    // var2 = (((var2 * var2) / 4096) * ((int32_t)dig_T3)) / 16384;
    // t_fine = var1 + var2;
    // temperature = (t_fine * 5 + 128) / 256;


    var1 = ((((adc_temp >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11; 
    var2 = (((((adc_temp >> 4) - ((int32_t)dig_T1)) * ((adc_temp >> 4) - ((int32_t)dig_T1))) >> 12) * 
        ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    temperature =(t_fine * 5 + 128) >> 8;

    if (temperature < temperature_min)
    {
        temperature = temperature_min;
    }
    else if (temperature > temperature_max)
    {
        temperature = temperature_max;
    }

    return temperature;
}