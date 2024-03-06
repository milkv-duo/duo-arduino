/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <inttypes.h>
#include "Stream.h"
#include "csi_uart.h"

#define SERIAL_PARITY_EVEN    (UART_PARITY_EVEN << 5)
#define SERIAL_PARITY_ODD     (UART_PARITY_ODD << 5)
#define SERIAL_PARITY_NONE    (UART_PARITY_NONE << 5)
#define SERIAL_PARITY(config) (csi_uart_parity_t)((config >> 5) & 0x3)

#define SERIAL_STOP_BIT_1     (UART_STOP_BITS_1 << 3)
#define SERIAL_STOP_BIT_1_5   (UART_STOP_BITS_1_5 << 3)
#define SERIAL_STOP_BIT_2     (UART_STOP_BITS_2 << 3)
#define SERIAL_STOP(config)   (csi_uart_stop_bits_t)((config >> 3) & 3)

#define SERIAL_DATA_5         (UART_DATA_BITS_5)
#define SERIAL_DATA_6         (UART_DATA_BITS_6)
#define SERIAL_DATA_7         (UART_DATA_BITS_7)
#define SERIAL_DATA_8         (UART_DATA_BITS_8)
#define SERIAL_DATA(config)   (csi_uart_data_bits_t)(config & 0x7)

#define SERIAL_5N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_5)
#define SERIAL_6N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_6)
#define SERIAL_7N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_7)
#define SERIAL_8N1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_8)
#define SERIAL_5N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_5)
#define SERIAL_6N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_6)
#define SERIAL_7N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_7)
#define SERIAL_8N2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_8)
#define SERIAL_5E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_5)
#define SERIAL_6E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_6)
#define SERIAL_7E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_7)
#define SERIAL_8E1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_8)
#define SERIAL_5E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_5)
#define SERIAL_6E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_6)
#define SERIAL_7E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_7)
#define SERIAL_8E2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_8)
#define SERIAL_5O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_5)
#define SERIAL_6O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_6)
#define SERIAL_7O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_7)
#define SERIAL_8O1           (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_8)
#define SERIAL_5O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_5)
#define SERIAL_6O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_6)
#define SERIAL_7O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_7)
#define SERIAL_8O2           (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_8)

class HardwareSerial : public Stream
{
  public:
    HardwareSerial(int uartNum, uint8_t rxPin, uint8_t txPin);
    ~HardwareSerial();

    void begin(unsigned long baud, uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1);
    void end();

    int available(void);

    // rtt serial driver not support.
    int peek(void);
    void flush(void) {}

    int read(void);
    size_t read(uint8_t *buffer, size_t size);
    inline size_t read(char * buffer, size_t size) {
        return read((uint8_t*) buffer, size);
    }

    void setTimeout(unsigned long timeout) { _timeout = timeout; }
    unsigned long getTimeout(void) { return _timeout; }

    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    int availableForWrite() { return 0; }

    operator bool() const;

  protected:
    int _uartNum;
    unsigned long _timeout;
    csi_uart_t _uart;
    uint8_t _data;
    bool _data_valid;

    uint8_t _rxPin, _txPin;
};

#define Serial Serial3

#if SERIAL_INTERFACES_COUNT > 0
extern HardwareSerial Serial0;
#endif
#if SERIAL_INTERFACES_COUNT > 1
extern HardwareSerial Serial1;
#endif
#if SERIAL_INTERFACES_COUNT > 2
extern HardwareSerial Serial2;
#endif
#if SERIAL_INTERFACES_COUNT > 3
extern HardwareSerial Serial3;
#endif

#if SERIAL_INTERFACES_COUNT > 4
extern HardwareSerial Serial4;
#endif

// XXX: Are we keeping the serialEvent API?
extern void serialEventRun(void) __attribute__((weak));
