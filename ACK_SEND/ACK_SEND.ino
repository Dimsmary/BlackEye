// TrackControl - the master or the transmitter

 // http://tmrh20.github.io/RF24/

 //~ - CONNECTIONS: nRF24L01 Modules See:
 //~ http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
 //~ 1 - GND
 //~ 2 - VCC 3.3V !!! NOT 5V
 //~ 3 - CE to Arduino pin 9
 //~ 4 - CSN to Arduino pin 10
 //~ 5 - SCK to Arduino pin 13
 //~ 6 - MOSI to Arduino pin 11
 //~ 7 - MISO to Arduino pin 12
 //~ 8 - UNUSED

#include <SPI.h>
//~ #include <TMRh20nRF24L01.h>
//~ #include <TMRh20RF24.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   2
#define CSN_PIN 3

// NOTE: the "LL" at the end of the constant is "LongLong" type
// These are the IDs of each of the slaves
const uint64_t slaveID[2] = {0xE8E8F0F0E1LL, 0xE8E8F0F0E2LL} ;

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int dataToSend[2];

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000;
int txVal = 0;
int ackMessg[4];
byte ackMessgLen = 2;
