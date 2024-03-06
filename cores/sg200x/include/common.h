/*
  Copyright (c) 2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "variant.h"

#include "binary.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __WEAK         __attribute__((weak))

#ifndef __ALWAYS_STATIC_INLINE
#define __ALWAYS_STATIC_INLINE  __attribute__((always_inline)) static inline
#endif

#ifndef __ASM
#define __ASM                   __asm     /*!< asm keyword for GNU Compiler */
#endif

#ifndef __STATIC_INLINE
#define __STATIC_INLINE         static inline
#endif

#define     __IM     volatile const       /*! Defines 'read only' structure member permissions */
#define     __OM     volatile             /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile             /*! Defines 'read / write' structure member permissions */

typedef enum {
  LOW     = 0,
  HIGH    = 1,
  CHANGE  = 2,
  FALLING = 3,
  RISING  = 4,
} PinStatus;

typedef enum {
  INPUT            = 0x0,
  OUTPUT           = 0x1,
  INPUT_PULLUP     = 0x2,
  INPUT_PULLDOWN   = 0x3,
  OUTPUT_OPENDRAIN = 0x4,
} PinMode;

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))  // abs() comes from STL
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x)     ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))  // round() comes from STL
#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define sq(x) ((x) * (x))
#define clockCyclesPerTick() (F_CPU / 100L)
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) (((a) * 1000L) / (F_CPU / 1000L))
#define microsecondsToClockCycles(a) ((a) * (F_CPU / 1000000L))

#define lowByte(w) ((uint8_t)((w) & 0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;
typedef unsigned int word;
typedef uint8_t pin_size_t;

typedef int (*irq_handler_t)(int irqn, void *priv);
void init(void);
void irq_init(void);
void tick_init(void);
void disable_irq(unsigned int irqn);
void enable_irq(unsigned int irqn);
int attach_irq(int irqn, irq_handler_t handler, unsigned long flags,
               const char *name, void *priv);
int request_irq(int irqn, irq_handler_t handler, unsigned long flags,
                const char *name, void *priv);
int detach_irq(int irqn);
void set_priority_irq(int irq_num, int priority);

#ifdef __cplusplus
}

void setup(void);
void loop(void);

#endif
#endif
