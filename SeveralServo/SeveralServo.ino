/*******************************************************************************
  * @笔者  ： 云上太阳
  * @日期  ： 2017年11月03日
  * @所属  ： 杭州众灵科技                  
  * @库文件： <Servo.h>
  * @功能：   蓝牙APP串口控制舵机
 ******************************************************************************/
/*----------------------------------------------------------------------------
    相关函数:
    1.servo类成员函数
    attach()              设定舵机的接口，只有9或10接口可利用。
    write()               用于设定舵机旋转角度的语句，可设定的角度范围是0°到180°。
    writeMicroseconds()　 用于设定舵机旋转角度的语句，直接用微秒作为参数。
    attached()            判断舵机参数是否已发送到舵机所在接口。
    detach()              使舵机与其接口分离，该接口（9或10）可继续被用作PWM接口。
              舵机停止指令：$DST!
    相关指令: 单舵机控制指令：#indexPpwmTtime!(index-舵机序号0,1,2…;pwm-舵机PWM值500-2500之间;time-舵机执行时间0-65535MS)
              多舵机控制指令：{#index1Ppwm1Ttime1！#index2Ppwm2Ttime2！…}动作组指令，多个单舵机指令合并在一起，然后加个大括号(大括号有无都可）
             
  ----------------------------------------------------------------------------*/
//#000P1000T100!
#include <Servo.h>                //声明调用Servo.h库
String inString = "";             //声明一个字符串
char cmd1[10]="";                 //声明一个字符数组，存储输入的指令
char cmd2[]="$DST!";              //声明一个字符数组，存储固定的指令
#define  SERVO_NUM  6             //宏定义舵机个数
#define  SERVO_TIME_PERIOD  20    //每隔20ms处理一次（累加）舵机的PWM增量 
//unsigned int time_max = 0;
byte  servo_pin[SERVO_NUM] = {10, A2, A3, A0, A1 ,7};            //宏定义舵机控制引脚
Servo myservo[SERVO_NUM];         //创建一个舵机类
typedef struct {                  //舵机结构体变量声明
    unsigned int aim = 1500;      //舵机目标值 
    float cur = 1500.0;           //舵机当前值
    unsigned  int time1 = 1000;   //舵机执行时间
    float inc= 0.0;               //舵机值增量，以20ms为周期
}duoji_struct;
duoji_struct servo_do[SERVO_NUM];           //用结构体变量声明一个舵机变量组                         
/*
时间处理函数，第一个参数是上一次处理时间点， 第二个参数是处理时间间隔，
到达时间间隔返回0，否则返回1
*/
bool handleTimePeriod( unsigned long *ptr_time, unsigned int time_period) {
    if((millis() - *ptr_time) < time_period) {
        return 1;  
    } else{
         *ptr_time = millis();
         return 0;
    }
}
//接收串口发来的字符串
void uartReceive(){        
    while (Serial.available()>0) {   //如果串口有数据
        char inChar = Serial.read(); //读取串口字符
        //inString += inChar;
        inString.concat(inChar);     //连接接收到的字符组
        delayMicroseconds(100);      //为了防止数据丢失,在此设置短暂延时100us
        Serial.flush();             //清空串口接收缓存
        if(inString.length() > 200){
            inString ="";
        }
    }
}
//解析串口接收指令   
void parseInStringCmd(){
    static unsigned int index, time1, pwm1, i;//声明三个变量分别用来存储解析后的舵机序号，舵机执行时间，舵机PWM
    unsigned int len;             //存储字符串长度  
    if(inString.length() > 0) {   //判断串口有数据      
        if((inString[0] == '#') || (inString[0] == '{')) {   //解析以“#”或者以“{”开头的指令
            char len = inString.length();       //获取串口接收数据的长度
            index=0; pwm1=0; time1=0;           //3个参数初始化
            for(i = 0; i < len; i++) {          //
                if(inString[i] == '#') {        //判断是否为起始符“#”
                    i++;                        //下一个字符
                    while((inString[i] != 'P') && (i<len)) {     //判断是否为#之后P之前的数字字符
                        index = index*10 + (inString[i] - '0');  //记录P之前的数字
                        i++;
                    }
                    i--;                          //因为上面i多自增一次，所以要减去1个
                } else if(inString[i] == 'P') {   //检测是否为“P”
                    i++;
                    while((inString[i] != 'T') && (i<len)) {  //检测P之后T之前的数字字符并保存
                        pwm1 = pwm1*10 + (inString[i] - '0');
                        i++;
                    }
                    i--;
                } else if(inString[i] == 'T') {  //判断是否为“T”
                    i++;
                    while((inString[i] != '!') && (i<len)) {//检测T之后!之前的数字字符并保存
                        time1 = time1*10 + (inString[i] - '0'); //将T后面的数字保存
                        i++;
                    }
                    if((index >= SERVO_NUM) || (pwm1 > 2500) ||(pwm1<500)) {  //如果舵机号和PWM数值超出约定值则跳出不处理 
                        break;
                    }
                    //检测完后赋值
                    servo_do[index].aim = pwm1;         //舵机PWM赋值
                    servo_do[index].time1 = time1;      //舵机执行时间赋值
                    float pwm_err = servo_do[index].aim - servo_do[index].cur;
                    servo_do[index].inc = (pwm_err*1.00)/(servo_do[index].time1/SERVO_TIME_PERIOD); //根据时间计算舵机PWM增量
                    index = pwm1 = time1 = 0; 
                }
            }
        } else if(strcmp(inString.c_str(),"$DST!")==0) { //.c_str把C++中的字符串转换为C中的字符串,然后和字符串"$DST!"作比较
                                                         //使用格式：类型 strcmp（参数1，参数2）
                                                         //功 能: 比较参数1和参数（1、若参数1>参数2，返回正数；2、若参数1<参数2，返回负数；3、若参数1=参数2，返回0；）   
            for(i = 0; i < SERVO_NUM; i++) {
                servo_do[i].aim =  (int)servo_do[i].cur;
            }           
        } 
    inString = "";
    } 
}  
//舵机PWM增量处理函数，每隔SERVO_TIME_PERIOD毫秒处理一次，这样就实现了舵机的连续控制
void handleServo() {
    static unsigned long systick_ms_bak = 0;
    if(handleTimePeriod(&systick_ms_bak, SERVO_TIME_PERIOD))return;  
    for(byte i = 0; i < SERVO_NUM; i++) {
        if(abs( servo_do[i].aim - servo_do[i].cur) <= abs (servo_do[i].inc) ) { //实际需要增量 <= 单次增量 则 直接令cur=aim
             myservo[i].writeMicroseconds(servo_do[i].aim);      
             servo_do[i].cur = servo_do[i].aim;
             
        } else {                                                                //实际需要增量 > 单次增量 则 cur增加inc的量
             servo_do[i].cur +=  servo_do[i].inc;
             myservo[i].writeMicroseconds((int)servo_do[i].cur); 
        }    
    }
}   

//初始化函数                    
void setup(){ 
    //put your setup code here, to run once:
    for(byte i = 0; i < SERVO_NUM; i++){
        myservo[i].attach(servo_pin[i]);   // 将10引脚与声明的舵机对象连接起来
    }
    Serial.begin(115200);                 //初始化波特率为115200
} 

//主循环函数
void loop(){ 
   //put your main code here, to run repeatedly:
   uartReceive();
   parseInStringCmd();
   handleServo();
} 
