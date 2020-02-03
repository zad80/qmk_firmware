# MCU name
MCU = cortex-m4
NRF52840EX = NRF52840EXP
SPACE_CADET_ENABLE = no
MAGIC_ENABLE = no
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
APP_SRC += $(MAIN_APP_PATH)/hid_keyboard.cpp

MSG_COMPILING = Compiling ergozad test C:
MSG_COMPILING_CPP = Compiling ergozad test C++:
USE_FPU_OPT = true
USE_FPU = true
# LED Configuration
#LED_MATRIX_ENABLE = IS31FL3731

# Build Options
#   comment out to disable the options.
#
#BACKLIGHT_ENABLE = yes
#BOOTMAGIC_ENABLE = yes	# Virtual DIP switch configuration
## (Note that for BOOTMAGIC on Teensy LC you have to use a custom .ld script.)
#MOUSEKEY_ENABLE = yes	# Mouse keys
#EXTRAKEY_ENABLE = yes	# Audio control and System control
#CONSOLE_ENABLE = yes	# Console for debug
#COMMAND_ENABLE = yes    # Commands for debug and configuration
#SLEEP_LED_ENABLE = yes  # Breathing sleep LED during USB suspend
#NKRO_ENABLE = yes	    # USB Nkey Rollover
#CUSTOM_MATRIX = yes # Custom matrix file
AUDIO_ENABLE = no
# SERIAL_LINK_ENABLE = yes

# project specific files
#SRC =	led.c
#LAYOUTS += 66_ansi
