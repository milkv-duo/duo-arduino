/*
Copyright (c) 2015 Intel Corporation.  All right reserved.
Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.

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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "common.h"
#include "csi_gpio_pin.h"
#include "csi_pin.h"
#include "csi_adc.h"
#include "csi_pwm.h"
#ifdef __cplusplus
 extern "C" {
#endif

extern dev_pin_map_t adc_map[];
extern dev_pin_map_t pwm_map[];
extern pin_name_t out_pin_map[];

/* Standard Arduino PWM resolution */
static int _readResolution = 10;
static int _ADCResolution = 12;
static int _writeResolution = 8;

extern csi_gpio_pin_t pin[];

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
    if (from == to) {
        return value;
    }
    if (from > to) {
        return value >> (from-to);
    }
    return value << (to-from);
}

void analogWriteResolution(int bits)
{
    _writeResolution = bits;
}

void analogReadResolution(int bits)
{
    _readResolution = bits;
}

void analogWrite(uint8_t pinNumber, uint32_t val)
{
    csi_pwm_t pwm;

    const dev_pin_map_t* pwm_pin = target_pin_number_to_dev(pinNumber, pwm_map, 0xFF);

    if (pwm_pin == NULL) {
        pr_err("pin GPIO %d is not used as PWM func\n", pinNumber);
        return;
    }

    uint8_t pwm_idx = pwm_pin->idx >> 2;
    uint8_t pwm_channel = pwm_pin->idx & 0x3;

    if (csi_pin_set_mux(pwm_pin->name, pwm_pin->func)) {
        pr_err("pin GPIO %d fails to config as PWM func\n", pinNumber);
        return;
    };

    csi_error_t ret_status = csi_pwm_init(&pwm, pwm_idx);
    if (ret_status != CSI_OK) {
        pr_err("GPIO pin %d init failed\n", pinNumber);
        return;
    }

    csi_pwm_out_stop(&pwm, pwm_channel);

    csi_pwm_out_config_continuous(&pwm, pwm_channel, 1 << _writeResolution,
                                  val & ((1 << _writeResolution) - 1),
                                  PWM_POLARITY_HIGH);
    csi_pwm_out_start(&pwm, pwm_channel);
}

uint32_t analogRead(uint32_t pinNumber)
{
    const dev_pin_map_t* adc_pin = target_pin_number_to_dev(pinNumber, adc_map, 0xFF);

    if (adc_pin == NULL) {
        pr_err("pin GPIO %d is not used as ADC func\n", pinNumber);
        return;
    }

    if (csi_pin_set_mux(adc_pin->name, adc_pin->func)) {
        pr_err("pin GPIO %d fails to config as ADC func\n", pinNumber);
        return;
    };

    pinMode(pinNumber, INPUT);

    csi_adc_t adc;
    csi_adc_init(&adc, 0);

    csi_adc_channel_enable(&adc, 1, true);

    csi_adc_start(&adc);

    int value = csi_adc_read(&adc);

    if (value < 0) {
        value = value & 0xFFF;
    }

    csi_adc_uninit(&adc);

    return mapResolution(value, _ADCResolution, _readResolution);
}

void analogReference(uint8_t mode)
{
    (void)mode;
    return;
}

#ifdef __cplusplus
}
#endif
