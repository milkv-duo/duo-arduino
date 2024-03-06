/*
  Copyright (c) 2022, Canaan Bright Sight Co., Ltd
  This file is modified from part of the k210 core for Arduino environment.
*/

#include "common.h"
#include "csi_riscv_gcc.h"

static bool maxed = false;
static uint32_t pad = 0, n = 0, d = 0;
static uint8_t dat = 0;

static void set_seed(unsigned long seed)
{
    maxed = true;
    pad = (uint32_t)(seed * csi_clint_get_value() & 0xFFFFFFFF);
    n = (uint32_t)(seed * csi_clint_get_value() & 0x12345678);
    d = (uint32_t)(seed * csi_clint_get_value() & 0x87654321);
}

static uint32_t get_random(void)
{
    if (!maxed) {
        set_seed(1);
    }

    pad += dat + d * n;
    pad = (pad << 3) + (pad >> 29);
    n = pad | 2;
    d ^= (pad << 31) + (pad >> 1);
    dat ^= (char)pad ^ (d >> 8) ^ 1;

    return pad ^ (d << 5) ^ (pad >> 18) ^ (dat << 1);
}


void randomSeed(uint32_t seed)
{
    set_seed(seed);
    get_random();
}

long random(long max)
{
    uint32_t x = get_random();
    uint64_t m = uint64_t(x) * uint64_t(max);
    uint32_t l = uint32_t(m);
    if (l < max) {
        uint32_t t = -max;
        if (t >= max) {
            t -= max;
            if (t >= max)
                t %= max;
        }
        while (l < t) {
            x = get_random();
            m = uint64_t(x) * uint64_t(max);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

long random(long min, long max)
{
    if (min >= max) {
        return min;
    }
    long diff = max - min;
    return random(diff) + min;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    const long run = in_max - in_min;
    if (run == 0) {
        return -1;
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;
    return (delta * rise) / run + out_min;
}

uint16_t makeWord(uint16_t w)
{
    return w;
}

uint16_t makeWord(uint8_t h, uint8_t l)
{
    return (h << 8) | l;
}
