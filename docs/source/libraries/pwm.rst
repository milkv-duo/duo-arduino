############
PWM
############

Duo提供1组4路独立的PWM信号输出。4路PWM皆可独立操作:

* 内部有30-bit计数器，输出周期和高/低电平拍数皆可配置

* 支持最高50MHz (100MHz/2) 或 74.25MHz (148.5MHz/2) 输出，最低约0.093Hz (100MHz/(2^30-1)) 或 0.138Hz (148.5MHz/(2^32-1))

* 支持连续输出(PWMMODE = 0)和固定脉冲个数输出(PWMMODE = 1)两种模式


目前支持的PWM对外引脚包括如下PIN编号:

*  4, 5, (6, 7), 9, 10, 11, (12, 16, 17, 31), 32

  不建议使用括号里面的PIN


相关API使用请参考arduino官方文档:  

* 部分analog io操作: analogWrite_ 和 analogWriteResolution_

.. _analogWrite: https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/

.. _analogWriteResolution: https://www.arduino.cc/reference/en/language/functions/analog-io/analogwriteresolution/

使用示例:
::

	// sg90 5V
	void setup() {
	  analogWriteResolution(14);  // 20ms
	}

	void loop() {
	  analogWrite(4, 500);
	  delay(1000);
	  analogWrite(4, 1000);
	  delay(1000);
	  analogWrite(4, 1500);
	  delay(1000);
	  analogWrite(4, 2000);
	  delay(1000);
	  analogWrite(4, 2500);
	  delay(1000);
	}
 

    
* 部分Advance io 操作: tone_ 和 noTone_

.. _tone: https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/

.. _noTone: https://www.arduino.cc/reference/en/language/functions/advanced-io/notone/

使用示例:
::

	void setup() {
	  tone(4, 1000);  // 1K Hz
	  tone(5, 1000);  // will be ignored
	  delay(2000);
	  noTone(4);
	  tone(5, 1000, 1000000);  // duration 1s
	  delay(2000);
	  noTone(5);
	}

	void loop() {
	}
	
