/*
 * i2cbb.h
 *
 * Wrapper for Linux Kernel I2C Driver (replacing bit-banging)
 */

#include <stdint.h>

// Initialize the I2C bus (open the device)
void i2cbb_init(const char *device_path);

// This executes the SMBus write byte protocol
int32_t i2cbb_write_byte_data(uint8_t i2c_address, uint8_t command,
                              uint8_t value);

// This executes the SMBus read byte protocol
int32_t i2cbb_read_byte_data(uint8_t i2c_address, uint8_t command);

// This executes the SMBus block write protocol
int32_t i2cbb_write_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                   uint8_t length, const uint8_t *values);

// This executes the SMBus block read protocol
int32_t i2cbb_read_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                  uint8_t length, uint8_t *values);

// Reads a raw block from the device (first byte read is used as length by
// convention here, or raw read)
int32_t i2cbb_read_rll(uint8_t i2c_address, uint8_t *values);
