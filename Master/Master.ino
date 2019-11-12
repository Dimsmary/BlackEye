#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// 引用了SPI/NRF库

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL }; 
// 定义了传输地址

unsigned char key_sta[2] = {0, 0};
RF24 radio(2,3);
// 设置NRF接口，使用默认SPI端口，2-CE / 3-CSN

byte button1_STA = LOW;
int buttonState1 = 0;
int lastButtonState1 = 1;

byte button2_STA = LOW;
int buttonState2 = 0;
int lastButtonState2 = 1;
// 按钮中断变量


const int  buttonPin1 = 0;   
const int  buttonPin2 = 7;       
// 中断输入脚



const long command1 = 256;
const long command2 = 512;
const long command3 = 768;
const long command4 = 1024;
const long command5 = 1280;
const long command6 = 1536;
const long command7 = 1792;
// 命令列表

void setup(){
  Serial.begin(115200);
  // 启用串口
  printf_begin();
  // 启用printf
  
  radio.begin();
  radio.setAutoAck(1);
  radio.enableAckPayload();
  // 允许可选ACK载荷
  // 启用NRF-ACK
  radio.setRetries(0,15);
  // 设定重试最小时间，最大重试次数
  radio.openWritingPipe(pipes[0]);
  radio.stopListening();
  // 切换到发送模式

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
// 初始化电位器读取
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
// 初始化按钮中断输入

  attachInterrupt(digitalPinToInterrupt(0), switch1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), switch2, CHANGE);
// 配置中断
}

long SendData(long data){
  long gotdata;
  //定义
  if (!radio.write(&data, sizeof(data))){
      return 0;
      // 未发送成功      
  }else{
    if(!radio.available()){ 
      return 0; 
      // 发送成功但ack为空
    }else{
      while(radio.available() ){
        radio.read( &gotdata, sizeof(gotdata));
      }
      return gotdata;
    }
  }
return 0;
}

void loop(){
  int potValueX = analogRead(A0);
  int potValueY = analogRead(A1);
  long angleValueX = map(potValueX, 0, 1023, 0, 180);
  long angleValueY = map(potValueY, 0, 1023, 0, 180);
  // 读取电位器并与舵机角度匹配 后方为高位识别命令码
  
  if (angleValueX > 95){
    SendData(command1);
  }
  else if(angleValueX < 85){
    SendData(command2);
  }
  if (angleValueY > 95){
    SendData(command3);
  }
  else if(angleValueY < 85){
    SendData(command4);
  }

  
// 发送操控命令
  
  // 按键中断检测
  if (buttonState1 == HIGH){
    buttonState1 = LOW;
    Serial.println(SendData(command5));
  }
  if (buttonState2 == HIGH){
    buttonState2 = LOW;
    SendData(command6);
  }
  // 按键中断处理
}

void switch1(){
  buttonState1 = digitalRead(buttonPin1);
  // 与上一次检测到的状态对比
  if (buttonState1 != lastButtonState1) {
    // 如果检测到状态改变
    if (buttonState1 == HIGH) {
      // 高电平为松开   
      Serial.println("off");
    } else {
      // 低电平为按下
      button1_STA = HIGH;
      // 标志位
      Serial.println("on");
    }
    //消抖
    delay(50);
  }
  // 每次改变都保存状态检测改变
  lastButtonState1 = buttonState1;
}

void switch2(){
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == HIGH) { 
      Serial.println("off");
    } else {
      button2_STA = HIGH;
      // 标志位
      Serial.println("on");
    }
    delay(50);
  }
  lastButtonState2 = buttonState2;
  // 原理同Switch1
}
