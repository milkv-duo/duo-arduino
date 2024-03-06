/*
 * Copyright (C) csitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include "hal_pin.h"
#include "csi_pin.h"

#define CHECK_PIN_NAME_VALID(PIN_NAME)                        \
    do                                                  \
    {                                                   \
        if (PIN_NAME >= PIN_NUM) \
        {                                               \
            pr_err("pin name %d is invalid\n", PIN_NAME);	\
            return (CSI_ERROR);                               \
        }                                               \
    } while (0)

extern const pin_reg_offset_t csi_pin_reg_offset[];
pin_name_t csi_gpio_to_pin(uint8_t gpio_bank, uint8_t pin_num, const csi_pinmap_t *pinmap)
{
    const csi_pinmap_t *map = pinmap;
    pin_name_t ret = (pin_name_t)0xFFFFFFFFU;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if ((map->idx == gpio_bank) && (map->channel == pin_num)) {
            ret = map->pin_name;
            break;
        }
        map++;
    }
    return ret;
}

uint32_t csi_pin_to_channel(pin_name_t pin_name, const csi_pinmap_t *pinmap)
{
    const csi_pinmap_t *map = pinmap;
    uint32_t ret = 0xFFFFFFFFU;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if (map->pin_name == pin_name) {
            ret = (uint32_t)map->channel;
            break;
        }
        map++;
    }
    return ret;
}

static uint16_t pin_to_mux_offset(pin_name_t pin_name)
{
    const pin_reg_offset_t *map = csi_pin_reg_offset;
    pin_name_t ret = (pin_name_t)0xFFFF;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if (map->pin_name == pin_name) {
            ret = map->mux_reg_offset;
            break;
        }
        map++;
    }

    return ret;
}

static uint16_t pin_to_cfg_offset(pin_name_t pin_name)
{
    const pin_reg_offset_t *map = csi_pin_reg_offset;
    pin_name_t ret = (pin_name_t)0xFFFF;

    while ((uint32_t)map->pin_name != 0xFFFFFFFFU) {
        if (map->pin_name == pin_name) {
            ret = map->cfg_reg_offset;
            break;
        }
        map++;
    }

    return ret;
}

csi_error_t csi_pin_set_mux(pin_name_t pin_name, pin_func_t pin_func)
{
    uint16_t offset;
    csi_error_t ret = CSI_OK;
    unsigned long mux_reg_base = PIN_MUX_BASE;

	CHECK_PIN_NAME_VALID(pin_name);

    offset = pin_to_mux_offset(pin_name);
    if (offset == 0xFFFF)
    {
        pr_err("pin name %d is unsupported to mux\n", pin_name);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

    pin_set_mux(mux_reg_base + offset, pin_func);

done:
    return ret;
}

pin_func_t csi_pin_get_mux(pin_name_t pin_name)
{
    uint16_t offset;
    csi_error_t ret = PIN_FUNC_INVALID;
    unsigned long mux_reg_base = PIN_MUX_BASE;

	CHECK_PIN_NAME_VALID(pin_name);

    offset = pin_to_mux_offset(pin_name);
    if (offset == 0xFFFF) {
        pr_warn("pin name %d is unsupported to mux\n", pin_name);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

    ret = pin_get_mux(mux_reg_base + offset);

done:
    return ret;
}

csi_error_t csi_pin_mode(pin_name_t pin_name, csi_pin_mode_t mode)
{
    uint16_t offset, value;
    csi_error_t ret = CSI_OK;
    unsigned long cfg_reg_base = PIN_CFG_BASE;

	CHECK_PIN_NAME_VALID(pin_name);

    offset = pin_to_cfg_offset(pin_name);
    if (offset == 0xFFFF) {
        pr_warn("pin name %d is unsupported to set mode\n", pin_name);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

    if ((pin_name == GPIO_ZQ) || (pin_name >= PWR_VBAT_DET && pin_name <= SD1_CLK))
        cfg_reg_base = PWR_PIN_CFG_BASE;

    switch (mode) {
    case GPIO_MODE_PULLNONE:
        value = PIN_MODE_NONE;
        break;
    case GPIO_MODE_PULLUP:
        value = PIN_MODE_PULLUP;
        break;
    case GPIO_MODE_PULLDOWN:
        value = PIN_MODE_PULLDOWN;
        break;
    default:
        pr_err("pin name %d is unsupported to set mode %d\n", pin_name, mode);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

    pin_set_mode(cfg_reg_base + offset, value);

done:
    return ret;
}

csi_error_t csi_pin_set_speed(pin_name_t pin_name, csi_pin_speed_t speed)
{
    uint16_t offset, value;
    csi_error_t ret = CSI_OK;
    unsigned long cfg_reg_base = PIN_CFG_BASE;

	CHECK_PIN_NAME_VALID(pin_name);

    offset = pin_to_cfg_offset(pin_name);
    if (offset == 0xFFFF) {
        pr_err("pin name %d is unsupported to set speed\n", pin_name);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

	if (pin_name >= PWR_VBAT_DET)
        cfg_reg_base = PWR_PIN_CFG_BASE;

    switch (speed) {
    case CSI_PIN_SPEED_SLOW:
        value = PIN_SPEED_SLOW;
        break;
    case CSI_PIN_SPEED_FAST:
        value = PIN_SPEED_FAST;
        break;
    default:
        pr_err("pin name %d is unsupported to set speed %d\n", pin_name, speed);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

    pin_set_speed(cfg_reg_base + offset, value);

done:
    return ret;
}

csi_error_t csi_pin_set_drive(pin_name_t pin_name, csi_pin_drive_t drive)
{
    uint16_t offset, value;
    csi_error_t ret = CSI_OK;
    unsigned long cfg_reg_base = PIN_CFG_BASE;

	CHECK_PIN_NAME_VALID(pin_name);

    offset = pin_to_cfg_offset(pin_name);
    if (offset == 0xFFFF) {
        pr_err("pin name %d is unsupported to set drive\n", pin_name);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

	if (pin_name >= PWR_VBAT_DET)
        cfg_reg_base = PWR_PIN_CFG_BASE;

    switch (drive) {
    case PIN_DRIVE_STRENGTH0:
        value = PIN_DRIVE_STRENGTH_0;
        break;
    case PIN_DRIVE_STRENGTH1:
        value = PIN_DRIVE_STRENGTH_1;
        break;
    default:
        pr_err("pin name %d is unsupported to set drive %d\n", pin_name, drive);
        ret = CSI_UNSUPPORTED;
        goto done;
    }

    pin_set_drive(cfg_reg_base + offset, value);

done:
    return ret;
}

csi_error_t csi_pin_speed(pin_name_t pin_name, cvi_pin_speed_t speed)
{
    csi_error_t ret;
    switch (speed) {
    case PIN_SPEED_LV0:
        ret = csi_pin_set_speed(pin_name, CSI_PIN_SPEED_SLOW);
        break;
    case PIN_SPEED_LV1:
        ret = csi_pin_set_speed(pin_name, CSI_PIN_SPEED_FAST);
        break;
    default:
        pr_err("pin name %d is unsupported to set speed %d\n", pin_name, speed);
        ret = CSI_UNSUPPORTED;
    }
	return ret;
}

csi_error_t csi_pin_drive(pin_name_t pin_name, cvi_pin_drive_t drive)
{
    csi_error_t ret;
    switch (drive) {
    case PIN_DRIVE_LV0:
        ret = csi_pin_set_drive(pin_name, PIN_DRIVE_STRENGTH0);
        break;
    case PIN_DRIVE_LV1:
        ret = csi_pin_set_drive(pin_name, PIN_DRIVE_STRENGTH1);
        break;
    default:
        pr_err("pin name %d is unsupported to set drive %d\n", pin_name, drive);
        ret = CSI_UNSUPPORTED;
    }
    return ret;
}
