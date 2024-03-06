############
GPIO
############

Duo支持4组 GPIO (General Purpose Input/Output)，GPIO0 ~ GPIO3。每组GPIO 提供 32 个可编程的输入输出管脚。
当前支持的外引脚包含如下PIN编号:

* 0,1,2,(4,5,6,7,9,10,11,12),14,15,(16,17),19,20,21,22,24,25,26,27,(31,32)

  不建议使用括号里面的PIN


相关API使用请参考arduino官方文档:  

* digital io操作: pinMode_, digitalRead_ 和 digitalWrite_

  当前不支持OUTPUT_OPENDRAIN模式

.. _pinMode: https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/

.. _digitalRead: https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/

.. _digitalWrite: https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/

使用示例:
::

	#define TEST_PIN 2
	// the setup function runs once when you press reset or power the board
	void setup() {
	  pinMode(TEST_PIN, OUTPUT);
	}

	// the loop function runs over and over again forever
	void loop() {
	  digitalWrite(TEST_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
	  delay(500);                      // wait for a second
	  digitalWrite(TEST_PIN, LOW);   // turn the LED off by making the voltage LOW
	  delay(500);                      // wait for a second
	}
 
* extenal interrupts操作: attachInterrupt_, detachInterrupt_ 和 digitalPinToInterrupt_。
  
  当前支持LOW/HIGH/RISING/FALLING模式，并不支持CHANGE模式

.. _attachInterrupt: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

.. _detachInterrupt: https://www.arduino.cc/reference/en/language/functions/external-interrupts/detachinterrupt/

.. _digitalPinToInterrupt: https://www.arduino.cc/reference/en/language/functions/external-interrupts/digitalpintointerrupt/

使用示例:
::

	// connect ledPin and interruptPin
	const byte ledPin = 1;
	const byte interruptPin = 2;
	volatile byte state = HIGH;

	void setup() {
	  Serial.begin(115200);
	  pinMode(ledPin, OUTPUT);
	  pinMode(interruptPin, INPUT_PULLUP);
	  digitalWrite(ledPin, LOW);
	  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
	}

	void loop() {
	  state = !state;
	  digitalWrite(ledPin, state);
	  delay(1000);
	}

	void blink() {
	  Serial.printf("trigger interrupt\n\r");
	}
    
* 部分Advance io 操作: pulseIn_, pulseInLong_, shiftIn_, shiftOut_

.. _pulseIn:  https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/

.. _pulseInLong:  https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong/

.. _shiftIn:  https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftin/

.. _shiftOut:  https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout/

使用示例:
::

	/*
	 * HC-SR04 example sketch
	 *
	 * https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380
	 *
	 * by Isaac100
	 */

	const int trigPin = 19;
	const int echoPin = 20;
	volatile byte state = HIGH;

	float duration, distance;

	void setup() {
	  pinMode(trigPin, OUTPUT);
	  pinMode(echoPin, INPUT);
	  Serial.begin(115200);
	}

	void loop() {
	  digitalWrite(trigPin, LOW);
	  delayMicroseconds(2);
	  digitalWrite(trigPin, HIGH);
	  delayMicroseconds(10);
	  digitalWrite(trigPin, LOW);

	  duration = pulseIn(echoPin, HIGH);

	  distance = (duration*.0343)/2;
	  Serial.print("Distance: ");
	  Serial.println(distance);
	  delay(100);
	}
