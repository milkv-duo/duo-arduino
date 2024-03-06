############
ADC
############

SARADC为模拟信号数字转换控制器。

* Duo最多支持3个独立通道，但是当前仅对外提供通道1,对应于PIN_ADC1（编号31）

* 支持12bit采样精度

使用请参考arduino官方文档: analogRead_ 和 analogReadResolution_: analogRead默认采用10bit采样，12bit硬件采样数据会映射到该位宽, 可通过analogReadResolution任意修改默认位宽，。


.. _analogRead: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/

.. _analogReadResolution: https://www.arduino.cc/reference/en/language/functions/analog-io/analogreadresolution/


使用示例：
::

    int analogPin = PIN_ADC1;

    int val = 0;

    void setup() {
      Serial.begin(115200);           //  setup serial
    }

    void loop() {
      val = analogRead(analogPin);  // read the input pin
      Serial.println(val);          // debug value
    }
