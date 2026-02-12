# zbitx Refactor

restructured:

- `src/`: Source code organized by component (core, drivers, modems, etc.)
- `include/`: Header files
- `scripts/`: Build and utility scripts
- `libs/`: External libraries (ft8_lib)
- `docs/`: Documentation
- `assets/`: Binary assets, images, and resources
- `data/`: Runtime data files

## Building

To build:

```bash
make
```

### Using Meson

Install the required dependencies:

```bash
sudo apt update
sudo apt install meson ninja-build build-essential

```

1. Setup the build directory:
```bash
meson setup builddir
```

2. Compile:
```bash
meson compile -C builddir
```

3. Run the application:
```bash
./builddir/sbitx
```

## Kernel I2C Configuration

To use the Kernel I2C interface for Si5351 and DS3231 (replacing bit-banged I2C), add the following to `/boot/firmware/config.txt`:

```ini
dtoverlay=i2c-gpio,bus=3,i2c_gpio_sda=13,i2c_gpio_scl=6
dtoverlay=i2c-rtc-gpio,ds1307,i2c_gpio_sda=13,i2c_gpio_scl=6
```

This creates a new I2C bus (likely `/dev/i2c-3`) and loads the RTC driver.

After reboot, you can verify the devices:
```bash
i2cdetect -y 3
```
You should see `UU` at address 0x68 (DS3231 under kernel control) and `60` at address 0x60 (Si5351).

To read the RTC time via the kernel driver:
```bash
cat /sys/class/rtc/rtc0/date
cat /sys/class/rtc/rtc0/time
```
