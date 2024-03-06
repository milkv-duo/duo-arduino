#ifndef VARIANT_H
#define VARIANT_H
#include "variant_pins.h"
#include "variant_soc.h"

#define DEBUG_SERIAL            Serial3
#define DEBUG_UART_BASE         DW_UART3_BASE

#define WIRE_INTERFACES_COUNT       4
#define OUT_PIN_NUM                 41

#define digitalPinToInterrupt(p)    (((p) < CONFIG_GPIO_NUM * 32) ? (p) : -1)

#define SERIAL_INTERFACES_COUNT 5

#define Wire Wire0
#endif
