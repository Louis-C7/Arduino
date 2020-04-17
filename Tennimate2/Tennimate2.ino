/**********************************************************************
程序名称/Program name      : Tennimate.h
团队/Team                 : Tennimate
作者/Author               : 陈力希
日期/Date                 : 2020.4.15

-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    

***********************************************************************/
/*-----------------------------------------------------------------------
电路连接：
  Arduino AFMOTOR SERVO-2引脚（Arduino引脚9） 连接 HC06-TX
  Arduino AFMOTOR SERVO-1引脚（Arduino引脚10）连接 舵机信号线
  Arduino A0引脚 连接 HCSR04的 trig 引脚
  Arduino A1引脚 连接 HCSR04的 echo 引脚
  Arduino 10引脚 连接 头部舵机 控制信号线
***********************************************************************/
#include <Servo.h>
int In1 = 12; // 定义uno的pin 5 向 input1 输出 
int In2 = 13; // 定义uno的pin 6 向 input2 输出
int ENA = 11; // 定义uno的pin 10 向 输出A使能端输出
int In3 = 7; // 定义uno的pin 5 向 input1 输出 
int In4 = 8; // 定义uno的pin 6 向 input2 输出
int ENB = 9; // 定义uno的pin 10 向 输出A使能端输出
int hcTrig = 2;
int hcEcho = 3;
 long duration, cm;
#define DIST_THRESHOLD 35        // 避障距离阈值（cm）

#define TURN_LEFT_90   900        // 左转90度延迟参数
#define TURN_RIGHT_90   1000      // 右转90度延迟参数
#define TURN_BACK   1600          // 掉头延迟参数

#define motorSpeed  100       //电机速度

/*Servo headServo;      // 建立头部舵机对象
int headServoPin = 10;       // 舵机控制引脚*/

                                                                                                
                           
void setup() {

  /*headServo.attach(headServoPin);    //头部舵机初始化
  headServo.write(90);   // 系统启动时将头部设置为90位置*/
  
  pinMode(ENA,OUTPUT);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(In3,OUTPUT);
  pinMode(In4,OUTPUT);

  pinMode(hcTrig, OUTPUT);
  pinMode(hcEcho, INPUT);  
}

void loop() {
forward();
}

void forward(){  
  digitalWrite(In1,HIGH); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,motorSpeed);
  analogWrite(ENB,motorSpeed);
  digitalWrite(In3,HIGH); //给高电平
  digitalWrite(In4,LOW);  //给低电平

}

// 后退
void backward(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,HIGH);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,HIGH);  //给低电平
  analogWrite(ENB,motorSpeed);  
}

// 左转
void turnL(){
  digitalWrite(In1,HIGH); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,HIGH);  //给低电平
  analogWrite(ENB,motorSpeed); 
}

// 右转
void turnR(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,HIGH);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,HIGH); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,motorSpeed);       
}

// 左前
void forwardL(){
  digitalWrite(In1,HIGH); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,LOW);  //给低电平   
  analogWrite(ENB,0);
}

// 右前
void forwardR(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,0);
    
  digitalWrite(In3,HIGH); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,motorSpeed); 
}

// 左后
void backwardL(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,HIGH);  //给低电平
  analogWrite(ENA,motorSpeed);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,0);
}

// 右后
void backwardR(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,0);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,HIGH);  //给低电平
  analogWrite(ENB,motorSpeed);  
}

// 停止
void stop(){
  digitalWrite(In1,LOW); //给高电平
  digitalWrite(In2,LOW);  //给低电平
  analogWrite(ENA,0);
  
  digitalWrite(In3,LOW); //给高电平
  digitalWrite(In4,LOW);  //给低电平
  analogWrite(ENB,0);   
}

// 左转90度
void turnL90(){
  turnL();
  delay(TURN_LEFT_90);
}

// 右转90度
void turnR90(){
  turnR();
  delay(TURN_RIGHT_90);  
}

// 掉头
void turnBack(){
  turnL();
  delay(TURN_BACK);  
}

int getDistance(){
   
  digitalWrite(hcTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(hcTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(hcTrig, LOW);

  duration = pulseIn(hcEcho, HIGH);
  cm = (duration/2) / 29.1;
  
  return cm;
}

// 检查左右方向距离并返回专项方向
void autoTurn(){
  
  // 检查右侧距离     
  for (int pos = 90; pos >= 0; pos -= 1) {
    headServo.write(pos);               
    delay(3);                     
  }
  delay(300);
  int rightDist =  getDistance();

  // 检查左侧距离     
  for (int pos = 0; pos <= 180; pos += 1) {
    headServo.write(pos);                
    delay(3);                     
  }
  delay(300);
  int leftDist =  getDistance();  

  //将头部调整到正前方
  for (int pos = 180; pos >= 90; pos -= 1) {
    headServo.write(pos);               
    delay(3);                     
  }
  delay(500);
  
  //检查左右距离并做出转向决定
  if ( rightDist < DIST_THRESHOLD && leftDist < DIST_THRESHOLD){  // 如果左右方向距离均小于允许距离                 
    turnBack();                                     // 掉头
    return;
  } else if ( rightDist >= leftDist){               // 如果右边距离大于左边距离                 
    turnR90();                                      // 右转90度
    return;
  } else {                                         // 如果左边距离大于右边距离                
    turnL90();                                     // 左转90度
    return;
  }
}

void autoMode(){ 
    delay(50);              // 提高系统稳定性等待
    int frontDist = getDistance(); // 检查前方距离
    if(frontDist >= DIST_THRESHOLD){          // 如果检测前方距离读数大于等于允许距离参数
      forward();               // 向前
    } else {                           // 如果检测前方距离小于允许距离参数
      stop();                  // 停止   
      autoTurn();                      // 检测左右侧距离并做出自动转向 
    }
  }
