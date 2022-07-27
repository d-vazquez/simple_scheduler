
#ifndef _I2C_TIVA_H_
#define _I2C_TIVA_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef struct
{
    uint32_t count;
    uint8_t preamble;
    union {
        uint8_t payload[32];
        uint8_t *payload_ptr;
    };
    uint8_t data_is_ptr;
} i2c_burst;


void InitI2C0(void);
void I2CSend(uint8_t slave_addr, uint8_t count, uint8_t *payload);
void I2CSend_withPreamble(uint8_t slave_addr, i2c_burst *burst);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);


#endif  // _I2C_TIVA_H_
