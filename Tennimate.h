/**********************************************************************
程序名称/Program name      : Tennimate.h
团队/Team                 : Tennimate
作者/Author               : 陈力希
日期/Date                 : 2020.4.15

-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    

***********************************************************************/

#ifndef TENNIMATE_H_
#define TENNIMATE_H_
#include "Arduino.h"
#include <Servo.h>


// Tennimate类
class Tennimate{
  public:
  // 提供车轮电机方向设置参数，车轮电机速度参数， 测距传感器引脚参数以及舵机控制引脚参数的构造函数
    Tennimate(int ENA_pin, int In1_pin, int In2_pin, int ENB_pin, int In3_pin, int In4_pin, byte M_Speed,int trigPin, int echoPin, int servoPin);
    
    void forward();       // 前进
    void backward();      // 后退
    void turnL();       // 左转
    void turnR();         // 右转
    void forwardL();      // 左前
    void forwardR();      // 右前
    void backwardL();     // 左后
    void backwardR();     // 右后 
    void stop();                // 停车
    int getDistance();          // 获取距离传感器读数

    void setHeadPos(int pos);   // 设定舵机角度
    int getHeadPos();           // 获取舵机角度

    void headServoIni();      // 舵机初始化
      
  private:

    Servo headServo;      // 建立头部舵机对象
    int headServoPin;       // 舵机控制引脚
    byte motorSpeed;     //电机速度

    int ENA;     //L298N上的ENA引脚
    int In1;     //L298N上的In1引脚
    int In2;
    int ENB;
    int In3;
    int In4;

    int hcTrig;  // 超声测距传感器Trig引脚
    int hcEcho;  // 超声测距传感器Echo引脚
    long duration, cm;   // 超声测距传感器用变量
};
#endif  
