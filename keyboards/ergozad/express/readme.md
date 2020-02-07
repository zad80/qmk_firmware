at this moment the project is still using the Arduino compiler so issue the following:
~~~
export PATH=$PATH:/home/zad/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/
~~~

Make example for this keyboard (after setting up your build environment):


Ther are two 'projects':
- test
  This is a simple project used only to validate the building inside the qmk source tree.
  The project builds a simple nrf52680express adafruit compatible application wihch blink the 
  default led at 2 seconds intervals.
~~~
    make ergozad/express/test:default
    make ergozad/express/test:default:flash
~~~
- version1
~~~
    make ergozad/express/version1:default
    make ergozad/express/version1:default:flash
~~~

Note: for both projects you have to manually place the board in dfu mode,
double press the reset button once connected.
Todo: implement the open close routine to place the board in dfu via software.

## tmk_core
To integrate the tmk_core, we need to find out which is the main loop which execute and read the keys from the matrix
    keyboard_task();