/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "WInterrupts.h"
#include "csi_gpio_pin.h"
#include "csi_pin.h"

extern csi_gpio_pin_t pin[];
extern pin_name_t out_pin_map[];

void attachInterrupt(pin_size_t pinNumber, void (*callback)(void), uint32_t mode)
{
    if (OUT_PIN_NUM <= pinNumber) {
        return;
    }

    pin_name_t name = out_pin_map[pinNumber];

    if (csi_pin_get_mux(name) != PIN_FUNC_GPIO) {
        pr_err("pin GPIO %d is not used as GPIO func\n", pinNumber);
        return;
    }

    if (pin[pinNumber].gpio == NULL) {
        return;
    }

    csi_gpio_irq_mode_t edge = GPIO_IRQ_MODE_RISING_EDGE;

    switch (mode) {
    case LOW:
        edge = GPIO_IRQ_MODE_LOW_LEVEL;
        break;
    case HIGH:
        edge = GPIO_IRQ_MODE_HIGH_LEVEL;
        break;
    case CHANGE:
        edge = GPIO_IRQ_MODE_BOTH_EDGE;
        break;
    case FALLING:
        edge = GPIO_IRQ_MODE_FALLING_EDGE;
        break;
    case RISING:
        edge = GPIO_IRQ_MODE_RISING_EDGE;
        break;
    default:
        edge = GPIO_IRQ_MODE_RISING_EDGE;
        break;
    }
    csi_gpio_pin_irq_mode(&pin[pinNumber], edge);
    csi_gpio_pin_attach_callback(&pin[pinNumber], callback, NULL);
    csi_gpio_pin_irq_enable(&pin[pinNumber], true);
}

void detachInterrupt(pin_size_t pinNumber)
{
    if (OUT_PIN_NUM <= pinNumber) {
        return;
    }

    pin_name_t name = out_pin_map[pinNumber];

    if (csi_pin_get_mux(name) != PIN_FUNC_GPIO) {
        pr_err("pin GPIO %d is not used as GPIO func\n", pinNumber);
        return;
    }

    if (pin[pinNumber].gpio == NULL) {
        return;
    }

    csi_gpio_pin_irq_enable(&pin[pinNumber], false);

    pin[pinNumber].callback = NULL;
    pin[pinNumber].arg = NULL;
}
