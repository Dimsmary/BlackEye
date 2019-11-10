/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define led 12
RF24 radio(2, 3); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;
void setup() {
  pinMode(12, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
//  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  Serial.begin(9600);
}
void loop() {
  delay(5);
  radio.stopListening();
  long recv = 0;
  // 定义时间
  int potValue = analogRead(A0);
  int potValue1 = analogRead(A1);
  long angleValue = map(potValue, 0, 1023, 0, 180) + 256;
  long angleValue1 = map(potValue1, 0, 1023, 0, 180) + 512;

  
  radio.write(&angleValue, sizeof(angleValue)); //识别码添加
  radio.write(&angleValue1, sizeof(angleValue1)); //识别码添加
//  Serial.println(angleValue);
//  radio.startListening();
//  unsigned long started_waiting_at = millis();
//  bool timeout = false;
//  while (!timeout)
//      if (millis() - started_waiting_at > 200 )
//        timeout = true;
//      radio.read(&recv, sizeof(recv));
//      if(recv == 1){
//        timeout = false;
//      }
//
//        
//  if(!timeout){
//      // Spew it
//      Serial.println("GOT");
//  }
}
