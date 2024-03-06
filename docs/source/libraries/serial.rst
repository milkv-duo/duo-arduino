############
UART
############

UART（Universal Asynchronous Receiver Transmitter）是一个异步串行的通信接口，主要功能是将来自外围设备的资料进行串并转换之后传入内部总线，以及将资料进行并串转换之后输出到外部设备。UART 的主要功能是和外部芯片的UART进行对接，从而实现两芯片间的通信。

Duo提供5个UART控制器,对应于5个全局Serial实例: Serial0~5,它们对应的默认PIN如下：

+--------+--------+----------+
|实例    |   RX   |    TX    |	
+--------+--------+----------+
|Serial0 |   17   |    16    |
+--------+--------+----------+
|Serial1 |   2    |    1     |
+--------+--------+----------+
|Serial2 |   7    |    6     |
+--------+--------+----------+
|Serial3 |   7    |    6     |
+--------+--------+----------+
|Serial4 |   5    |    4     |
+--------+--------+----------+

默认的调试接口: Serial => Serial3。

对于RX/TX重叠的Serial可以交替使用（使用之前通过begin接口使能）。

相关API使用请参考arduino官方文档:  Serial_。

.. _Serial: https://www.arduino.cc/reference/en/language/functions/communication/serial/


使用示例:
::

	void setup() {
	  Serial2.begin(115200);
	}

	bool select = true;
	void loop() {

	  //read from port 2, send to port 3:
	  if (select && Serial2.available()) {
	      int inByte = Serial2.read();
	      Serial3.begin(115200);
	      Serial3.printf("From 2: %c \n", inByte);
	      int i = 0;
	      while(Serial2.available()) {
			inByte = Serial2.read();
			Serial3.printf(" -- %d: ", i++);
			Serial3.write(inByte);
	      }
	      Serial3.printf("2 Msg end\n");
	      select = false;
	  }

	  //read from port 3, send to port 2:
	  if (!select && Serial3.available()) {
	      int inByte = Serial3.read();
	      Serial2.begin(115200);
	      Serial2.printf("From 3: %c \n", inByte);
	      int i = 0;
	      while(Serial3.available()) {
			inByte = Serial3.read();
			Serial2.printf(" -- %d: ", i++);
			Serial2.write(inByte);
	      }
	      Serial2.printf("3 Msg end\n");
	      select = true;
	  }
	}

 
	
