const int CEPIN = 9;
const int CSNPIN = 10;

// In summary,
// nRF 24L01 pin    Arduino pin   name
//          1                     GND
//          2                     3.3V
//          3             9       CE
//          4             10      CSN  
//          5             13      SCLK
//          6             11      MOSI/COPI
//          7             12      MISO/CIPO 

//radio setup
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(CEPIN, CSNPIN);

//number of address used
const byte address[6] = "00001"; 

//pushbuttons pins
const int leftPin = 4;
const int rightPin = 7;
const int fwdPin = 8;
const int snzPot = A0;
const int norPot = A1;

void setup(){
  Serial.begin(115200);
  boolean retval = radio.begin();
  Serial.println(retval);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(fwdPin, INPUT_PULLUP);
}

void loop(){\
  int nor = digitalRead(norPot) << 4;
  int snz = digitalRead(snzPot) << 3;
  int left = digitalRead(leftPin) << 2;
  int fwd = digitalRead(fwdPin) << 1;
  int right = digitalRead(rightPin) << 0;
  
  int data = left | fwd | right | snz | nor;
  if (data){
    Serial.print( "sending data = " );
    Serial.println(data);
    radio.write(&data, sizeof(data)) ;
   }
}
