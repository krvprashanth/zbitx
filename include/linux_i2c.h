#ifndef LINUX_I2C_H
#define LINUX_I2C_H

#include <stdint.h>

int32_t sbitx_i2c_write_byte_data(uint8_t i2c_address, uint8_t command,
                              uint8_t value);
int32_t sbitx_i2c_read_byte_data(uint8_t i2c_address, uint8_t command);
int32_t sbitx_i2c_write_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                   uint8_t length, const uint8_t *values);
int32_t sbitx_i2c_read_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                  uint8_t length, uint8_t *values);
int32_t sbitx_i2c_read_rll(uint8_t i2c_address, uint8_t *values);

#endif
