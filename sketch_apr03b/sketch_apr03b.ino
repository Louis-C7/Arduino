#include <SoftwareSerial.h>

SoftwareSerial softSerial(10, 11); // RX, TX
typedef struct
{
  int data[50][2] = {{0,0}};
  int len = 0;
}List;
List list;

void setup() {
  // put your setup code here, to run once:
  softSerial.begin(115200);
  Serial.begin(115200);
}

void loop() {
  if(softSerial.available())
  {
    getList();
    for (int i=0; i<list.len; i++)
    {
      Serial.print(list.data[i][0]);
      Serial.print('\t');
      Serial.println(list.data[i][1]);
    }
    Serial.println("============");
    clearList();
  }

}


String detectString()
{
  while(softSerial.read() != '{');
  return(softSerial.readStringUntil('}'));
}
void clearList()
{
  memset(list.data, sizeof(list.data),0);
  list.len = 0;
}
void getList()
{
  String s = detectString();
  String numStr = "";
  for(int i = 0; i<s.length(); i++)
  {
    if(s[i]=='('){
      numStr = "";
    }
    else if(s[i] == ','){
      list.data[list.len][0] = numStr.toInt();
      numStr = "";
    }
    else if(s[i]==')'){
      list.data[list.len][1] = numStr.toInt();
      numStr = "";
      list.len++;
    }
    else{
      numStr += s[i];
    }
  }
}
