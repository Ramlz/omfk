CROSS_TOOL              ?= arm-none-eabi-

ARCH                    ?= arm
CORE                    ?= m4
PLATFORM                ?= stm32
PLATFORM_SERIES         ?= f3

BOARD                   ?= nucleo-stm32f303K8

SERIAL_DEVICE           ?= /dev/ttyACM0

FLASH_ADDR              ?= 0x8000000
