# Hey Emacs, this is a -*- makefile -*-
##############################################################################
# Architecture or project specific options
#

# utilities script and tricks
# How to recursively find all files with the same name in a given folder
# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


ALL_INDEX_HTMLS := $(call rwildcard,foo/,index.html)

# How to recursively find all files that match a pattern
# BLUETOOTH_FULLSRC += $(call rwildcard,$(BLUETOOTHDIR_LIB)/src/,*.cpp)
# STARTUPSRC =  $(wildcard $(NRF52)/*.c)
# STARTUPSRC += $(call rwildcard, $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/,*.c)
# STARTUPSRC += $(call rwildcard, $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/,*.cpp)
# STARTUPSRC += $(wildcard $(NRF52)/*.cpp)
# STARTUPSRC += $(wildcard $(NRF52)/**/*.cpp)


#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#
MCU_LDSCRIPT=nrf52840_s140_v6
# Imported source files and paths
NRF52_BASE = $(TOP_DIR)/keyboards/ergozad/boards/nrf52
NRF52 = $(NRF52_BASE)/core
# directories and sources path
NRFEXPRESS = $(NRF52)/feather_nrf52840_express
NRFEXPRESS_LIB = $(NRF52_BASE)/libraries
STARTUPLD = $(NRFEXPRESS)/ld
BLUETOOTHDIR_LIB = $(NRFEXPRESS_LIB)/Bluefruit52Lib
NEOPIXEL_LIB = $(NRFEXPRESS_LIB)/Adafruit_NeoPixel
BUTTONFEVER_LIB = $(NRFEXPRESS_LIB)/ButtonFever/src/
BLUETOOTH_INCDIR = $(BLUETOOTHDIR_LIB)/src/ $(BLUETOOTHDIR_LIB)/src/utility $(BLUETOOTHDIR_LIB)/src/services $(BLUETOOTHDIR_LIB)/src/clients
LITTLEFS_DIR_LIB = $(NRFEXPRESS_LIB)/Adafruit_LittleFS
LITTLEFS_INCDIR = $(LITTLEFS_DIR_LIB)/src/ $(LITTLEFS_DIR_LIB)/src/littlefs
INTERNALFS_DIR_LIB = $(NRFEXPRESS_LIB)/InternalFileSystem
INTERNALFS_INCDIR = $(INTERNALFS_DIR_LIB)/src/ $(INTERNALFS_DIR_LIB)/src/flash
OLED_INCDIR = $(NRFEXPRESS_LIB)/oled
SPI_INCDIR = $(NRFEXPRESS_LIB)/SPI
WIRE_INCDIR = $(NRFEXPRESS_LIB)/Wire
# this is added only in case or BLUEFRUIT_ENABLE , see rules.mk
ifeq ($(strip $(BLUEFRUIT_ENABLE)), yes)
INCDIR += $(BLUETOOTH_INCDIR)
INCDIR += $(LITTLEFS_INCDIR)
INCDIR += $(INTERNALFS_INCDIR)
INCDIR += $(NEOPIXEL_LIB)
INCDIR += $(BUTTONFEVER_LIB)
INCDIR += $(SPI_INCDIR)
INCDIR += $(WIRE_INCDIR)
INCDIR += $(OLED_INCDIR)
INCDIR += $(OLED_INCDIR)/csrc
endif
# Here we keep all the parameter for linking compiling and so on check platform
INCDIR += $(NRF52) $(NRF52)/nordic $(NRF52)/cmsis/include $(NRF52)/freertos/config \
         $(NRF52)/utility \
		 $(NRF52)/common \
         $(NRF52)/nordic/nrfx $(NRF52)/nordic/nrfx/hal $(NRF52)/nordic/nrfx/mdk $(NRF52)/nordic/nrfx/soc \
         $(NRF52)/nordic/nrfx/drivers/include $(NRF52)/nordic/nrfx/drivers/src \
         $(NRF52)/nordic/softdevice/s140_nrf52_6.1.1_API/include \
         $(NRF52)/freertos/Source/include \
         $(NRF52)/freertos/config \
         $(NRF52)/freertos/portable/GCC/nrf52 \
         $(NRF52)/freertos/portable/CMSIS/nrf52 \
         $(NRF52)/sysview/Config \
         $(NRF52)/Adafruit_TinyUSB_Core \
         $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src \
         $(NRF52)/Adafruit_TinyUSB_Core/tinyusb/src/class/cdc \
         $(NRF52)/sysview/SEGGER \
         $(NRFEXPRESS) $(APP_INC)

ifneq ("$(wildcard $(KEYBOARD_PATH_1)/$(KEYBOARD_FOLDER_1).h)","")
    INCDIR += $(KEYBOARD_PATH_1)
endif
ifneq ("$(wildcard $(KEYBOARD_PATH_2)/$(KEYBOARD_FOLDER_2).h)","")
    INCDIR += $(KEYBOARD_PATH_2)
endif
ifneq ("$(wildcard $(KEYBOARD_PATH_3)/$(KEYBOARD_FOLDER_3).h)","")
    INCDIR += $(KEYBOARD_PATH_3)
endif
ifneq ("$(wildcard $(KEYBOARD_PATH_4)/$(KEYBOARD_FOLDER_4).h)","")
    INCDIR += $(KEYBOARD_PATH_4)
endif
ifneq ("$(wildcard $(KEYBOARD_PATH_5)/$(KEYBOARD_FOLDER_5).h)","")
    INCDIR += $(KEYBOARD_PATH_5)/
endif
# get all the sources necessary to build the core lib $(BUILD_DIR)/$(TARGET).a

BLUETOOTH_FULLSRC := $(call rwildcard,$(BLUETOOTHDIR_LIB)/src/,*.c)
BLUETOOTH_FULLSRC += $(call rwildcard,$(BLUETOOTHDIR_LIB)/src/,*.cpp)
LITTLEFS_SRC := $(call rwildcard,$(LITTLEFS_DIR_LIB)/src/,*.c)
LITTLEFS_SRC += $(call rwildcard,$(LITTLEFS_DIR_LIB)/src/,*.cpp)
INTERNALFS_SRC := $(call rwildcard,$(INTERNALFS_DIR_LIB)/src/,*.c)
INTERNALFS_SRC += $(call rwildcard,$(INTERNALFS_DIR_LIB)/src/,*.cpp)
NEOPIXEL_SRC += $(call rwildcard,$(NEOPIXEL_LIB)/,*.cpp)
BUTTONFEVER_LIB_SRC += $(call rwildcard,$(BUTTONFEVER_LIB)/,*.cpp)
OLED_LIB_SRC += $(call rwildcard,$(OLED_INCDIR)/,*.cpp)
OLED_LIB_SRC += $(call rwildcard,$(SPI_INCDIR)/,*.cpp)
OLED_LIB_SRC += $(call rwildcard,$(WIRE_INCDIR)/,*.cpp)
OLED_LIB_SRC += $(call rwildcard,$(OLED_INCDIR)/csrc,*.c)

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
$(NRF52)/common/timer.c \
$(NRF52)/common/suspend.c \
$(NRF52)/common/eeprom.c \
$(NRF52)/common/bootloader.c \
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
OPT_DEFS += -DU8X8_USE_PINS


APP_SRC += $(NRFEXPRESS)/variant.cpp

NRFEXPRESSSRC := $(patsubst $(TOP_DIR)/%,%,$(CORESRC))
VPATH += $(NRFEXPRESSSRC)


#
# Project, sources and paths
##############################################################################


##############################################################################

# Define programs and commands.
SHELL = sh
REMOVE = rm -f
REMOVEDIR = rmdir
COPY = cp

# Compiler settings
# for references: $(TOP_DIR)/keyboards/ergozad/boards/nrf52840express/platform.txt
CC = arm-none-eabi-gcc
CCP = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
AR = arm-none-eabi-ar
NM = arm-none-eabi-nm
HEX = $(OBJCOPY) -O ihex

QUANTUM = quantum/ quantum/audio quantum/process_keycode tmk_core/common/
#IINCDIR = $(patsubst -I%,%,$(INCDIR) $(APP_INC) $(DINCDIR) $(UINCDIR) $(BLUETOOTH_INCDIR) $(LITTLEFS_INCDIR) $(INTERNALFS_INCDIR) $(QUANTUM))
#IINCDIR   = $(patsubst %,-I%,$(IINCDIR))
#to use also tmk_core files use this line

SRC += $(APP_SRC) $(QUANTUM_SRC)

IINCDIR   = $(patsubst %,-I%,$(INCDIR) $(APP_INC) $(DINCDIR) $(UINCDIR) $(BLUETOOTH_INCDIR) $(LITTLEFS_INCDIR) $(INTERNALFS_INCDIR) $(QUANTUM))

C_SOURCE   := $(filter %.c, $(CORESRC))
S_SOURCE   := $(filter %.s, $(CORESRC)) $(filter %.S, $(CORESRC))
CPP_SOURCE := $(filter %.cpp, $(CORESRC))
# are used only to build the final library .a which is then used with the APP_SRC
C_OBJ = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(CORESRC)))))
print_var:
	echo QMK_KEYBOARD_H = $(QMK_KEYBOARD_H)
	echo KEYBOARD_PATH_1 = $(KEYBOARD_PATH_1)
	echo KEYBOARD_FOLDER_1 = $(KEYBOARD_FOLDER_1)
	echo KEYBOARD_PATH_2 = $(KEYBOARD_PATH_2)
	echo KEYBOARD_FOLDER_2 = $(KEYBOARD_FOLDER_2)
	echo KEYBOARD_PATH_3 = $(KEYBOARD_PATH_3)
	echo KEYBOARD_FOLDER_3 = $(KEYBOARD_FOLDER_3)
	echo KEYBOARD_PATH_4 = $(KEYBOARD_PATH_4)
	echo KEYBOARD_FOLDER_4 = $(KEYBOARD_FOLDER_4)
	echo KEYBOARD_PATH_5 = $(KEYBOARD_PATH_5)
	echo KEYBOARD_FOLDER_5 = $(KEYBOARD_FOLDER_5)

gen_file:
	echo SRC = $(SRC)
	$(foreach APP,$(SRC), $(eval APP_C_SRC+=$(filter %.c, $(APP))) )
	$(foreach APP,$(SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(SRC), $(eval IINCDIR+=-I$(dir $(APP))) )
	$(foreach APP,$(TMK_COMMON_SRC), $(eval IINCDIR+=-I$(dir $(APP))) )
ifeq ($(strip $(BLUEFRUIT_ENABLE)), yes)
	$(foreach APP,$(BLUETOOTH_FULLSRC), $(eval APP_C_SRC+=$(filter %.c, $(APP))) )
	$(foreach APP,$(BLUETOOTH_FULLSRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(LITTLEFS_SRC), $(eval APP_C_SRC+=$(filter %.c, $(APP))) )
	$(foreach APP,$(LITTLEFS_SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(INTERNALFS_SRC), $(eval APP_C_SRC+=$(filter %.c, $(APP))) )
	$(foreach APP,$(INTERNALFS_SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(NEOPIXEL_SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(BUTTONFEVER_LIB_SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(INTERNALFS_INCDIR), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(OLED_LIB_SRC), $(eval APP_CPP_SRC+=$(filter %.cpp, $(APP))) )
	$(foreach APP,$(OLED_LIB_SRC), $(eval APP_C_SRC+=$(filter %.c, $(APP))) )

endif
	echo filing $(APP_INC)

COMMON_VPATH += $(DRIVER_PATH)/arm

COMMON_FLAGS += $(CDC_FLAGS) -DCFG_TUSB_OS=OPT_OS_FREERTOS -D__LINT__ -D__NVIC_PRIO_BITS $(OPT_DEFS)

ARDUINO_FLAGS += -DF_CPU=64000000 -DARDUINO=10809 -DARDUINO_NRF52840_FEATHER -DARDUINO_ARCH_NRF52 "-DARDUINO_BSP_VERSION=\"0.14.6\"" -DNRF52840_XXAA
ARDUINO_FLAGS += -DUSBCON -DUSE_TINYUSB -DUSB_VID=0x239A -DUSB_PID=0x8029 "-DUSB_MANUFACTURER=\"Adafruit LLC\"" "-DUSB_PRODUCT=\"Feather nRF52840 Express\""
ARDUINO_FLAGS += -DSOFTDEVICE_PRESENT -DARDUINO_NRF52_ADAFRUIT -DNRF52_SERIES -DLFS_NAME_MAX=64 -Ofast -DCFG_DEBUG=2 -DQMK_KEYBOARD_H=\"$(QMK_KEYBOARD_H)\" $(HALF_LAYOUT)

# avoid the problem in quantum debug_enabled not defined
#ARDUINO_FLAGS += -include $(NRF52)/debug.h

MCUFLAGS = -mcpu=cortex-m4 -mthumb -c -g -w -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float

# to be used with gcc
ASFLAGS = -c -g -x assembler-with-cpp $(ARDUINO_FLAGS) $(IINCDIR)



# to be used with the g++
CPPFLAGS = $(MCUFLAGS) -std=gnu++11 -ffunction-sections -fdata-sections
CPPFLAGS += -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD
CPPFLAGS += $(ARDUINO_FLAGS) $(IINCDIR)

# to be used with the gcc
CFLAGS = $(MCUFLAGS) -std=gnu11 -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -MMD $(ARDUINO_FLAGS) $(IINCDIR)

# to be used during the creation of the elf file
ELF_FLAGS= -L$(BUILD_DIR) -Ofast -Wl,--gc-sections -save-temps -L$(STARTUPLD)  -L$(BUILD_DIR)/$(STARTUPLD) -T$(MCU_LDSCRIPT).ld -Wl,-Map,$(BUILD_DIR)/$(TARGET).map
ELF_FLAGS +=-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float
ELF_FLAGS +=-Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
ELF_FLAGS +=--specs=nano.specs --specs=nosys.specs



OBJ_DIR = $(BUILD_DIR)/obj_$(TARGET)

# used when creating the zip to be flashed on the target
#  adafruit-nrfutil --verbose dfu serial -pkg ergozad_express_test_default.zip -p /dev/ttyACM0 -b 115200 --singlebank
DFU_UTIL ?= adafruit-nrfutil
DFU_ARGS ?= dfu genpkg --dev-type 0x0052 --sd-req 0x00B6 --application
ifneq ("$(SERIAL)","")
	DFU_ARGS += -S $(SERIAL)
endif

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



# Generate a .qmk for the QMK-FF
qmk:$(BUILD_DIR)/$(TARGET).zip
	printf "@ $(TARGET).json\n@=info.json\n" | zipnote -w $(TARGET).qmk


# Define a list of all objects
define compile
mkdir -p $(OBJ_DIR)/$(dir $1)
echo "SRC_compile=$1 -> $2 $3 $1 -o $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))"
@$(SILENT) || printf "$(MSG_COMPILING) $1" | $(AWK_CMD)
$(eval CMD := $2 $3 $1 -o $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))
@$(BUILD_CMD)
endef

# Add the build directory to the output
define compile_app
mkdir -p $(OBJ_DIR)/$(dir $1)
echo "SRC_compile_app=$1 -> $2 $3 $1 -o $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))"
@$(SILENT) || printf "APPLICATION $(MSG_COMPILING) $1" | $(AWK_CMD)
$(eval CMD := $2 $3 $1 -o  $(OBJ_DIR)/$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$1)))))
@$(BUILD_CMD)
endef

define archive
mkdir -p $(dir $1)
echo "SRC=$1 -> $(AR) rcs $1 $2"
@$(SILENT) || printf "Archiving $1" | $(AWK_CMD)
$(eval CMD=$$(AR) rcs $1 $2)
@$(BUILD_CMD)
endef


# substitute with SRC
#APP_C_OBJ = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(OBJ_DIR)/$(APP_SRC)))

DEPFILES := $(APP_C_OBJ:%.o=$(BUILD_DIR)/%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))

elf:$(BUILD_DIR)/$(TARGET).elf

ALL_ASFLAGS = $(MCUFLAGS) -x assembler-with-cpp $(ASFLAGS) $(EXTRAFLAGS)

# todo: remember to add again the dependency gen_file for building the code
# necessary to add the qmk code
$(BUILD_DIR)/lib$(TARGET).a:
	@mkdir -p $(@D)
	echo IINCDIR = $(IINCDIR)
	$(REMOVE) -f $(BUILD_DIR)/$(TARGET).elf
	$(foreach APP,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(APP_CPP_SRC))), $(eval APP_C_OBJ+=$(OBJ_DIR)/$(APP)))
	$(foreach APP,$(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(APP_C_SRC))), $(eval APP_C_OBJ+=$(OBJ_DIR)/$(APP)))
	$(foreach OUTPUT,$(APP_C_SRC), $(call compile_app ,$(OUTPUT), $(CC), $(CFLAGS)))
	$(foreach OUTPUT,$(APP_CPP_SRC), $(call compile_app ,$(OUTPUT), $(CCP), $(CPPFLAGS)))
	$(foreach OUTPUT,$(C_SOURCE), $(call compile ,$(OUTPUT), $(CC), $(CFLAGS)) )
	$(foreach OUTPUT,$(CPP_SOURCE), $(call compile ,$(OUTPUT), $(CCP), $(CPPFLAGS)))
	$(foreach OUTPUT,$(S_SOURCE), $(call compile ,$(OUTPUT), $(CC), $(ALL_ASFLAGS)))
	echo C_OBJ = $(C_OBJ)
	$(foreach OUTPUT,$(C_OBJ), $(call archive ,$@, $(OBJ_DIR)/$(OUTPUT)))
	@$(BUILD_CMD)

%.zip: $(BUILD_DIR)/$(TARGET).hex
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

### start tmk_core/rule.mk remove once done

cpfirmware: $(FIRMWARE_FORMAT)
	$(SILENT) || printf "Copying $(TARGET).$(FIRMWARE_FORMAT) to qmk_firmware folder" | $(AWK_CMD)
	$(COPY) $(BUILD_DIR)/$(TARGET).$(FIRMWARE_FORMAT) $(TARGET).$(FIRMWARE_FORMAT) && $(PRINT_OK)
eep: $(BUILD_DIR)/$(TARGET).eep
lss: $(BUILD_DIR)/$(TARGET).lss
sym: $(BUILD_DIR)/$(TARGET).sym
LIBNAME=$(BUILD_DIR)/lib$(TARGET).a
lib: $(LIBNAME)

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(BUILD_DIR)/$(TARGET).hex
#ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf
ELFSIZE = $(SIZE) $(BUILD_DIR)/$(TARGET).elf


# Display compiler version information.
gccversion :
	@$(SILENT) || $(CC) --version


BEGIN = gccversion
# Link: create ELF output file from object files.
.SECONDARY : $(BUILD_DIR)/$(TARGET).elf
.PRECIOUS : $(OBJ)

%.elf: $(OBJ ) | $(BEGIN)
	#echo "#################### ELF FILE ########################"
	#echo "OBJ=$(OBJ)"
	#echo "SRC=$(SRC)"
	#echo "ELF_FLAGS=$(ELF_FLAGS)"
	#echo APP_C_SRC = $(APP_C_SRC)
	#echo APP_CPP_SRC = $(APP_CPP_SRC)
	#echo "########"
	#echo APP_C_OBJ = $(APP_C_OBJ)
	echo "$(CC) $(ELF_FLAGS) -o $@ $(APP_C_OBJ) -Wl,--start-group -lm $(BUILD_DIR)/lib$(TARGET).a -Wl,--end-group"
	@$(SILENT) || printf "$(MSG_LINKING) $@" | $(AWK_CMD)
	$(eval CMD=$(CC) $(ELF_FLAGS) -o $@ $(APP_C_OBJ) -Wl,--start-group -lm $(BUILD_DIR)/lib$(TARGET).a -Wl,--end-group)
	@$(BUILD_CMD)

# Create final output files (.hex, .eep) from ELF output file.
#arm-none-eabi-objcopy -O ihex /tmp/arduino_build_527534/hid_keyboard.ino.elf /tmp/arduino_build_527534/hid_keyboard.ino.hex

%.hex: %.elf
	@$(SILENT) || printf "$(MSG_FLASH) yhooo $@ \n" | $(AWK_CMD)
	@$(SILENT) || echo CMD=$(HEX) $< $@
	$(eval CMD=$(HEX) $< $@ )
	@$(BUILD_CMD)


zip: clean gen_file lib $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).zip
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

all: zip
clean: cleanfull
# Target: clean project.
cleanfull:
	echo INCDIR = $(INCDIR)
	echo "LIBNAME= $(LIBNAME)"
	$(REMOVE) $(LIBNAME)
	$(foreach OUTPUT,$(OBJ), $(REMOVE) -r $(OUTPUT) 2>/dev/null)
