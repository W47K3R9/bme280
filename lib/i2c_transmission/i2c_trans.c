#include "i2c_trans.h"

int init_i2c(uint8_t speed, uint8_t prescale)
{
    if (prescale > 3)
    {
        return -1;
    }
    // Analog In 4 and 5 are SDA and SCL Out.
    // Pullup Resistor is needed.
    DDRC = 0;
    PORTC = (1 << PULLUP4) | (1 << PULLUP5);
    // Clear Prescaler Bits.
    I2C_STATUS &= ~(0b11);
    // Set speed bits to 24.
    I2C_SPEED = speed;
    return 0;
}

int master_transmit_byte(uint8_t address, uint8_t data)
{
    // Prepare for Transmission and set start bit.
    I2C_CONTROL = (1 << ENABLE) | (1 << START) | (1 << INTERRUPT);
    // In case that the bus is busy, 
    // wait until INTERRUPT is set to 1.
    while(!(I2C_CONTROL & (1 << INTERRUPT)));
    // Check correct status.
    if ((I2C_STATUS & ~(0b11)) != COND_START)
    {
        // Error: Start failed!
        return 1;
    }

    // Determine Address along with write bit (0 for last bit).
    I2C_DATA = (address << 1) & ~(0b1);
    // Transmit address with write command.
    I2C_CONTROL = (1 << ENABLE) | (1 << INTERRUPT);
    while(!(I2C_CONTROL & (1 << INTERRUPT)));
    if ((I2C_STATUS & ~(0b11)) != COND_ADDR_WRITE_ACK)
    {
        // Error: No acknowledgement for address!
        return 2;
    }

    // Now the byte of data can be sent.
    I2C_DATA = data;
    // Transmit data.
    I2C_CONTROL = (1 << ENABLE) | (1 << INTERRUPT);
    while(!(I2C_CONTROL & (1 << INTERRUPT)));
    if ((I2C_STATUS & ~(0b11)) != COND_DATA_WRITE_ACK)
    {
        // Error: No acknowledgement for data!
        return 3;
    }

    // Stop Transfer!
    I2C_CONTROL = (1 << ENABLE) | (1 << STOP) | (1 << INTERRUPT);
    return 0;
}

int master_receive_nbytes(uint8_t address, uint8_t* storage, uint8_t num_bytes)
{
    // Prepare to receive and set Start Bit.
    I2C_CONTROL = (1 << ENABLE) | (1 << START) | (1 << INTERRUPT);
    // In case that the bus is busy, 
    // wait until INTERRUPT is set to 1.
    while(!(I2C_CONTROL & (1 << INTERRUPT)));

    // Check correct status.
    if ((I2C_STATUS & ~(0b11)) != COND_START)
    {
        // Error: Start failed!
        return 1;
    }

    // Determine Address along with read bit (1 for last bit).
    I2C_DATA = (address << 1) | 0b1;
    // Transmit address with write command.
    I2C_CONTROL = (1 << ENABLE) | (1 << INTERRUPT);
    while(!(I2C_CONTROL & (1 << INTERRUPT)));
    if ((I2C_STATUS & ~(0b11)) != COND_ADDR_READ_ACK)
    {
        // Error: No acknowledgement for address!
        return 2;
    }
    
    // Now the reading process begins.
    for (int i = 0; i < num_bytes - 2; ++i)
    {
        // Enable the first data package.
        I2C_CONTROL = (1 << ENABLE) | (1 << INTERRUPT) | (1 << ACK_MST);
        while(!(I2C_CONTROL & (1 << INTERRUPT)));
        if ((I2C_STATUS & ~(0b11)) != COND_DATA_READ_ACK)
        {
            // Error: Master ack not received!
            return 3;
        }
        *(storage + i) = I2C_DATA;
    }
    // Enable the last data package and deactivate acknowledge.
    I2C_CONTROL = (1 << ENABLE) | (1 << INTERRUPT);
    while(!(I2C_CONTROL & (1 << INTERRUPT)));
    if ((I2C_STATUS & ~(0b11)) != COND_DATA_READ_NACK)
    {
        // Error: Master nack not received!
        return 4;
    }
    *(storage + (num_bytes - 1)) = I2C_DATA;

    // Stop reading!
    I2C_CONTROL = (1 << ENABLE) | (1 << STOP) | (1 << INTERRUPT);
    return 0;
}