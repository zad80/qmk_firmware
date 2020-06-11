#ifndef BATTERY_UTILS_HEADER
#define BATTERY_UTILS_HEADER
#include <stdarg.h>
#include <bluefruit.h>
#include <delay.h>
#define VBAT_MV_PER_LSB   (0.73242188F)   // 3.0V ADC range and 12-bit ADC resolution = 3000mV/4096

#ifdef NRF52840_XXAA
#define VBAT_DIVIDER      (0.5F)          // 150K + 150K voltage divider on VBAT
#define VBAT_DIVIDER_COMP (2.0F)          // Compensation factor for the VBAT divider
#else
#define VBAT_DIVIDER      (0.71275837F)   // 2M + 0.806M voltage divider on VBAT = (2M / (0.806M + 2M))
#define VBAT_DIVIDER_COMP (1.403F)        // Compensation factor for the VBAT divider
#endif
#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

#ifdef __cplusplus
// place here the simbol you wish to export to c programs too
extern "C"{
    void setupBattery();
    float readVBAT();
    uint8_t mvToPercent(float);
};

#endif
#endif
