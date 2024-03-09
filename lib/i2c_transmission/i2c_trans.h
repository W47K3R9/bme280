#ifndef I2C_TRANS_H
#define I2C_TRANS_H

#include <stdint.h>
#include "i2c_trans.h"
#include "ls_macros.h"

#define I2C_SPEED DEREF_8BIT(0xB8)
#define I2C_CONTROL DEREF_8BIT(0xBC)
#define I2C_STATUS DEREF_8BIT(0xB9)
#define I2C_DATA DEREF_8BIT(0xBB)
#define I2C_SLAVE_ADDRESS DEREF_8BIT(0xBA)
#define DDRC DEREF_8BIT(0x27)
#define PORTC DEREF_8BIT(0x28)
#define PULLUP4 4
#define PULLUP5 5
#define INTERRUPT 7
#define START 5
#define STOP 4
#define ENABLE 2
#define ACK_MST 6
#define COND_START 0x08
#define COND_ADDR_WRITE_ACK 0x18
#define COND_ADDR_READ_ACK 0x40
#define COND_DATA_WRITE_ACK 0x28
#define COND_DATA_READ_ACK 0x50
#define COND_DATA_READ_NACK 0x58


int init_i2c(uint8_t speed, uint8_t prescale);
int master_transmit_byte(uint8_t address, uint8_t data);
int master_receive_nbytes(uint8_t address, uint8_t* storage, 
    uint8_t num_bytes);

#endif // I2C_TRANS_H