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
#include <Tennimate.h>
#define DIST_THRESHOLD 35        // 避障距离阈值（cm）

#define TURN_LEFT_90   900        // 左转90度延迟参数
#define TURN_RIGHT_90   1000      // 右转90度延迟参数
#define TURN_BACK   1600          // 掉头延迟参数

// 建立Tennimate对象。其中对象参数分别是：
// (L298N模块的ENA, In1, In2, ENB, In3, In4引脚
// 车轮电机运转速度，测距传感器TRIG引脚， 测距传感器ECHO引脚，头部舵机信号引脚 )
Tennimate TM(5, 12, 13, 7, 8, 6, 100, 2, 3, 10); 
                                                                                                
                           
void setup() {

  TM.headServoIni();    //头部舵机初始化
  TM.setHeadPos(90);    // 系统启动时将头部设置为90位置
}

void loop() {
autoMode();
}

// 左转90度
void turnL90(){
  TM.turnL();
  delay(TURN_LEFT_90);
}

// 右转90度
void turnR90(){
  TM.turnR();
  delay(TURN_RIGHT_90);  
}

// 掉头
void turnBack(){
  TM.turnL();
  delay(TURN_BACK);  
}

// 检查左右方向距离并返回专项方向
void autoTurn(){
  
  // 检查右侧距离     
  for (int pos = 90; pos >= 0; pos -= 1) {
    TM.setHeadPos(pos);               
    delay(3);                     
  }
  delay(300);
  int rightDist =  TM.getDistance();

  // 检查左侧距离     
  for (int pos = 0; pos <= 180; pos += 1) {
    TM.setHeadPos(pos);                
    delay(3);                     
  }
  delay(300);
  int leftDist =  TM.getDistance();  

  //将头部调整到正前方
  for (int pos = 180; pos >= 90; pos -= 1) {
    TM.setHeadPos(pos);               
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
    int frontDist = TM.getDistance(); // 检查前方距离
    if(frontDist >= DIST_THRESHOLD){          // 如果检测前方距离读数大于等于允许距离参数
      TM.forward();               // 向前
    } else {                           // 如果检测前方距离小于允许距离参数
      TM.stop();                  // 停止   
      autoTurn();                      // 检测左右侧距离并做出自动转向 
    }
  }
