#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// 设置NRF接口，使用默认SPI端口，2-CE / 3-CSN
RF24 radio(2,3);

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.
void setup(){

  Serial.begin(115200);
  printf_begin();

  // Setup and configure rf radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
//  radio.setPayloadSize(4);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[0]);
}


void SendData(long data) {
    radio.stopListening();                                  // First, stop listening so we can talk.
    long gotByte;  
    if (!radio.write( &data, sizeof(data) )){
        Serial.println(F("failed."));      
    }else{

      if(!radio.available()){ 
        Serial.println(F("Blank Payload Received.")); 
      }else{
        while(radio.available() ){
          radio.read( &gotByte, sizeof(gotByte) );
          printf("Got response %d\n",gotByte);
        }
      }
    }
}
void loop(void) {
    long i = 256;
    SendData(i);
    delay(1000);
}
