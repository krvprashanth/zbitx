# zbitx Firmware

This repository contains the firmware for the sBitx radio.

## Project Structure

The project has been restructured into a standard embedded firmware layout:

- `src/`: Source code organized by component (core, drivers, modems, etc.)
- `include/`: Header files
- `scripts/`: Build and utility scripts
- `libs/`: External libraries (ft8_lib)
- `docs/`: Documentation
- `assets/`: Binary assets, images, and resources
- `data/`: Runtime data files

## Building

To build the project:

```bash
make
```

Or use the build script:

```bash
./scripts/build
```

## Running

To start the application:

```bash
./scripts/start.sh
```

## Updating

To pull the latest changes and rebuild:

```bash
./scripts/update
```
