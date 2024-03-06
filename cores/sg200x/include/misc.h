/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#ifndef _WIRING_
#define _WIRING_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "wiring_constants.h"

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned long us);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_ */
