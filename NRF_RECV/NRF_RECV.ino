/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#define button 4 
RF24 radio(2, 3); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
Servo myServo;
Servo myServo1;
boolean buttonState = 0;

void setup() {
  pinMode(button, INPUT);
  myServo.attach(5);
  myServo1.attach(9);
  radio.begin();
//  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  Serial.begin(9600);
}
void loop() {
  delay(5);
  radio.startListening();
  if ( radio.available()) {
      long angleV = 0;
      radio.read( &angleV, sizeof(angleV) );
      int order = angleV >> 8;
      if(order == 1){
        myServo.write(angleV - 256);
        radio.stopListening();
        long sendd = 1;
        radio.write(&sendd, sizeof(sendd) );
      }
      else if(order == 2){
        myServo1.write(angleV - 512);
      }

  }
}
