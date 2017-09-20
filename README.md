omfk is an experimental kernel witten in C. Mostly it's for the self-educational purpose.

# Prerequisites

## Supported devices

- stm32f303XX (tested on `NUCLEO STM32f303RE`)

## Tools

omfk is being developed under `Archlinux AMD64`, so build instructions will be targeted mostly on it.

- `arm-none-eabi-gcc` from `community` repo (in Ubuntu `gcc-arm-none-eabi`) :  
build tooldchain

- `minicom` from `community` repo :  
for testing via debug terminal

- `stlink` from `community` repo ([Ubuntu users need to install from source](https://github.com/texane/stlink/blob/master/doc/compiling.md)) :  
for writing embedded flash memory

# Build instructions

Just call `make`. Kernel binary `omfk.bin` will be generated in `build/` directory.

## Build targets

- all :  
create build directory and build kernel binary

- flash :  
build kernel and flash it to the device

- test :  
build, flash kernel and the launch minicom on `/dev/ttyACMO` (NOTE: device is configurable via `SERIAL_DEVICE` variable e.g. `make test SERIAL_DEVICE=/dev/ttyUSB0`)

- clean :  
delete build directory
