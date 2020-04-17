#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire,OLED_RESET);
volatile int cx;
volatile int cy;
volatile char c;
String json;

void setup(){
  cx = 0;
  cy = 0;
  c = 0;
  json = "";
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.setTextColor(WHITE);//开像素点发光
}

void loop(){
  display.clearDisplay();//清屏
  if (Serial.available() > 0) {
    c = char(Serial.read());
    json = String(json) + String(c);
    if (c == '}') {
      //display.setTextSize(1);//设置字体大小  
      //display.setCursor(40, 40);//设置显示位置
      //display.println(json);
      //display.display(); // 开显示
      DynamicJsonDocument doc(200); //声明一个JsonDocument对象
      deserializeJson(doc, json);
      JsonObject obj = doc.as<JsonObject>();
      int cx = doc["cx"];
      int cy = doc["cy"];
      if (cx!=0&&cy!=0)
      {
        //Serial.print("cx = ");
        //Serial.print(cx);
        //Serial.print("cy = ");
        //Serial.println(cy);
        display.setTextSize(1);//设置字体大小  
        display.setCursor(15, 30);//设置显示位置
        display.print(json);
        display.display(); // 开显示
        delay(100);
        }
      json = "";
      }
  }
}
