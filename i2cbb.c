/*
 * i2cbb.c
 *
 * Wrapper for Linux Kernel I2C Driver (replacing bit-banging)
 */

#include "i2cbb.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int i2c_fd = -1;

void i2cbb_init(const char *device_path) {
  if (i2c_fd != -1)
    return;

  i2c_fd = open(device_path, O_RDWR);
  if (i2c_fd < 0) {
    perror("Failed to open I2C device");
    // We do not exit here, but subsequent calls will fail.
  }
}

static void set_up_i2c(uint8_t i2c_address) {
  if (i2c_fd < 0)
    return;
  if (ioctl(i2c_fd, I2C_SLAVE, i2c_address) < 0) {
    perror("Failed to acquire bus access and/or talk to slave");
  }
}

int32_t i2cbb_write_byte_data(uint8_t i2c_address, uint8_t command,
                              uint8_t value) {
  if (i2c_fd < 0)
    return -1;
  set_up_i2c(i2c_address);

  if (i2c_smbus_write_byte_data(i2c_fd, command, value) < 0) {
    return -1;
  }
  return 0;
}

int32_t i2cbb_read_byte_data(uint8_t i2c_address, uint8_t command) {
  if (i2c_fd < 0)
    return -1;
  set_up_i2c(i2c_address);

  return i2c_smbus_read_byte_data(i2c_fd, command);
}

int32_t i2cbb_write_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                   uint8_t length, const uint8_t *values) {
  if (i2c_fd < 0)
    return -1;
  set_up_i2c(i2c_address);

  if (i2c_smbus_write_i2c_block_data(i2c_fd, command, length, values) < 0) {
    return -1;
  }
  return 0;
}

int32_t i2cbb_read_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                  uint8_t length, uint8_t *values) {
  if (i2c_fd < 0)
    return -1;
  set_up_i2c(i2c_address);

  // i2c_smbus_read_i2c_block_data returns the number of bytes read
  // It reads up to I2C_SMBUS_BLOCK_MAX bytes (32)
  int res = i2c_smbus_read_i2c_block_data(i2c_fd, command, values);
  if (res < 0)
    return -1;
  return res;
}

// Emulate simple raw read sequence:
// 1. Read a block (header + data)
// 2. Parse length from first byte
int32_t i2cbb_read_rll(uint8_t i2c_address, uint8_t *values) {
  if (i2c_fd < 0)
    return -1;
  set_up_i2c(i2c_address);

  // Read a sufficiently large chunk in one go to keep transaction atomic
  // We assume 64 bytes is enough and the buffer can handle it (or we use a temp
  // buffer) The device implementation we are replacing did: Read Length, Read
  // Length Bytes. So total length = Length + 1.

  uint8_t temp_buf[64];

  // Issue a single read for 64 bytes.
  // If the device has less data, it might send 0xFFs or similar, which we will
  // ignore based on the length byte.
  int bytes_read = read(i2c_fd, temp_buf, sizeof(temp_buf));
  if (bytes_read < 1)
    return -1;

  uint8_t length = temp_buf[0];
  if (length == 0 || length > (bytes_read - 1)) {
    // Valid data must be present. If length claims more data than we read, we
    // might have an issue, or if length is 0. Note: The original code returned
    // 'length' but read 'length' bytes into 'values'.
    // So values[0] is HEAD of data.
    return -1;
  }

  // Copy data to user buffer
  // We assume 'values' points to a buffer large enough.
  // The caller passes 'buff' in sbitx_gtk.c.
  for (int i = 0; i < length; i++) {
    values[i] = temp_buf[i + 1];
  }

  return length;
}
