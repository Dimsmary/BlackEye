#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// 设置NRF接口，使用默认SPI端口，2-CE / 3-CSN
RF24 radio(2,3);

// 管道地址
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

void setup(){

  Serial.begin(115200);
  printf_begin();

  // Setup and configure rf radio

  radio.begin();
  radio.setAutoAck(1);                    // 确保ACK开启
  radio.enableAckPayload();               // 开启自定义ACK有效载荷
  radio.setRetries(0,15);                 // 重试之间的最短时间，最大次数重试次数
  radio.setPayloadSize(4);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();                 // Start listening
}

void loop(void) {
  // Pong back role.  Receive each packet, dump it out, and send it back
    byte pipeNo = 0;
    long gotByte = 0;                                       // Dump the payloads until we've gotten everything
    while( radio.available(&pipeNo)){
      radio.read( &gotByte, sizeof(gotByte) );
      Serial.println(gotByte);
      radio.writeAckPayload(pipeNo,&gotByte, sizeof(gotByte) );    
   }
}
