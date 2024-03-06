/*
 * Copyright (C) Cvitek Co., Ltd. 2019-2020. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csi_pin.h"
#include "HardwareSerial.h"
#include "Arduino.h"

#if SERIAL_INTERFACES_COUNT > 0
HardwareSerial Serial0(0, PIN_UART0_RX, PIN_UART0_TX);
#endif
#if SERIAL_INTERFACES_COUNT > 1
HardwareSerial Serial1(1, PIN_UART1_RX, PIN_UART1_TX);
#endif
#if SERIAL_INTERFACES_COUNT > 2
HardwareSerial Serial2(2, PIN_UART2_RX, PIN_UART2_TX);
#endif
#if SERIAL_INTERFACES_COUNT > 3
HardwareSerial Serial3(3, PIN_UART3_RX, PIN_UART3_TX);
#endif
#if SERIAL_INTERFACES_COUNT > 4
HardwareSerial Serial4(4, PIN_UART4_RX, PIN_UART4_TX);
#endif

extern dev_pin_map_t uart_rx_map[];
extern dev_pin_map_t uart_tx_map[];

extern "C" void * __dso_handle = 0 ;

HardwareSerial::HardwareSerial(int uartNum, uint8_t rx, uint8_t tx) :
    _uartNum(uartNum),
    _timeout(1000),
    _data_valid(false),
    _rxPin(rx),
    _txPin(tx)
{
}

HardwareSerial::~HardwareSerial()
{
    end();
}

void HardwareSerial::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin)
{
    if (rxPin < 0) {
        rxPin = _rxPin;
    }

    if (txPin < 0) {
        txPin = _txPin;
    }

    const dev_pin_map_t* rx_pin = target_pin_number_to_dev(rxPin, uart_rx_map, _uartNum);
    const dev_pin_map_t* tx_pin = target_pin_number_to_dev(txPin, uart_tx_map, _uartNum);

    if (rx_pin == NULL || tx_pin == NULL) {
        pr_err("pin GPIO %d or %d are not used as Uart func\n", rxPin, txPin);
        return;
    }

    if (csi_pin_set_mux(rx_pin->name, rx_pin->func)) {
        pr_err("pin GPIO %d fails to config as Uart_rx func\n", rxPin);
        return;
    }

    if (csi_pin_set_mux(tx_pin->name, tx_pin->func)) {
        pr_err("pin GPIO %d fails to config as Uart_tx func\n", txPin);
        return;
    }

    csi_uart_init(&_uart, _uartNum);

    _rxPin = rxPin;
    _txPin = txPin;

    csi_uart_format(&_uart, SERIAL_DATA(config), SERIAL_PARITY(config), SERIAL_STOP(config));
    csi_uart_baud(&_uart, baud);
}

void HardwareSerial::end()
{
    if (_uart.dev.reg_base) {
        csi_uart_uninit(&_uart);
    }
}

int HardwareSerial::available(void)
{
    if (_data_valid) {
        return 1;
    }

    int32_t ret = csi_uart_receive(&_uart, &_data, 1, 0);
    if (ret) {
        _data_valid = true;
        return 1;
    }

    return 0;
}

int HardwareSerial::read(void)
{
    if (_data_valid) {
        _data_valid = false;
        return _data;
    }

    int32_t ret = csi_uart_receive(&_uart, &_data, 1, _timeout);
    if (ret) {
        return _data;
    } else {
        while (1);
        return -1;
    }
}

int HardwareSerial::peek(void)
{
    if (_data_valid) {
        return _data;
    }

    int32_t ret = csi_uart_receive(&_uart, &_data, 1, _timeout);
    if (ret) {
        _data_valid = true;
        return _data;
    } else {
        return -1;
    }
}

// read characters into buffer
// terminates if size characters have been read, or no further are pending
// returns the number of characters placed in the buffer
// the buffer is NOT null terminated.
size_t HardwareSerial::read(uint8_t *buffer, size_t size)
{
    if (size == 0) {
        return 0;
    }

    if (_data_valid) {
        buffer[0] = _data;
        _data_valid = false;
        buffer++;
        size--;
    }
    if (size > 0) {
        return csi_uart_receive(&_uart, buffer, size, _timeout) + 1;
    } else {
        return 1;
    }
}

size_t HardwareSerial::write(uint8_t c)
{
    csi_uart_putc(&_uart, c);
    return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
    return csi_uart_send(&_uart, buffer, size, _timeout);
}

HardwareSerial::operator bool() const
{
    return _uart.dev.reg_base != 0;
}

extern "C"
{
    char _SerialPutChar(char c)
    {
        if (Serial)
        {
            Serial.write(c);
            return (c & 0xFF);
        }

        return 0;
    }
}
