/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

#include "csi_riscv_gcc.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

static void _500usdelay(void)
{
    uint64_t start = csi_clint_get_value();
    uint64_t cur;
    uint32_t cnt = (F_CPU / 1000U / 2U);

    while (1) {
        cur = csi_clint_get_value();

        if (start > cur) {
            if ((start - cur) >= cnt) {
                break;
            }
        } else {
            if (cur - start >= cnt) {
                break;
            }
        }
    }
}

__WEAK void delay(uint64_t ms)
{
    while (ms) {
        ms--;
        _500usdelay();
        _500usdelay();
    }
}

static void _10udelay(void)
{
    uint64_t start = csi_clint_get_value();
    uint32_t cnt = (F_CPU / 1000U / 100U);

    while (1) {
        uint64_t cur = csi_clint_get_value();

        if (start > cur) {
            if ((start - cur) >= cnt) {
                break;
            }
        } else {
            if (cur - start >= cnt) {
                break;
            }
        }
    }
}

/**
 * Ps: At least delay over 10us
*/
void delayMicroseconds(uint64_t us)
{
    uint64_t ms = us / 1000;
    delay(ms);
    us -= ms * 1000;
    us /= 10U;
    while (us) {
        us--;
        _10udelay();
    }
}

#ifdef __cplusplus
} // extern "C"
#endif
