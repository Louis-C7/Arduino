int input1 = 12; // 定义uno的pin 5 向 input1 输出 
int input2 = 13; // 定义uno的pin 6 向 input2 输出
int enA = 11; // 定义uno的pin 10 向 输出A使能端输出
int input3 = 7; // 定义uno的pin 5 向 input1 输出 
int input4 = 8; // 定义uno的pin 6 向 input2 输出
int enB = 9; // 定义uno的pin 10 向 输出A使能端输出
#define motorSpeed  100       //电机速度
void setup() {
pinMode(input3,OUTPUT);
pinMode(input4,OUTPUT);
pinMode(enB,OUTPUT);
pinMode(input1,OUTPUT);
pinMode(input2,OUTPUT);
pinMode(enA,OUTPUT);
}
 
void loop() {
  
forward();
}
void forward()
{
  digitalWrite(input1,HIGH); //给高电平
  digitalWrite(input2,LOW);  //给低电平
  analogWrite(enA,motorSpeed);
  digitalWrite(input3,HIGH); //给高电平
  digitalWrite(input4,LOW);  //给低电平
  analogWrite(enB,motorSpeed);
  }
