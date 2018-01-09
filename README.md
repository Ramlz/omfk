`omfk` is an experimental kernel written in C. Mostly it's for the 
self-educational purpose.

# Prerequisites

## Supported devices

- stm32f3 (tested on `NUCLEO STM32f303RE`)

## Tools

`omfk` is being developed under `Archlinux AMD64`, so build instructions will 
be targeted mostly on it.

- `arm-none-eabi-gcc` from `community` repo (in Ubuntu `gcc-arm-none-eabi`) :  
build toolchain

- `minicom` from `community` repo :  
for testing via debug terminal

- `stlink` from `community` repo ([Ubuntu users need to install from source](
https://github.com/texane/stlink/blob/master/doc/compiling.md)) :  
for writing embedded flash memory

# Build instructions

Just call `make`. The kernel binary `omfk.bin` will be generated in `build/` 
directory. 

## Build targets

- all :  
create build directory and build the kernel binary

- flash :  
build the kernel binary  and flash it to the device

- test :  
build, flash the kernel binary and launch minicom on `/dev/ttyACMO`  
(NOTE: device is configurable via `SERIAL_DEVICE` variable e.g. 
`make test SERIAL_DEVICE=/dev/ttyUSB0`)

- clean :  
delete build directory

## Build options

All configurable build options can be found in `user_vars.mk`  

- CROSS_TOOL :  
host cross-compiler prefix

- ARCH :  
target architecture e.g. `arm`

- CORE :  
target CPU

- PLATFORM :  
target MCU family

- PLATFORM_SERIES :  
target MCU series/model

- SERIAL_DEVICE :  
debug tty device

- FLASH_ADDR :  
start of MCU flash memory address
