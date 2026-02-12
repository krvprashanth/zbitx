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

### Prerequisites

Install the required dependencies:

```bash
sudo apt update
sudo apt install meson ninja-build build-essential
sudo apt install libgtk-3-dev libasound2-dev libfftw3-dev libncurses-dev libsqlite3-dev
```

For `wiringPi` (required for hardware control), you may need to install it from source or a specific package depending on your distribution. See `docs/install.txt` for details.

### Using Meson

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
