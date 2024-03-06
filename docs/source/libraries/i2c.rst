############
Wire/I2C
############

该库用于与 `I2C/TWI` 设备进行通信。

* Duo支持5个I2C总线。

* 支持全引脚映射。


使用请参考arduino官方文档： Wire_。

.. _Wire: https://www.arduino.cc/reference/en/language/functions/communication/wire/

使用示例：
::

    #include <Wire.h>

    void receive(int a) {
      Serial.printf("receive %d bytes\n\r", a);
      while(a--) {
        Serial.printf("%d \n\r", Wire1.read());
      }
    }

    void setup() {
      Serial.begin(115200);
      Wire1.begin(0x50);
      Wire1.onReceive(receive);
      Wire.begin();
      Serial.printf("test slave\n\r");
      Wire1.print();
    }

    byte val = 0;

    void loop() {
      // put your main code here, to run repeatedly:
      Wire.beginTransmission(0x50);
      Serial.printf("send %d \n\r", ++val);
      Wire.write(val);         // Sends value byte
      Wire.endTransmission();      // Stop transmitting
      Wire1.onService();
    }
