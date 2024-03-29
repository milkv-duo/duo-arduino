/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

#ifndef _DRV_GPIO_PIN_H_
#define _DRV_GPIO_PIN_H_

#include <stdint.h>
#include <stdbool.h>
#include "csi_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct   csi_gpio_pin_t
 * \brief    GPIO PIN control block
 */

typedef struct csi_gpio_pin csi_gpio_pin_t;
struct csi_gpio_pin {
    csi_gpio_t      *gpio;
    uint32_t        pin_idx;
    void (*callback)(csi_gpio_pin_t *pin, void *arg);
    void            *arg;
};

typedef struct {
    csi_gpio_t     gpio;
    csi_gpio_pin_t *pin[32];
} csi_gpio_manage_t;

/**
  \brief       Initialize GPIO pin handle
  \param[in]   pin         GPIO pin handle
  \param[in]   pin_name    GPIO pin name
  \return      Error code
*/
csi_error_t csi_gpio_pin_init(csi_gpio_pin_t *pin, pin_name_t pin_name);

/**
  \brief       De-initialize GPIO pin
  \param[in]   pin         GPIO pin handle
  \return      None
*/
void csi_gpio_pin_uninit(csi_gpio_pin_t *pin);

/**
  \brief       Attach the interrupt callback to the GPIO pin
  \param[in]   pin         GPIO pin handle
  \param[in]   callback    Callback function
  \param[in]   arg         User param passed to callback
  \return      Error code
*/
csi_error_t csi_gpio_pin_attach_callback(csi_gpio_pin_t *pin, void *callback, void *arg);

/**
  \brief       Config pin direction
  \param[in]   pin         GPIO pin handle
  \param[in]   dir         \ref csi_gpio_dir_t
  \return      Error code
*/
csi_error_t csi_gpio_pin_dir(csi_gpio_pin_t *pin, csi_gpio_dir_t dir);

/**
  \brief       Config pin mode
  \param[in]   pin        GPIO pin handle
  \param[in]   mode        \ref csi_gpio_mode_t
  \return      Error code
*/
csi_error_t csi_gpio_pin_mode(csi_gpio_pin_t *pin, csi_gpio_mode_t mode);

/**
  \brief       Config pin irq params
  \param[in]   pin        GPIO pin handle
  \param[in]   mode        Interrupt trigger mode \ref csi_gpio_irq_mode_t
  \return      Error code
*/
csi_error_t csi_gpio_pin_irq_mode(csi_gpio_pin_t *pin, csi_gpio_irq_mode_t mode);

/**
  \brief       Enable or disable gpio pin interrupt
  \param[in]   pin        GPIO pin handle
  \param[in]   enable      0:disable  1:enable
  \return      Error code
*/
csi_error_t csi_gpio_pin_irq_enable(csi_gpio_pin_t *pin, bool enable);

/**
  \brief       Set debounce of pin when pin configed as input
  \param[in]   pin        GPIO pin handle
  \param[in]   enbale      0: disable   1:enable
  \return      Error code
*/
csi_error_t csi_gpio_pin_debounce(csi_gpio_pin_t *pin, bool enable);

/**
  \brief       Set one or zero to specified pin
  \param[in]   pin        GPIO pin handle
  \param[in]   value       Value to be set \ref csi_gpio_pin_state_t
  \return      None
*/
void csi_gpio_pin_write(csi_gpio_pin_t *pin, csi_gpio_pin_state_t value);

/**
  \brief       Toggle output pin value,ex.if previous value is 1, then output 0
  \param[in]   pin        GPIO pin handle
  \return      None
*/
void csi_gpio_pin_toggle(csi_gpio_pin_t *pin);

/**
  \brief       Get the value of specified GPIO pin
  \param[in]   pin        GPIO port handle
  \return      gpio pin state, \ref csi_gpio_pin_state_t
*/
csi_gpio_pin_state_t csi_gpio_pin_read(csi_gpio_pin_t *pin);

#ifdef __cplusplus
}
#endif

#endif  /* _GPIO_PIN_H_*/
