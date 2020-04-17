

#include <Wire.h>
#define BAUD_RATE 19200
#define CHAR_BUF 128

void setup() {
  Serial.begin(BAUD_RATE);
  Wire.begin();
  delay(1000); // 给OpenMV一个启动的时间
}

void loop() {
  int32_t temp = 0;
  char buff[CHAR_BUF] = {0};

  Wire.requestFrom(0x12, 2);
  if (Wire.available() == 2) { // got length?

    temp = Wire.read() | (Wire.read() << 8);
    delay(1); // Give some setup time...

    Wire.requestFrom(0x12, temp);
    if (Wire.available() == temp) { // got full message?

      temp = 0;
      while (Wire.available()) buff[temp++] = Wire.read();

    } else {
      while (Wire.available()) Wire.read(); // Toss garbage bytes.
    }
  } else {
    while (Wire.available()) Wire.read(); // Toss garbage bytes.
  }

  Serial.print(buff);
  delay(10); // Don't loop to quickly.
}
