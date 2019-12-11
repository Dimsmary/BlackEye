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

const long command1 = 4096;
const long command2 = 8192;
const long command3 = 12288;
const long command4 = 16384;
const long command5 = 20480;
const long command6 = 24576;
const long command7 = 28672;
const long command8 = 32768;

// 命令列表

long eye_sta[] = {0, 0, 0, 0};
long back_data;
// 状态数组

long servoANG[] = {900, 900};
// 舵机角度
int servo_speed = 1;
// 舵机速度
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
   int order = gotdata >> 12;
   // 识别码
   Serial.println(order);
   if(order == 1){
    radio.writeAckPayload(pipeNo,&command1, sizeof(gotdata) );
      if (servoANG[0] < 1800){
      servoANG[0] = servoANG[0] + servo_speed;
    }
   }
   
   else if(order  == 2){
    radio.writeAckPayload(pipeNo,&command2, sizeof(gotdata) );
    if (servoANG[0] > 0){
      servoANG[0] = servoANG[0] - servo_speed;
    }
   }
   else if(order  == 3){
    radio.writeAckPayload(pipeNo,&command3, sizeof(gotdata) );
    if (servoANG[1] < 1800){
      servoANG[1] = servoANG[1] + servo_speed;
    }
   }
   else if(order  == 4){
    radio.writeAckPayload(pipeNo,&command4, sizeof(gotdata) );
    if (servoANG[1] > 0){
      servoANG[1] = servoANG[1] - servo_speed;
    }
   }
   // 以上为舵机控制命令
   else if(order == 5){
    eye_sta[1] = !eye_sta[1];
    digitalWrite(control1, eye_sta[1]);
    radio.writeAckPayload(pipeNo, &eye_sta[1], sizeof(eye_sta[1]) );
   }
   
   // 舵机电源控制
   
   else if(order == 6){
    eye_sta[2] = !eye_sta[2];
    digitalWrite(control2, eye_sta[2]);
    radio.writeAckPayload(pipeNo, &eye_sta[2], sizeof(eye_sta[2]));
   }
   
   // 摄像头电源控制
  else if(order == 7){
    back_data = command7 + analogRead(A0);
    // 命令识别码 低八位跟电池模拟值
    if (eye_sta[1] == 1){
      back_data = back_data + 256;
    }
    if (eye_sta[2] == 1){
      back_data = back_data + 512;
    }
    if (servo_speed == 2){
      back_data = back_data + 1024;
    }
    else if(servo_speed == 3){
      back_data = back_data + 2048;
    }
    else if(servo_speed == 4){
      back_data = back_data + 3072;
    }
    radio.writeAckPayload(pipeNo, &back_data, sizeof(back_data));
  }
  else if(order == 8){
    if(servo_speed == 4){
      servo_speed = 1;
    }
    else{
      servo_speed ++;
    }
  }
   myServo0.write(servoANG[0] / 10);
   myServo1.write(servoANG[1] / 10);

   // 更新舵机方位角
}
