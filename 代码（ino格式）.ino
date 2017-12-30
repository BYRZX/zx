主机（仅实现部分功能，主要是控制密码锁）：
#include <Key.h>
#include <Keypad.h>

#include<Wire.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup()
{
  Wire.begin();
  Serial.begin(9600);
} long x = 0;
void loop()
{


  char keyz = keypad.getKey();

  if (keyz != NO_KEY) {
    Wire.beginTransmission(4);
    Wire.write("x is ");
    Serial.println(keyz);
    Wire.write(keyz);
    Wire.endTransmission();
  }


  delay(100);
}





从机代码：
#include <IRremote.h
#include <Servo.h>
#include<Wire.h>
Servo myservo;
Servo dengguan;//设置两个舵机，舵机myservo负责控制门锁，dengguan负责在门开时开灯，门锁时关灯
long x=0;//红外解码初始化
#define servoPin  9
int mima=0;
#if defined (ESP32)
#define servoPin  D9   //使用ESP32时，数字引脚号
#endif
int dengkong=11;//控制舵机的引脚
int kai=7,guan=8;//7,8控制门开时亮绿灯、门关时亮红灯
int pos = 0; //舵机初始角度
int RECV_PIN = 10;   //红外线接收器OUTPUT端接在pin 10
IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
void setup(){
Serial.begin(9600);
dengguan.attach(dengkong);
irrecv.enableIRIn(); // 启动红外解码
myservo.attach(servoPin);  //舵机工作
Wire.begin(4);//设计主从机
Wire.onReceive(receiveEvent);//设计密码锁敲入密码时信息传递至本机
Serial.begin(9600);
}
void loop()
{//char dada=0;   if(dada!=0){
// Serial.println("x=");
// Serial.println(dada)}
char zzz=Wire.read();
if (irrecv.decode(&results)) {      //解码成功，收到一组红外线信号
Serial.println(results.value);//// 输出红外线解码结果（十六进制）
x=results.value;
irrecv.resume();
//  接收下一个值    irrecv.resume();
}
if(x==16754775||zzz=='D')
for (pos = 0; pos < 90; pos  += 1) // 舵机转动控制循环（开门）
{ // in steps of 1 degree
myservo.write(pos);              // 转动
dengguan.write(pos);
delay(15);                       // 延迟
}
else if(x==16769565||zzz=='A')
for (pos = 90; pos >= 1; pos -= 1) // 舵机转动控制循环（关门）
{
myservo.write(pos);
dengguan.write(pos);
delay(15);
}
x=0;
if(pos==90)
{digitalWrite(guan,HIGH);//8引脚插绿灯，控制亮灯（开；绿灯）
digitalWrite(kai,LOW);
}if(pos==0){
digitalWrite(kai,HIGH);//7引脚插红灯，控制亮灯（关；红灯）
digitalWrite(guan,LOW);
}}
void receiveEvent(int howMany)
{
/*while(1<Wire.available())
{
char c=Wire.read();
Serial.print(c);
}*/
char x=Wire.read();
Serial.println(x);
if(x=='D')
for (pos = 0; pos < 90; pos  += 1) //控制角度改变转动（开门）
{ // in steps of 1 degree
myservo.write(pos);            //输入角度
dengguan.write(pos);
delay(15);                       // 延时
}
else if(x=='A')
for (pos = 90; pos >= 1; pos -= 1) //控制角度改变转动（关门）
{
myservo.write(pos);              // 输入角度
dengguan.write(pos);
delay(15);                       //延时
}
｝
//从主机获得矩阵键盘中的按键信息
//解码数据：FFA857 FFE21D 16754775 16769565
 
