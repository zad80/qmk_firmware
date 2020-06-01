# ErgoDox BT

This project lists all the step and reference used to build an Ergodox keyboard with bluetooth functionality.

## References

Here a list of useful link useful to the developing of the project:

- [Nrf52840 MDK usb dongle wiki ](<https://wiki.makerdiary.com/nrf52840-mdk-usb-dongle/programming/>)
- [Ergodox Bt project](xudongz.com/blog/2019/ergoblue)

  - <https://github.com/ergoblue/adafruit-nrf52/> arduino core for the adafruit nrf52840
  - 

- [Ergodox 3d print stl](https://www.thingiverse.com/thing:2533064)

- [Adafruit nfr52840 express](<https://www.adafruit.com/product/4062>)
  It's our take on an 'all-in-one' Arduino-compatible + Bluetooth Low Energy with built in USB plus battery charging. With native USB it's even ready to join the CircuitPython party **For this chip, we've added Arduino IDE support** - you can program the nRF52840 chip directly to take full advantage of the Cortex-M4 processor, and then calling into the Nordic SoftDevice radio stack when you need to communicate over BLE. Since the underlying API and peripherals are the same for the '832 and '840, you can supercharge your older nRF52832 projects with the same exact code, with a single recompile! **We've also chosen this chip for our first BLE-friendly CircuitPython board**! CircuitPython works best with disk drive access, and this is the only BLE-plus-USB-native chip that has the memory to handle running a the little Python interpreter. The massive RAM and speedy Cortex M4F chip makes this a good match.

  It's got tons of awesome peripherals: plenty of GPIO, analog inputs, PWM, timers, etc. Best of all, it's got that native USB! Finally, no need for a separate USB serial chip like CP2104 or FT232. Serial is handled as a USB CDC descriptor, and the chip can act like a keyboard, mouse, MIDI device or even disk drive. (Note that we don't have support for anything but CDC for Arduino at this time)

  We pre-programed the chip with our UF2 bootloader, which can use either commandline UART programming with nrfutil (we use this for Arduino) or drag-n-drop mass storage, for CircuitPython installation and also because mass-storage-drive bootloaders make updating firmware so easy. Want to program the chip directly? You can use our command line tools with your favorite editor and toolchain. 

  To make it easy to use for portable projects, we added a connector for any of our 3.7V Lithium polymer batteries and built in battery charging. You don't need a battery because it will run just fine straight from the micro USB connector. But, if you do have a battery, you can take it on the go, then plug in the USB to recharge. The Feather will automatically switch over to USB power when it's available. We also tied the battery thru a divider to an analog pin, so you can measure and monitor the battery voltage to detect when you need a recharge.

  [info](<https://learn.adafruit.com/introducing-the-adafruit-nrf52840-feather>)

  



## Tools

### adafruit-nrfutil

Python package used to flash and read nfr52X device via command line for adafruit product

  ~~~bash
  pip3 install adafruit-nrfutil
  ~~~
https://learn.adafruit.com/introducing-the-adafruit-nrf52840-feather/update-bootloader
the device may have an old bootloader:
to check connect the device with the usb cable and double click the reset button, a new usb drive will appear and inside it the file INFO_UF2.TXT will tell you which is the current bootloader version.

download the latest here : https://github.com/adafruit/Adafruit_nRF52_Bootloader/releases/download/0.2.13/feather_nrf52840_express_bootloader-0.2.13_s140_6.1.1.zip
and then issue the following command to update AFTER have reconnected the board and double cklicked the reset button in order to get the usb driver appears.


  ~~~bash
  adafruit-nrfutil --verbose dfu serial --package feather_nrf52840_express_bootloader-0.2.9_s140_6.1.1.zip -p /dev/ttyACM0 -b 115200 --singlebank --touch 1200
  ~~~

Note: install the one on github not the one with pip3 https://github.com/adafruit/Adafruit_nRF52_nrfutil#installing-from-source

### nrfutil

Python package used to flash and read nfr52X device via command line

### nrfconnect

Same purpose of nrfutils but with a GUI

### Arduino

Make sure to install at least arduino version 1.8 then add the following url to the address for board management url you find in settings:

Start the Arduino IDE
Go into Preferences
Add https://www.adafruit.com/package_adafruit_index.json as an 'Additional Board Manager URL' 

Open the board management and search for nrf52 , it will appear the adafruit boards collection, install it .

After that in Tool -> Board you can select the correct board which is Adafruit Bluefruit nRF52840 Feather Express.

### QMK

Documentation: <https://docs.qmk.fm>

Interesting makefile:

https://github.com/sekigon-gonnoc/qmk_firmware/blob/nrf52/tmk_core/nrf.mk

It compiles using the arm crosscompiler and then uses the original nrfutil and not the one from adafruit with the following parameters:
nrfutil dfu usb_serial -pkg $(TARGET).zip -p $$USB;

### QMK Integration

To integrate the qmk firmware we can follow two approaches :

1. Identify only the common code which is run in the main loop:

   ~~~C
   while (1) {
           keyboard_task();
       }
   ~~~
1. identify the parts which initialize the board and then the code which scan the matrix

Because both approaches have one steps in common, identify the code of the main loop, it is wise to start with the point number one and then eventually tackle also the step number two.



#### QMK common code

The common code is everything that is running in the keyboard_task(), this function is heavily ifdefined in order to enable or disable all the features available in the QMK firmware: https://docs.qmk.fm/#/features

We can divide this task in further subtask:

- identify the common code
- identify the API used by the common code to communicate with the hardware i.e https://docs.qmk.fm/#/internals_gpio_control
- identify how to compile simple code and to flash it on the board

#### Compilation and flash

Here we can reverse the Arduino ide in order to obtain the compilation options and flash task.

Building enabling verbose option during building it is possible to see the following output:

~~~

~~~
Looking at the output it is possible to identify 4 phases:

1. Libraries used in the project file using the arduino-builder and the .ino file
2. Compilation of the core.a library which contains all the dependencies identified at the point 1 plus the high level code used in the main loop 
3. Linking everything in the hex file
4. Creation of the .zip file by the use of the adafruit-nrfutil which at the end will be loaded on the final target.

The build tool used is dated 2017: /tmp/arduino_build_117665/core/core.a

Looking at the qmk_firmware there is a mkfile which uses similar compilation options and it is : ./lib/ugfx/tools/gmake_scripts/cpu_stm32m4.mk which seems supporting the following board equipped with the cortex-m4: <https://www.st.com/en/microcontrollers-microprocessors/stm32f303.html>

One board which includes that mk file is the STM32F429i-Discovery/ ./lib/ugfx/boards/base/STM32F429i-Discovery/example_chibios_2.x/Makefile . As stated in the main documentation of the qmk arm cpu are supported by chibios os.

Lets try to identify a keyboard which uses that processor in order to try to build it, the clueboard 66 uses 32-bit 72MHz ARM Cortex-M4 processor (STM32F303CCT6)

make clueboard/66_hotswap/gen1:default

Building the clueboard using the latest arm-eabi-none-gcc fails **but using the one available in the arduino works**:  PATH=$PATH:/home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/

The keyboards uses various pieces from 

lib/chibios/os

drivers/boards/GENERIC_STM32_F303XC/board.c

tmk_core/common

tmk_core/protocol

keyboards/clueboard/66_hotswap

quantum/

##### Creating a base for the keyboard

Create a copy of the keyboard clueboard after renaming directories and few file it was possible to compile with the command :

~~~bashexport 
PATH=$PATH:/home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/
make ergozad/express/version1:default:flash
~~~

Then we need to actually create the correct makefile

**The main gcc parameters are defined inside the tmk_core/chibios.mk** it is included in ./build_keyboard.mk when MCU_FAMILY is defined: 

~~~
# Determine and set parameters based on the keyboard's processor family.
# We can assume a ChibiOS target When MCU_FAMILY is defined since it's
# not used for LUFA
ifdef MCU_FAMILY
    FIRMWARE_FORMAT?=bin
    PLATFORM=CHIBIOS
else ifdef NRF52840EX <--------- Defined in ergozad/express/test/rules.mk
    PLATFORM=NRF52840
    FIRMWARE_FORMAT=zip
else ifdef ARM_ATSAM
    PLATFORM=ARM_ATSAM
    FIRMWARE_FORMAT=bin
else
    PLATFORM=AVR
    FIRMWARE_FORMAT?=hex
endif
~~~
I defined another PLATFORM=NRF52840 in case NRF52840EX is defined and the firmware format is zip. so we are going to define NRF52840EX in keyboard/ergozad/express/gen1/rules.mk and another platform mk will be defined in the tmk_core directory.

Moreover a new directory **lib/nrfexpress** has been created in order **to keep all the file necessary to build the firmware**.

The main makefile is **build_keyboard.mk**, which includes also **common.mk** and defines various things as keyboard folderpath and file format.

build_keyboard.mk includes common.mk includes message.mk (used to format messages during compilation)

**IN .arduino15/packages/adafruit/hardware/nrf52/0.14.6/cores/nRF5/linker/ there are all file for the linking part, ld** scripts and startup assembler, it can be useful to be used to create a new platforms



makefile explanation:

tmk_core/nrfexpress.mk where are defined specific final operations to build the firmware

tmk_core/rules.mk where are defined generic operations



NOTE:

to make a c/h file included directly it is possible to place a .c/.h file in a directory and name it with the same name of the containing directory.

For example ergozad/ergozad.c or ergozad/express/test/test.c otherwise it is necessary to explicitly add it in the relative keyboard rules.mk as :

~~~
SRC += filename.c
~~~

Build options:

`make COLOR=false` - turns off color output

`make SILENT=true` - turns off output besides errors/warnings

`make VERBOSE=true` - outputs all of the gcc stuff (not interesting, unless you need to debug)

`make EXTRAFLAGS=-E` - Preprocess the code without doing any compiling (useful if you are trying to debug #define commands)

File layout:

