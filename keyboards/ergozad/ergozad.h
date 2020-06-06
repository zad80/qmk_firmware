//
// Created by zad on 2/5/20.
//
/*
 *
This file is used to define the matrix for your keyboard. You should define at least one C macro which translates an array into a matrix representing the physical switch matrix for your keyboard. If it's possible to build your keyboard with multiple layouts you should define additional macros.
If you have only a single layout you should call this macro LAYOUT.
When defining multiple layouts you should have a base layout, named LAYOUT_all, that supports all possible switch positions on your matrix, even if that layout is impossible to build physically. This is the macro you should use in your default keymap. You should then have additional keymaps named default_<layout> that use your other layout macros. This will make it easier for people to use the layouts you define.
Layout macro names are entirely lowercase, except for the word LAYOUT at the front.
 */

#ifndef QMK_FIRMWARE_ERGOZAD_H
#define QMK_FIRMWARE_ERGOZAD_H

#include "quantum.h"

#define LAYOUT_ergodox(                                           \
                                                                  \
    Ar0c0, Ar0c1, Ar0c2, Ar0c3, Ar0c4, Ar0c5, Ar0c6,              \
    Ar1c0, Ar1c1, Ar1c2, Ar1c3, Ar1c4, Ar1c5, Ar1c6,              \
    Ar2c0, Ar2c1, Ar2c2, Ar2c3, Ar2c4, Ar2c5, Ar2c6, Ar2c7, Ar2c8,\
    Ar3c0, Ar3c1, Ar3c2, Ar3c3, Ar3c4, Ar3c5,               Ar3c8,\
    Ar4c0, Ar4c1, Ar4c2, Ar4c3, Ar4c4,        Ar4c6, Ar4c7, Ar4c8,\
                                                                \
    Br0c0, Br0c1, Br0c2, Br0c3, Br0c4, Br0c5, Br0c6,              \
    Br1c0, Br1c1, Br1c2, Br1c3, Br1c4, Br1c5, Br1c6,              \
    Br2c0, Br2c1, Br2c2, Br2c3, Br2c4, Br2c5, Br2c6, Br2c7 ,Br2c8,\
    Br3c0, Br3c1, Br3c2, Br3c3, Br3c4, Br3c5,               Br3c8,\
    Br4c0, Br4c1, Br4c2, Br4c3, Br4c4,        Br4c6, Br4c7, Br4c8)\
   {                                                            \
    {Ar0c0, Ar0c1, Ar0c2, Ar0c3, Ar0c4, Ar0c5, Ar0c6, KC_NO, KC_NO,    Br0c0, Br0c1, Br0c2, Br0c3, Br0c4, Br0c5, Br0c6, KC_NO, KC_NO},\
    {Ar1c0, Ar1c1, Ar1c2, Ar1c3, Ar1c4, Ar1c5, Ar1c6, KC_NO, KC_NO,    Br1c0, Br1c1, Br1c2, Br1c3, Br1c4, Br1c5, Br1c6, KC_NO, KC_NO},\
    {Ar2c0, Ar2c1, Ar2c2, Ar2c3, Ar2c4, Ar2c5, Ar2c6, Ar2c7, Ar2c8,    Br2c0, Br2c1, Br2c2, Br2c3, Br2c4, Br2c5, Br2c6, Br2c7, Br2c8},\
    {Ar3c0, Ar3c1, Ar3c2, Ar3c3, Ar3c4, Ar3c5, KC_NO, KC_NO, Ar3c8,    Br3c0, Br3c1, Br3c2, Br3c3, Br3c4, Br3c5, KC_NO, KC_NO, Br3c8},\
    {Ar4c0, Ar4c1, Ar4c2, Ar4c3, Ar4c4, KC_NO, Ar4c6, Ar4c7, Ar4c8,    Br4c0, Br4c1, Br4c2, Br4c3, Br4c4, KC_NO, Br4c6, Br4c7, Br4c8},\
                                                                 \
    {},\
    {},\
    {},\
    {},\
    {},\
   }
#endif //QMK_FIRMWARE_ERGOZAD_H


// #define LAYOUT_ergodox(                                         \
//                                                                 \
//     k00,k01,k02,k03,k04,k05,k06,                                \
//     k10,k11,k12,k13,k14,k15,k16,                                \
//     k20,k21,k22,k23,k24,k25,k26,k27,k28,                         \
//     k30,k31,k32,k33,k34,k35,k36,                                 \
//     k40,k41,k42,k43,k44,k45,k46,k47,k48,                         \
//                                                                 \
//     k09,k0A,k0B,k0C,k0D,k0E,k0F,                                \
//     k19,k1A,k1B,k1C,k1D,k1E,k1F,                                \
//     k29,k2A,k2B,k2C,k2D,k2E,k2F,k2G,k2H,                         \
//     k39,k3A,k3B,k3C,k3D,k3E,k3F,                                 \
//     k49,k4A,k4B,k4C,k4D,k4E,k4F,k4G,k4H)                         \
//                                                                 \
//    /* matrix positions every line is a column !!! 9 colum (line here) for 5 row */             \
// /* Y -------------------------------------------->  krc   */         \
//    {                                                            \
//     { k40, k30, k20, k10, k00 },   \
//     { k41, k31, k21, k11, k01 },   \
//     { k42, k32, k22, k12, k02 },   \
//     { k43, k33, k23, k13, k03  },   \
//     { k44, k34, k24, k14, k04  },   \
//     { KC_NO, KC_NO, k25, k15, k05 },   \
//     { k46, k36, KC_NO, k16, k06 },   \
//     { k47, KC_NO, k27, KC_NO,KC_NO },   \
//     { k48, KC_NO , k28, KC_NO,KC_NO },   \
//                                                                  \
//     { k49, k39, k29, k19, k09 },   \
//     { k4A, k3A, k2A, k1A, k0A },   \
//     { k4B, k3B, k2B, k1B, k0B },   \
//     { k4C, k3C, k2C, k1C, k0C  },   \
//     { k4D, k3D, k2D, k1D, k0D  },   \
//     { KC_NO, KC_NO, k2E, k1E, k0E },   \
//     { k4F, k3F, KC_NO, k1F, k0F },   \
//     { k4G, KC_NO, k2G, KC_NO,KC_NO },   \
//     { k4H, KC_NO, k2H, KC_NO ,KC_NO },   \
//    }
// #endif //QMK_FIRMWARE_ERGOZAD_H
