/**********************************************************************
程序名称/Program name      : Tennimate.h
团队/Team                 : Tennimate
作者/Author               : 陈力希
日期/Date                 : 2020.4.15

-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    

***********************************************************************/

#include "Tennimate.h"

// 提供车轮电机方向设置参数，车轮电机速度参数， 测距传感器引脚参数以及舵机控制引脚参数的构造函数
Tennimate::Tennimate(int ENA_pin, int In1_pin, int In2_pin, int ENB_pin, int In3_pin, int In4_pin, byte M_Speed,int trigPin, int echoPin, int servoPin)
{
  
  headServoPin = servoPin;
  hcTrig = trigPin;
  hcEcho = echoPin;
  
  motorSpeed = M_Speed;

  ENA = ENA_pin;     //L298N上的ENA引脚
  In1 = In1_pin;     //L298N上的In1引脚
  In2 = In2_pin;
  ENB = ENB_pin;
  In3 = In3;
  In4 = In4;

  pinMode(ENA,OUTPUT);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(In3,OUTPUT);
  pinMode(In4,OUTPUT);

  pinMode(hcTrig, OUTPUT);
  pinMode(hcEcho, INPUT);  
}

// 初始化舵机
void Tennimate::headServoIni(){
  headServo.attach(headServoPin);               
}

// 设置舵机位置
void Tennimate::setHeadPos(int pos){
    headServo.write(pos);               
}

// 获取舵机位置
int Tennimate::getHeadPos(){
    return headServo.read();               
}

// 读取传感器距离读数（单位为厘米）
int Tennimate::getDistance(){
   
  digitalWrite(hcTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(hcTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(hcTrig, LOW);

  duration = pulseIn(hcEcho, HIGH);
  cm = (duration/2) / 29.1;
  
  return cm;
}


// 前进
void Tennimate::forward(){  
  digitalWrite(In1,HIGH); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,HIGH); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,motorSpeed);
   
}

// 后退
void Tennimate::backward(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,HIGH);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,HIGH);  //给低电平
  analogWrite(ENB,motorSpeed);  
}

// 左转
void Tennimate::turnL(){
  digitalWrite(In1,HIGH); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,HIGH);  //给低电平
  analogWrite(ENB,motorSpeed); 
}

// 右转
void Tennimate::turnR(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,HIGH);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,HIGH); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,motorSpeed);       
}

// 左前
void Tennimate::forwardL(){
  digitalWrite(In1,HIGH); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,LOW);  //给低电平   
  analogWrite(ENB,0);
}

// 右前
void Tennimate::forwardR(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,0);
    
  digitalWrite(In3,HIGH); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,motorSpeed); 
}

// 左后
void Tennimate::backwardL(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,HIGH);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,0);
}

// 右后
void Tennimate::backwardR(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,0);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,HIGH);  //给低电平
  analogWrite(ENB,motorSpeed);  
}

// 停止
void Tennimate::stop(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,0);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,0);   
}
