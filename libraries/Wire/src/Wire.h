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

#include "Arduino.h"
#include <inttypes.h>
#include "Stream.h"
#include "csi_iic.h"
#include "RingBuffer.h"

// namespace arduino {

#define HARDWARE_I2C_BUFF_LEN   (64)

typedef void(*user_onRequest)(void);
typedef void(*user_onReceive)(uint8_t*, int);

class TwoWire : public Stream
{
  public:
    TwoWire(uint8_t idx, uint8_t pinSDA, uint8_t pinSCL);
    void begin(csi_iic_addr_mode_t addr_mode = IIC_ADDRESS_7BIT);
    void begin(uint16_t address, csi_iic_addr_mode_t addr_mode = IIC_ADDRESS_7BIT);

    void end();
    void setClock(uint32_t);

    void beginTransmission(uint16_t);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);

    size_t requestFrom(uint16_t address, size_t quantity, bool stopBit);
    size_t requestFrom(uint16_t address, size_t quantity);

    size_t write(uint8_t data);
    size_t write(const uint8_t * data, size_t quantity);

    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void(*)(int));
    void onRequest(void(*)(void));

    void print(void);

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    void setWireTimeout(uint64_t timeout, bool reset_on_timeout) { _timeout = timeout / 1000; }
    using Print::write;

    void onService(void);

  private:
    csi_iic_t _iic;
    uint8_t _uc_pinSDA;
    uint8_t _uc_pinSCL;
    uint8_t _idx;
    uint16_t _addr;
    uint64_t _timeout;
    bool _master;

    bool transmissionBegun;

    // RX Buffer
    RingBufferN<256> rxBuffer;

    //TX buffer
    RingBufferN<256> txBuffer;
    uint8_t txAddress;

    // Callback user functions
    void (*onRequestCallback)(void);
    void (*onReceiveCallback)(int);

    // TWI clock frequency
    static const uint32_t TWI_CLOCK = 100000;
};

#if WIRE_INTERFACES_COUNT > 0
extern TwoWire Wire0;
#endif
#if WIRE_INTERFACES_COUNT > 1
extern TwoWire Wire1;
#endif
#if WIRE_INTERFACES_COUNT > 2
extern TwoWire Wire2;
#endif
#if WIRE_INTERFACES_COUNT > 3
extern TwoWire Wire3;
#endif
#if WIRE_INTERFACES_COUNT > 4
extern TwoWire Wire4;
#endif
