/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "wiring_digital.h"
#include "csi_gpio_pin.h"
#include "csi_pin.h"

#ifdef __cplusplus
 extern "C" {
#endif

extern pin_name_t out_pin_map[];
extern pin_func_t out_pin_func_map[];
csi_gpio_pin_t pin[OUT_PIN_NUM];

void pinMode(uint8_t pinNumber, uint8_t pinMode)
{
    if (OUT_PIN_NUM <= pinNumber) {
        return;
    }

    pin_name_t name = out_pin_map[pinNumber];

    if (name == 0xFF) {
        pr_err("pin %d cannot be used as general GPIO\n", pinNumber);
        return;
    }

    if (csi_pin_get_mux(name) != out_pin_func_map[pinNumber]) {
        if (csi_pin_set_mux(name, out_pin_func_map[pinNumber])) {
            pr_err("pin %d cannot be configed as general GPIO\n", pinNumber);
            return;
        }
    }

    if (pin[pinNumber].gpio == NULL) {
        csi_error_t ret_status = csi_gpio_pin_init(&pin[pinNumber], name);
        if (ret_status != CSI_OK) {
            pr_err("GPIO pin %d init failed\n", pinNumber);
            return;
        }
    }

    csi_gpio_mode_t mode = GPIO_MODE_PULLNONE;
    csi_gpio_dir_t dir = GPIO_DIRECTION_INPUT;

    switch (pinMode) {
    case INPUT:
        break;
    case INPUT_PULLUP:
        mode = GPIO_MODE_PULLUP;
        break;
    case INPUT_PULLDOWN:
        mode = GPIO_MODE_PULLDOWN;
        break;
    case OUTPUT:
        dir = GPIO_DIRECTION_OUTPUT;
        break;
    case OUTPUT_OPENDRAIN:
        dir = GPIO_DIRECTION_OUTPUT;
        mode = GPIO_MODE_OPEN_DRAIN;
        break;
    default:
        break;
    }

    csi_gpio_pin_dir(&pin[pinNumber], dir);
    csi_pin_mode(name, mode);
}

void digitalWrite(uint8_t pinNumber, uint8_t status)
{
    if (OUT_PIN_NUM <= pinNumber) {
        return;
    }

    pin_name_t name = out_pin_map[pinNumber];

    if (csi_pin_get_mux(name) != out_pin_func_map[pinNumber]) {
        pr_err("pin GPIO %d is not used as GPIO func\n", pinNumber);
        return;
    }

    if (pin[pinNumber].gpio == NULL) {
        return;
    }

    csi_gpio_pin_write(&pin[pinNumber], (LOW == status) ? GPIO_PIN_LOW : GPIO_PIN_HIGH);

}

int digitalRead(uint8_t pinNumber)
{
    if (OUT_PIN_NUM <= pinNumber) {
        return;
    }

    pin_name_t name = out_pin_map[pinNumber];

    if (csi_pin_get_mux(name) != out_pin_func_map[pinNumber]) {
        pr_err("pin GPIO %d is not used as GPIO func\n", pinNumber);
        return;
    }

    if (pin[pinNumber].gpio == NULL) {
        return;
    }

    return csi_gpio_pin_read(&pin[pinNumber]) == GPIO_PIN_LOW ? LOW : HIGH;
}

#ifdef __cplusplus
}
#endif
