############
LED示例
############


示例代码：
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
