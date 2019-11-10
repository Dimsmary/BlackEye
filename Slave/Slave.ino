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

RF24 radio(2,3);
// 设置NRF接口，使用默认SPI端口，2-CE / 3-CSN
void setup(){
  Serial.begin(115200);
  // 启用串口
  printf_begin();
  // 启用printf

  myServo0.attach(5);
  myServo1.attach(9);
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
}

void loop(void) {
    byte pipeNo = 0;
    long gotdata = 0; 
    while( radio.available(&pipeNo)){
      radio.read( &gotdata, sizeof(gotdata) );
      
   }
   int order = gotdata >> 8;
   // 识别码
   
   if(order == 1){
    radio.writeAckPayload(pipeNo,&gotdata, sizeof(gotdata) );
    myServo0.write(gotdata - 256);
    
   }
   else if(order  == 2){
    radio.writeAckPayload(pipeNo,&gotdata, sizeof(gotdata) );
    myServo1.write(gotdata - 512);
   }
}
