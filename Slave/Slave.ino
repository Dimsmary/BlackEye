#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <Servo.h>

// 引用了SPI/NRF库

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL }; 
// 定义了传输地址

Servo myServo0;
Servo myServo1;
// 定义舵机对象


// 设置NRF接口，使用默认SPI端口，2-CE / 3-CSN

const int control1 = 8;
const int control2 = 13;
// MOS输出控制脚

const long command1 = 256;
const long command2 = 512;
const long command3 = 768;
const long command4 = 1024;
const long command5 = 1280;
const long command6 = 1536;
const long command7 = 1792;
const long command8 = 2048;

// 命令列表

long eye_sta[] = {0, 0, 0, 0};
// 状态数组

long servoANG[] = {900, 900};
// 舵机角度

const int servo0 = 5;
const int servo1 = 10;
// 舵机端口

RF24 radio(4,9);
void setup(){
  Serial.begin(115200);
  // 启用串口
  printf_begin();
  // 启用printf 

  myServo0.attach(servo0);
  myServo1.attach(servo1);
  // 绑定舵机输出口
  
  radio.begin();
  radio.setAutoAck(1);
  radio.enableAckPayload();
  // 允许可选ACK载荷
  // 启用NRF-ACK
  radio.setRetries(0,15);
  // 设定重试最小时间，最大重试次数
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  // 切换到发送模式

  pinMode(control1, OUTPUT);
  pinMode(control2, OUTPUT);
  pinMode(A0, INPUT);
//  digitalWrite(ctr1, HIGH);
  // 开启舵机电源
}

void loop(void) {
    byte pipeNo = 0;
    long gotdata = 0; 
    while( radio.available(&pipeNo)){
      radio.read( &gotdata, sizeof(gotdata) );
      
   }
   int order = gotdata >> 8;
   // 识别码
   Serial.println(order);
   if(order == 1){
    radio.writeAckPayload(pipeNo,&command1, sizeof(gotdata) );
      if (servoANG[0] < 1800){
      servoANG[0] = servoANG[0] + 1;
    }
   }
   
   else if(order  == 2){
    radio.writeAckPayload(pipeNo,&command2, sizeof(gotdata) );
    if (servoANG[0] > 0){
      servoANG[0] = servoANG[0] - 1;
    }
   }
   else if(order  == 3){
    radio.writeAckPayload(pipeNo,&command3, sizeof(gotdata) );
    if (servoANG[1] < 1800){
      servoANG[1] = servoANG[1] + 1;
    }
   }
   else if(order  == 4){
    radio.writeAckPayload(pipeNo,&command4, sizeof(gotdata) );
    if (servoANG[1] > 0){
      servoANG[1] = servoANG[1] - 1;
    }
   }
   // 以上为舵机控制命令
   else if(order == 5){
    eye_sta[1] = !eye_sta[1];
    radio.writeAckPayload(pipeNo, &command3, sizeof(eye_sta[1]) );
    digitalWrite(control1, eye_sta[1]);
   }
   else if(order == 6){
    eye_sta[2] = !eye_sta[2];
    radio.writeAckPayload(pipeNo, &command4, sizeof(eye_sta[2]) );
    digitalWrite(control2, eye_sta[2]);
   }

   myServo0.write(servoANG[0] / 10);
   myServo1.write(servoANG[1] / 10);

   // 更新舵机方位角
}
