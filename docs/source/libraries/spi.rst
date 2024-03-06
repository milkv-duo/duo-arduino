############
SPI
############

* 对外开放两个spi总线，且SPI0默认用作lcd驱动。

* SPI支持全引脚映射。

* 暂不支持从机。

使用请参考arduino官方文档： SPI_。


.. _SPI: https://www.arduino.cc/reference/en/language/functions/communication/spi/

使用示例：
::

    #include <SPI.h>

    char in_byte = 3;

    void setup() {
      // put your setup code here, to run once:
      Serial.begin(115200);
      SPI.begin();
    }

    void loop() {
      // put your main code here, to run repeatedly:
      SPI.beginTransaction(SPISettings());
      char out_byte = SPI.transfer(in_byte);        // spi loop back
      SPI.endTransaction();
      Serial.printf("receive %x\n\r", out_byte);
    }
