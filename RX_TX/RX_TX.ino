#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX
volatile int left;
volatile int right;
volatile char c;
String json;

void setup(){
  left = 0;
  right = 0;
  c = 0;
  json = "";
  Serial.begin(115200);
  softSerial.begin(115200);
}

void loop(){
  if (softSerial.available() > 0) {
    c = char(softSerial.read());
    json = String(json) + String(c);
    if (c == '}') {
      DynamicJsonDocument doc(200); //声明一个JsonDocument对象
      deserializeJson(doc, json);
      JsonObject obj = doc.as<JsonObject>();
      int left = doc["left"];
      int right = doc["right"];
      if (left!=0&&right!=0)
      {
        Serial.print("left = ");
        Serial.print(left);
        Serial.print("right = ");
        Serial.println(right);
        }
      json = "";
      }
  }
}
