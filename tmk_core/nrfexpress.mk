# Hey Emacs, this is a -*- makefile -*-
##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x800
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#
MCU_LDSCRIPT=nrf52840_s140_v6
# Imported source files and paths
NRF52 = $(TOP_DIR)/keyboards/ergozad/boards/nrf52


NRFEXPRESS = $(NRF52)/feather_nrf52840_express
STARTUPLD=$(NRFEXPRESS)/ld
# Here we keep all the parameter for linking compiling and so on check platform

INCDIR = $(NRF52) $(NRF52)/nordic $(NRF52)/cmsis/include $(NRF52)/freertos/config \
         $(NRF52)/utility \
         $(NRF52)/nordic/nrfx $(NRF52)/nordic/nrfx/hal $(NRF52)/nordic/nrfx/mdk $(NRF52)/nordic/nrfx/soc \
         $(NRF52)/nordic/nrfx/drivers/include $(NRF52)/nordic/nrfx/drivers/src \
         $(NRF52)/nordic/softdevice/s140_nrf52_6.1.1_API/include \
         $(NRF52)/freertos/Source/include \
         $(NRF52)/freertos/config \
         $(NRF52)/freertos/portable/GCC/nrf52 \
         $(NRF52)/freertos/portable/CMSIS/nrf52 \
         $(NRF52)/sysview/Config \
         $(NRF52)/sysview/SEGGER \
         $(NRF52)/Adafruit_TinyUSB_Core \
         $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src \
         $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/cdc \
         $(NRF52)/sysview/SEGGER \
         $(NRFEXPRESS)


#STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.c
# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# How to recursively find all files with the same name in a given folder
ALL_INDEX_HTMLS := $(call rwildcard,foo/,index.html)

# How to recursively find all files that match a pattern
ALL_HTMLS := $(call rwildcard,foo/,*.html)

# STARTUPSRC =  $(wildcard $(NRF52)/*.c)
# STARTUPSRC += $(call rwildcard, $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/,*.c)
# STARTUPSRC += $(call rwildcard, $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/,*.cpp)
# STARTUPSRC += $(wildcard $(NRF52)/*.cpp)
# STARTUPSRC += $(wildcard $(NRF52)/**/*.cpp)


STARTUPASM = $(STARTUPLD)/gcc_startup_nrf52840.S $(STARTUPLD)/gcc_startup_nrf52.S

CORESRC = $(NRF52)/Adafruit_TinyUSB_Core/Adafruit_TinyUSB_Core.cpp \
$(NRF52)/Adafruit_TinyUSB_Core/Adafruit_USBD_CDC.cpp \
$(NRF52)/Adafruit_TinyUSB_Core/Adafruit_USBD_Device.cpp \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/cdc/cdc_device.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/hid/hid_device.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/midi/midi_device.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/msc/msc_device.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/vendor/vendor_device.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/common/tusb_fifo.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/device/usbd.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/device/usbd_control.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/portable/nordic/nrf5x/dcd_nrf5x.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/portable/nordic/nrf5x/hal_nrf5x.c \
$(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/tusb.c \
$(NRF52)/HardwarePWM.cpp \
$(NRF52)/IPAddress.cpp \
$(NRF52)/Print.cpp \
$(NRF52)/RingBuffer.cpp \
$(NRF52)/Stream.cpp \
$(NRF52)/Tone.cpp \
$(NRF52)/Uart.cpp \
$(NRF52)/WInterrupts.c \
$(NRF52)/WMath.cpp \
$(NRF52)/WString.cpp \
$(NRF52)/abi.cpp \
$(NRF52)/avr/dtostrf.c \
$(NRF52)/delay.c \
$(NRF52)/freertos/Source/croutine.c \
$(NRF52)/freertos/Source/event_groups.c \
$(NRF52)/freertos/Source/list.c \
$(NRF52)/freertos/Source/portable/MemMang/heap_3.c \
$(NRF52)/freertos/Source/queue.c \
$(NRF52)/freertos/Source/stream_buffer.c \
$(NRF52)/freertos/Source/tasks.c \
$(NRF52)/freertos/Source/timers.c \
$(NRF52)/freertos/portable/CMSIS/nrf52/port_cmsis.c \
$(NRF52)/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c \
$(NRF52)/freertos/portable/GCC/nrf52/port.c \
$(NRF52)/hooks.c \
$(NRF52)/itoa.c \
$(NRF52)/linker/gcc_startup_nrf52.S \
$(NRF52)/linker/gcc_startup_nrf52840.S \
$(NRF52)/main.cpp \
$(NRF52)/new.cpp \
$(NRF52)/nordic/nrfx/drivers/src/nrfx_power.c \
$(NRF52)/nordic/nrfx/drivers/src/nrfx_qspi.c \
$(NRF52)/nordic/nrfx/drivers/src/nrfx_spim.c \
$(NRF52)/nordic/nrfx/hal/nrf_ecb.c \
$(NRF52)/nordic/nrfx/hal/nrf_nvmc.c \
$(NRF52)/nordic/nrfx/mdk/system_nrf52.c \
$(NRF52)/nordic/nrfx/mdk/system_nrf52840.c \
$(NRF52)/pulse.c \
$(NRF52)/pulse_asm.S \
$(NRF52)/rtos.cpp \
$(NRF52)/sysview/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c \
$(NRF52)/sysview/SEGGER/SEGGER_RTT.c \
$(NRF52)/sysview/SEGGER/SEGGER_SYSVIEW.c \
$(NRF52)/sysview/SEGGER_SYSVIEW_FreeRTOS.c \
$(NRF52)/utility/AdaCallback.c \
$(NRF52)/utility/adafruit_fifo.cpp \
$(NRF52)/utility/debug.cpp \
$(NRF52)/utility/utilities.c \
$(NRF52)/wiring.c \
$(NRF52)/wiring_analog.cpp \
$(NRF52)/wiring_analog_nRF52.c \
$(NRF52)/wiring_digital.c \
$(NRF52)/wiring_private.c \
$(NRF52)/wiring_shift.c $(STARTUPASM)



#STARTUPINC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/startup/ARMCMx/devices/STM32L4xx \
             $(CHIBIOS)/os/common/ext/CMSIS/include \
             $(CHIBIOS)/os/common/ext/CMSIS/ST/STM32L4xx


# Define linker script file here
ifneq ("$(wildcard $(KEYBOARD_PATH_5)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_5)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_4)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_3)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_2)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_1)/ld/$(MCU_LDSCRIPT).ld
else
    LDSCRIPT = $(STARTUPLD)/$(MCU_LDSCRIPT).ld
endif

APP_SRC += $(NRFEXPRESS)/variant.cpp

NRFEXPRESSSRC = $(KERNSRC) \
       $(PORTSRC) \
       $(OSALSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(STREAMSSRC) \
	   $(STARTUPLD) \
	   $(PORTASM) \
	   $(OSALASM)

NRFEXPRESSSRC := $(patsubst $(TOP_DIR)/%,%,$(NRFEXPRESSSRC))
VPATH+=$(NRFEXPRESSSRC)


#
# Project, sources and paths
##############################################################################


##############################################################################
# Compiler settings
# for references: $(TOP_DIR)/keyboards/ergozad/boards/nrf52840express/platform.txt
CC = arm-none-eabi-gcc
CCP = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
AR = arm-none-eabi-ar
NM = arm-none-eabi-nm
HEX = $(OBJCOPY) -O $(FORMAT)
EEP =
BIN = $(OBJCOPY) -O binary

IINCDIR   = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))

# this receipe is for adding to APP_C_SRC APP_CPP_SRC and APP_INC the file which
# are included from other makefile, like common.mk tmk_core/rules.mk this is necessary because
# for some reason , using filter as below doesn't work.
APP_C_SRC := $(filter %.c, $(SRC)) $(filter %.c, $(APP_SRC))
APP_CPP_SRC := $(filter %.cpp, $(SRC)) $(filter %.cpp, $(APP_SRC))
gen_file:
	$(foreach APP,$(SRC), $(eval APP_C_SRC+=$(filter %.c, $(APP))) )
	$(foreach APP,$(SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(SRC), $(eval APP_INC+=-I$(dir $(APP))) )
	echo filing $(APP_INC)

IINCDIR += $(APP_INC)

COMMON_VPATH += $(DRIVER_PATH)/arm

# cpp build rules
# /home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-g++
# -mcpu=cortex-m4 -mthumb
# -c -g -w -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -std=gnu++11
# -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib
# --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD -DF_CPU=64000000
# -DARDUINO=10809 -DARDUINO_NRF52840_FEATHER -DARDUINO_ARCH_NRF52 "-DARDUINO_BSP_VERSION=\"0.14.6\"" -DNRF52840_XXAA -DUSBCON -DUSE_TINYUSB -DUSB_VID=0x239A -DUSB_PID=0x8029 "-DUSB_MANUFACTURER=\"Adafruit LLC\"" "-DUSB_PRODUCT=\"Feather nRF52840 Express\"" -DSOFTDEVICE_PRESENT -DARDUINO_NRF52_ADAFRUIT -DNRF52_SERIES -DLFS_NAME_MAX=64 -Ofast -DCFG_DEBUG=2
# -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/cmsis/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/hal -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/mdk -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/soc -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/drivers/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/drivers/src -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/softdevice/s140_nrf52_6.1.1_API/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/Source/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/config -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/portable/GCC/nrf52 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/portable/CMSIS/nrf52 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/sysview/SEGGER -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/sysview/Config -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/Adafruit_TinyUSB_Core -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/Adafruit_TinyUSB_Core/tinyusb/src -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/variants/feather_nrf52840_express
# /home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/Uart.cpp -o /tmp/arduino_build_527534/core/Uart.cpp.o

# c compilation rules
# /home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-gcc
# -mcpu=cortex-m4 -mthumb
# -c -g -w -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -std=gnu11
# -ffunction-sections -fdata-sections -nostdlib
# --param max-inline-insns-single=500 -MMD -DF_CPU=64000000 -DARDUINO=10809 -DARDUINO_NRF52840_FEATHER -DARDUINO_ARCH_NRF52 "-DARDUINO_BSP_VERSION=\"0.14.6\"" -DNRF52840_XXAA -DUSBCON -DUSE_TINYUSB -DUSB_VID=0x239A -DUSB_PID=0x8029 "-DUSB_MANUFACTURER=\"Adafruit LLC\"" "-DUSB_PRODUCT=\"Feather nRF52840 Express\"" -DSOFTDEVICE_PRESENT -DARDUINO_NRF52_ADAFRUIT -DNRF52_SERIES -DLFS_NAME_MAX=64 -Ofast -DCFG_DEBUG=2 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/cmsis/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/hal -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/mdk -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/soc -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/drivers/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/nrfx/drivers/src -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/nordic/softdevice/s140_nrf52_6.1.1_API/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/Source/include -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/config -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/portable/GCC/nrf52 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/freertos/portable/CMSIS/nrf52 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/sysview/SEGGER -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/sysview/Config -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/Adafruit_TinyUSB_Core -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/Adafruit_TinyUSB_Core/tinyusb/src -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5 -I/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/variants/feather_nrf52840_express /home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/Adafruit_TinyUSB_Core/tinyusb/src/class/msc/msc_device.c -o /tmp/arduino_build_527534/core/Adafruit_TinyUSB_Core/tinyusb/src/class/msc/msc_device.c.o

THUMBFLAGS = -DTHUMB_PRESENT -DTHUMB_NO_INTERWORKING -mthumb -DTHUMB
# COMPILEFLAGS += -fomit-frame-pointer
COMPILEFLAGS += -nostdlib $(MCUFLAGS)
COMPILEFLAGS += -ffunction-sections  -mfloat-abi=hard
COMPILEFLAGS += -fdata-sections -MMD -DF_CPU=64000000 -u _printf_float -w  --param max-inline-insns-single=500
COMPILEFLAGS += $(IINCDIR) $(THUMBFLAGS)
# dependency for generating dependency files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d
COMPILEFLAGS += $(DEPFLAGS)

ARDUINO_FLAGS += -DARDUINO=10809 -DARDUINO_NRF52840_FEATHER -DARDUINO_ARCH_NRF52 "-DARDUINO_BSP_VERSION=\"0.14.6\"" -DNRF52840_XXAA -DUSBCON -DUSE_TINYUSB -DUSB_VID=0x239A -DUSB_PID=0x8029 "-DUSB_MANUFACTURER=\"Adafruit LLC\"" "-DUSB_PRODUCT=\"Feather nRF52840 Express\"" -DSOFTDEVICE_PRESENT -DARDUINO_NRF52_ADAFRUIT -DNRF52_SERIES -DLFS_NAME_MAX=64 -Ofast -DCFG_DEBUG=2
ARDUINO_FLAGS += $(CDC_FLAGS) -Iqmk_firmware/quantum/
FREERTOS_FLAGS =  -DCFG_TUSB_OS=OPT_OS_FREERTOS -D__LINT__
CFLAGS += $(COMPILEFLAGS) $(ARDUINO_FLAGS) $(FREERTOS_FLAGS) -std=gnu11
ASFLAGS += $(ARDUINO_FLAGS)
ASFLAGS += $(THUMBFLAGS)

CPPFLAGS += $(COMPILEFLAGS) -fno-rtti -std=gnu++11 -fno-threadsafe-statics -fno-exceptions
CPPFLAGS += $(ARDUINO_FLAGS)  $(FREERTOS_FLAGS)


# /home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-gcc -L/tmp/arduino_build_527534 -Ofast -Wl,--gc-sections -save-temps
# -L/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/linker -Tnrf52840_s140_v6.ld -Wl,-Map,/tmp/arduino_build_527534/hid_keyboard.ino.map
# -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
# --specs=nano.specs --specs=nosys.specs -o /tmp/arduino_build_527534/hid_keyboard.ino.elf
# /tmp/arduino_build_527534/sketch/hid_keyboard.ino.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEAdvertising.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLECentral.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLECharacteristic.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEClientCharacteristic.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEClientService.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEConnection.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEDiscovery.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEGatt.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEPeriph.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEScanner.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEService.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEUuid.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/bluefruit.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEAncs.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientBas.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientCts.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientDis.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientHidAdafruit.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientUart.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEBas.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEBeacon.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEDfu.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEDis.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEHidAdafruit.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEHidGeneric.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEMidi.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEUart.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/EddyStone.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/utility/AdaMsg.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/utility/bonding.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/utility/bootloader_util.c.o /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/Adafruit_LittleFS.cpp.o
# /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/Adafruit_LittleFS_File.cpp.o /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/littlefs/lfs.c.o
# /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/littlefs/lfs_util.c.o /tmp/arduino_build_527534/libraries/InternalFileSytem/InternalFileSystem.cpp.o
# /tmp/arduino_build_527534/libraries/InternalFileSytem/flash/flash_cache.c.o /tmp/arduino_build_527534/libraries/InternalFileSytem/flash/flash_nrf5x.c.o /tmp/arduino_build_527534/core/variant.cpp.o
# -Wl,--start-group -lm /tmp/arduino_build_527534/core/core.a -Wl,--end-group

LDFLAGS +=-Wl,--gc-sections -T$(MCU_LDSCRIPT).ld -L$(STARTUPLD)
LDFLAGS +=-Wl,--no-wchar-size-warning
LDFLAGS +=-Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
LDFLAGS +=--specs=nano.specs --specs=nosys.specs
LDFLAGS += -mthumb -mfloat-abi=hard
LDFLAGS += -Wl,--start-group $(BUILD_DIR)/lib$(TARGET).a -Wl,--end-group
LDSYMBOLS =,--defsym=__process_stack_size__=$(USE_PROCESS_STACKSIZE)
LDSYMBOLS :=$(LDSYMBOLS),--defsym=__main_stack_size__=$(USE_EXCEPTIONS_STACKSIZE)
#LDFLAGS += -Wl,--script=$(LDSCRIPT)$(LDSYMBOLS)

OPT_DEFS += -DPROTOCOL_CHIBIOS

MCUFLAGS = -mcpu=$(MCU)

OBJ_DIR = $(BUILD_DIR)/obj_$(TARGET)

# FPU options default (Cortex-M4 and Cortex-M7 single precision).
ifeq ($(USE_FPU_OPT),)
  USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv4-sp-d16 -fsingle-precision-constant
endif

# FPU-related options
ifeq ($(USE_FPU),)
  USE_FPU = no
endif
ifneq ($(USE_FPU),no)
  OPT    += $(USE_FPU_OPT)
  OPT_DEFS  += -DCORTEX_USE_FPU=TRUE
else
  OPT_DEFS  += -DCORTEX_USE_FPU=FALSE
endif

DEBUG = gdb

DFU_ARGS ?= dfu genpkg --dev-type 0x0052 --sd-req 0x00B6 --application
ifneq ("$(SERIAL)","")
	DFU_ARGS += -S $(SERIAL)
endif

ST_LINK_ARGS ?=


DFU_UTIL ?= adafruit-nrfutil
#  adafruit-nrfutil --verbose dfu serial -pkg ergozad_express_test_default.zip -p /dev/ttyACM0 -b 115200 --singlebank
# Generate a .qmk for the QMK-FF
qmk:$(BUILD_DIR)/$(TARGET).zip
	printf "@ $(TARGET).json\n@=info.json\n" | zipnote -w $(TARGET).qmk

define EXEC_DFU_UTIL
	until lsusb | grep -q "ID 239a:0029"; do\
		echo "nrf52840express not found ." ;\
		sleep 5 ;\
	done
	$(DFU_UTIL) --verbose dfu serial -pkg $(BUILD_DIR)/$(TARGET).zip -p /dev/ttyACM0 -b 115200 --singlebank
endef

dfu-util:
	$(call EXEC_DFU_UTIL)
dfu-util-split-left: dfu-util

dfu-util-split-right: dfu-util
# adafruit-nrfutil dfu genpkg --dev-type 0x0052 --sd-req 0x00B6 --application
# /tmp/arduino_build_117665/hid_keyboard.ino.hex /tmp/arduino_build_117665/hid_keyboard.ino.zip

# Define a list of all objects
define compile
mkdir -p $(OBJ_DIR)/$(dir $1)
echo "SRC=$1 -> $2 -c $3 $1 -o $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))"
@$(SILENT) || printf "$(MSG_COMPILING) $1" | $(AWK_CMD)
$(eval CMD := $2 -c $3 $1 -o $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))
@$(BUILD_CMD)
endef

# Add the build directory to the output
define compile_app
mkdir -p $(OBJ_DIR)/$(dir $1)
echo "SRC=$1 -> $2 -c $3 $1 -o $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))"
@$(SILENT) || printf "APPLICATION $(MSG_COMPILING) $1" | $(AWK_CMD)
$(eval CMD := $2 -c $3 $1 -o  $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))
@$(BUILD_CMD)
endef

define archive
mkdir -p $(dir $1)
echo "SRC=$1 -> $(AR) rcs $1 $2"
@$(SILENT) || printf "Archiving $1" | $(AWK_CMD)
$(eval CMD=$$(AR) rcs $1 $2)
@$(BUILD_CMD)
endef

C_SOURCE   := $(filter %.c, $(CORESRC))
S_SOURCE   := $(filter %.s, $(CORESRC)) $(filter %.S, $(CORESRC))
CPP_SOURCE := $(filter %.cpp, $(CORESRC))
C_OBJ = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(CORESRC)))))


# substitute with SRC
APP_C_OBJ = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(OBJ_DIR)/$(APP_SRC)))

DEPFILES := $(APP_C_OBJ:%.o=%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))


ALL_ASFLAGS = $(MCUFLAGS) -x assembler-with-cpp $(ASFLAGS) $(EXTRAFLAGS)
# todo: remember to add again the dependency gen_file for building the code
# necessary to add the qmk code
$(BUILD_DIR)/lib$(TARGET).a:
	@mkdir -p $(@D)
	rm -f $(BUILD_DIR)/$(TARGET).zip
	rm -f $(BUILD_DIR)/$(TARGET).hex
	rm -f $(BUILD_DIR)/$(TARGET).elf
	$(foreach OUTPUT,$(APP_C_SRC), $(call compile_app ,$(OUTPUT), $(CC), $(CFLAGS)))
	$(foreach OUTPUT,$(APP_CPP_SRC), $(call compile_app ,$(OUTPUT), $(CCP), $(CPPFLAGS)))
	$(foreach OUTPUT,$(C_SOURCE), $(call compile ,$(OUTPUT), $(CC), $(CFLAGS)) )
	$(foreach OUTPUT,$(CPP_SOURCE), $(call compile ,$(OUTPUT), $(CCP), $(CPPFLAGS)))
	$(foreach OUTPUT,$(S_SOURCE), $(call compile ,$(OUTPUT), $(CC), $(ALL_ASFLAGS)))
	echo C_OBJ = $(C_OBJ)
	$(foreach OUTPUT,$(C_OBJ), $(call archive ,$@, $(OBJ_DIR)/$(OUTPUT)))
	@$(BUILD_CMD)

%.zip: %.hex
	$(DFU_UTIL) $(DFU_ARGS) $< $@

# arm-none-eabi-gcc -L/tmp/arduino_build_527534 -Ofast -Wl,--gc-sections -save-temps -L/home/zad/.arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/linker
# -Tnrf52840_s140_v6.ld -Wl,-Map,/tmp/arduino_build_527534/hid_keyboard.ino.map
# -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
# --specs=nano.specs --specs=nosys.specs -o /tmp/arduino_build_527534/hid_keyboard.ino.elf
# /tmp/arduino_build_527534/sketch/hid_keyboard.ino.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEAdvertising.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLECentral.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLECharacteristic.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEClientCharacteristic.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEClientService.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEConnection.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEDiscovery.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEGatt.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEPeriph.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEScanner.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEService.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/BLEUuid.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/bluefruit.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEAncs.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientBas.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientCts.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientDis.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientHidAdafruit.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/clients/BLEClientUart.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEBas.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEBeacon.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEDfu.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEDis.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEHidAdafruit.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEHidGeneric.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEMidi.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/BLEUart.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/services/EddyStone.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/utility/AdaMsg.cpp.o
# /tmp/arduino_build_527534/libraries/Bluefruit52Lib/utility/bonding.cpp.o /tmp/arduino_build_527534/libraries/Bluefruit52Lib/utility/bootloader_util.c.o
# /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/Adafruit_LittleFS.cpp.o /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/Adafruit_LittleFS_File.cpp.o
# /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/littlefs/lfs.c.o /tmp/arduino_build_527534/libraries/Adafruit_LittleFS/littlefs/lfs_util.c.o
# /tmp/arduino_build_527534/libraries/InternalFileSytem/InternalFileSystem.cpp.o /tmp/arduino_build_527534/libraries/InternalFileSytem/flash/flash_cache.c.o
# /tmp/arduino_build_527534/libraries/InternalFileSytem/flash/flash_nrf5x.c.o /tmp/arduino_build_527534/core/variant.cpp.o
# -Wl,--start-group -lm /tmp/arduino_build_527534/core/core.a -Wl,--end-group
ELF_FLAGS=-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float -Ofast -Wl,--gc-sections -save-temps -T$(MCU_LDSCRIPT).ld -L$(STARTUPLD)
ELF_FLAGS +=-Wl,-Map,$(BUILD_DIR)/$(TARGET).map
ELF_FLAGS +=-Wl,--cref -Wl,--check-sections -Wl,--check-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
ELF_FLAGS +=--specs=nano.specs --specs=nosys.specs

%.elf: $(OBJ) | $(BEGIN)
	echo "OBJ=$(OBJ)"
	echo "SRC=$(SRC)"
	echo "ELF_FLAGS=$(ELF_FLAGS)"
	echo APP_C_SRC = $(APP_C_SRC)
	echo APP_CPP_SRC = $(APP_CPP_SRC)
	echo "########"
	echo APP_C_OBJ = $(APP_C_OBJ)
	echo "$(CC) $(ELF_FLAGS) -o $@ $(APP_C_OBJ) -Wl,--start-group -lm $(BUILD_DIR)/lib$(TARGET).a -Wl,--end-group"
	@$(SILENT) || printf "$(MSG_LINKING) $@" | $(AWK_CMD)
	$(eval CMD=$(CC) $(ELF_FLAGS) -o $@ $(APP_C_OBJ) -Wl,--start-group -lm $(BUILD_DIR)/lib$(TARGET).a -Wl,--end-group)
	@$(BUILD_CMD)

zip: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).zip
flash: zip dfu-util
ifeq ($(strip $(BOOTLOADER)),dfu)
	$(call EXEC_DFU_UTIL)
else ifeq ($(strip $(MCU_FAMILY)),KINETIS)
	$(call EXEC_TEENSY)
else ifeq ($(strip $(MCU_FAMILY)),STM32)
	$(call EXEC_DFU_UTIL)
else
	$(PRINT_OK); $(SILENT) || printf "$(MSG_FLASH_BOOTLOADER)"
endif

all: lib zip
clean: cleanfull
# Target: clean project.
cleanfull:
	echo "LIBNAME= $(LIBNAME)"
	$(REMOVE) $(LIBNAME)
	$(foreach OUTPUT,$(OBJ), $(REMOVE) -r $(OUTPUT) 2>/dev/null)