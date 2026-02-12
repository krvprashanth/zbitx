#include "linux_i2c.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define I2C_BUS "/dev/i2c-3"

static int i2c_open_bus(uint8_t i2c_address) {
  int file = open(I2C_BUS, O_RDWR);
  if (file < 0) {
    return -1;
  }
  if (ioctl(file, I2C_SLAVE, i2c_address) < 0) {
    close(file);
    return -1;
  }
  return file;
}

// Internal SMBus access helper for single byte commands
static inline int32_t i2c_smbus_access(int file, char read_write,
                                       uint8_t command, int size,
                                       union i2c_smbus_data *data) {
  struct i2c_smbus_ioctl_data args;
  args.read_write = read_write;
  args.command = command;
  args.size = size;
  args.data = data;
  return ioctl(file, I2C_SMBUS, &args);
}

int32_t sbitx_i2c_write_byte_data(uint8_t i2c_address, uint8_t command,
                                  uint8_t value) {
  int file = i2c_open_bus(i2c_address);
  if (file < 0)
    return -1;

  union i2c_smbus_data data;
  data.byte = value;
  int32_t result = i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
                                    I2C_SMBUS_BYTE_DATA, &data);

  close(file);
  return result;
}

int32_t sbitx_i2c_read_byte_data(uint8_t i2c_address, uint8_t command) {
  int file = i2c_open_bus(i2c_address);
  if (file < 0)
    return -1;

  union i2c_smbus_data data;
  int32_t result;
  if (i2c_smbus_access(file, I2C_SMBUS_READ, command, I2C_SMBUS_BYTE_DATA,
                       &data) < 0) {
    result = -1;
  } else {
    result = data.byte;
  }

  close(file);
  return result;
}

/*
 * Use plain write() to support blocks larger than 32 bytes (OLED uses 128
 * bytes). This sends [command, data[0], data[1], ...] which matches the
 * original sbitx_i2c behavior.
 */
int32_t sbitx_i2c_write_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                       uint8_t length, const uint8_t *values) {
  int file = i2c_open_bus(i2c_address);
  if (file < 0)
    return -1;

  uint8_t *buf = malloc(length + 1);
  if (!buf) {
    close(file);
    return -1;
  }

  buf[0] = command;
  memcpy(buf + 1, values, length);

  int32_t result;
  if (write(file, buf, length + 1) == length + 1) {
    result = 0;
  } else {
    result = -1;
  }

  free(buf);
  close(file);
  return result;
}

/*
 * Use I2C_RDWR for block reads to ensure a proper combined transaction:
 * START, ADDR+WR, command, REPEATED_START, ADDR+RD, Data..., STOP.
 */
int32_t sbitx_i2c_read_i2c_block_data(uint8_t i2c_address, uint8_t command,
                                      uint8_t length, uint8_t *values) {
  int file = i2c_open_bus(i2c_address);
  if (file < 0)
    return -1;

  struct i2c_msg msgs[2];
  struct i2c_rdwr_ioctl_data rdwr;

  // First message: Write the register/command byte
  msgs[0].addr = i2c_address;
  msgs[0].flags = 0; // Write
  msgs[0].len = 1;
  msgs[0].buf = &command;

  // Second message: Read the data
  msgs[1].addr = i2c_address;
  msgs[1].flags = I2C_M_RD; // Read
  msgs[1].len = length;
  msgs[1].buf = values;

  rdwr.msgs = msgs;
  rdwr.nmsgs = 2;

  int32_t result;
  if (ioctl(file, I2C_RDWR, &rdwr) < 0) {
    result = -1;
  } else {
    result = length;
  }

  close(file);
  return result;
}

/*
 * Custom read function for the ZBitx front panel
 * Uses standard read() for bulk data transfer.
 */
int32_t sbitx_i2c_read_rll(uint8_t i2c_address, uint8_t *values) {
  int file = i2c_open_bus(i2c_address);
  if (file < 0)
    return -1;

  int32_t bytes_read = read(file, values, 1024);

  close(file);

  if (bytes_read <= 0)
    return -1;

  // Find the terminator if it exists
  for (int i = 0; i < bytes_read; i++) {
    if (values[i] == '}' || values[i] == 0) {
      return i + 1;
    }
  }

  return bytes_read;
}
