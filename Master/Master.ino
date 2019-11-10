#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// 引用了SPI/NRF库

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL }; 
// 定义了传输地址

RF24 radio(2,3);
// 设置NRF接口，使用默认SPI端口，2-CE / 3-CSN
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
}

long SendData(long data){
  long gotdata;
  //定义
  if (!radio.write( &data, sizeof(data) )){
      return 0x100;
      // 未发送成功      
  }else{
    if(!radio.available()){ 
      return 0x200; 
      // 发送成功但ack为空
    }else{
      while(radio.available() ){
        radio.read( &gotdata, sizeof(gotdata) );
      }
      return gotdata;
    }
  }
}

void loop(){
  int potValueX = analogRead(A0);
  int potValueY = analogRead(A1);
  long angleValueX = map(potValueX, 0, 1023, 0, 180) + 256;
  long angleValueY = map(potValueY, 0, 1023, 0, 180) + 512;
  // 读取电位器并与舵机角度匹配 后方为高位识别命令码
//  radio.write(&angleValueX, sizeof(angleValueX)); 
//  radio.write(&angleValueY, sizeof(angleValueY)); 
  SendData(angleValueX);
  SendData(angleValueY);
}
