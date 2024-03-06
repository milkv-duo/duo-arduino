/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "Arduino.h"
#include "common.h"
#include "csi_gpio_pin.h"
#include "csi_pin.h"
#include "csi_pwm.h"
#include "wiring_analog.h"
#include "Tone.h"

int channel = -1;
static csi_pwm_t active_pwm;

extern dev_pin_map_t pwm_map[];

void tone(pin_size_t pinNumber, unsigned int frequency)
{
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

    if (channel != -1 && channel != pwm_channel) {
        return;
    }

    if (channel == -1) {
        csi_error_t ret_status = csi_pwm_init(&active_pwm, pwm_idx);
        if (ret_status != CSI_OK) {
            pr_err("GPIO pin %d init failed\n", pinNumber);
            return;
        }
        channel = pwm_channel;
    }

    csi_pwm_out_stop(&active_pwm, pwm_channel);

    csi_pwm_out_config_continuous(&active_pwm, pwm_channel, 1000 * 1000 / frequency,
                                  1000 * 1000 / frequency / 2, PWM_POLARITY_LOW);

    csi_pwm_out_start(&active_pwm, pwm_channel);
}

void tone(pin_size_t pinNumber, unsigned int frequency, unsigned long duration)
{
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

    if (channel != -1 && channel != pwm_channel) {
        return;
    }

    if (channel == -1) {
        csi_error_t ret_status = csi_pwm_init(&active_pwm, pwm_idx);
        if (ret_status != CSI_OK) {
            pr_err("GPIO pin %d init failed\n", pinNumber);
            return;
        }
        channel = pwm_channel;
    }

    csi_pwm_out_stop(&active_pwm, pwm_channel);

    csi_pwm_out_config_count(&active_pwm, pwm_channel, 1000 * 1000 / frequency, duration);
    csi_pwm_out_start(&active_pwm, pwm_channel);
}

void noTone(pin_size_t __attribute__((unused)) pin)
{
    if (channel != -1) {
        csi_pwm_out_stop(&active_pwm, channel);
        channel = -1;
    }
}
