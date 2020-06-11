#ifndef SINGLE_LED_HEADER
#define SINGLE_LED_HEADER
#include <stdarg.h>
#include <bluefruit.h>
#include <delay.h>
// reference https://github.com/SethSenpai/singleLEDLibrary
#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    void setupBattery();
    float readVBAT();
    uint8_t mvToPercent(float);
};
class sllib
{
    public:
    //public variables and fucntions
        sllib(int pin);

        //breathing
        void breathSingle(int speed);

        //blink
        void blinkSingle(int speed);
        void blinkSingle(int timeHigh, int timeLow);
        void blinkRandomSingle(int minTime, int maxTime);

        //flicker
        void flickerSingle();
        void flickerSingle(int intMin, int intMax);
        void flickerSingle(int intMin, int intMax, int speed);

        //blink pattern
        void patternSingle(int pattern[], int speed);

        //future update function
        void update();
        void setPatternSingle(int pattern[], int lenghtarray);
        void setBreathSingle(int speed);
        void setFlickerSingle();
        void setBlinkSingle(int speed);
        void setRandomBlinkSingle(int minTime, int maxTime);
        void setOnSingle();
        void setOffSingle();
        int runningFunction = 0;

    private:
    //private variables and fucntion
        //gobal variables
        int _pin;
        unsigned long milOld;

        int speedp;
        int timep;
        //int pPatt[];
        int* arrP = 0;

        //variables for blinking
        bool ioBlink = false;
        int rndTemp = 0;

        //variables for pattern
        int counter = 0;

        bool _use_analog = false;
};
#endif
#endif
