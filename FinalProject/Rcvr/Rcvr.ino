const int CEPIN = 9;
const int CSNPIN = 10;

#include <Servo.h>

Servo servoarm1;
Servo servoarm2;
Servo servohead;
int pos = 0;

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

//dc motors pins
const int rightDirPin = 4;
const int rightPWMPin = 5;
const int leftDirPin = 7;
const int leftPWMPin = 6;


void setup() {

  // dc motor pins
  pinMode(rightDirPin, OUTPUT);
  pinMode(leftDirPin, OUTPUT);

  Serial.begin(115200);

  // RF24 setup
  bool retval = radio.begin();
  // Serial.println(retval);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //servo motor pins
  servoarm1.attach(3);
  servoarm2.attach(8);
//  servohead.attach(#);
}

void loop(){
  if (radio.available())
  {
    int data;

    radio.read(&data, sizeof(data));
    //Serial.println(data);

    switch (data) {
        break;
      
      // motor moving right
      case 0x01:
        Serial.println("turning right");
        digitalWrite(rightDirPin, LOW);
        analogWrite(rightPWMPin, 200);
        break;
      
      //motor moving forward
      case 0x02:
        Serial.println("forward");
        digitalWrite(rightDirPin, HIGH);
        analogWrite(rightPWMPin, 0);
        digitalWrite(leftDirPin, LOW);
        analogWrite(leftPWMPin, 255);
        break;
      
      //motor moving left
      case 0x04:
        Serial.println("turning left");
        digitalWrite(leftDirPin, HIGH);
        analogWrite(leftPWMPin, 50);
        break;
      
      //sneezing sequence
      case 0x08:
        Serial.println("sneezing");
        //all arms are at position 0
//        servohead.write(0);
        servoarm1.write(0);
        servoarm2.write(0);
        delay(400);
        //arm1 and head move
//        servohead.write(100);
        servoarm1.write(100);
        delay(400);
        //arm1 and head go back to position 0
//        servohead.write(0);
        servoarm1.write(0);
        break;
      
      //normal sequence
      case 0x16:
        Serial.println("normal");
        //constant movement of arms
        for (pos = 0; pos <= 180; pos += 1){
          servoarm1.write(pos);
          servoarm2.write(pos);
          delay(15);
         }
         for (pos = 180; pos >= 0; pos -= 1){
          servoarm1.write(pos);
          servoarm2.write(pos);
          delay(15);
         }
        break;
      
      //default sequence, everything is static
      default:
        Serial.println("invalid code");
        stop();
        digitalWrite(rightDirPin, LOW);
        digitalWrite(rightPWMPin, LOW);
//        servohead.write(0);
        servoarm1.write(0);
        servoarm2.write(0);
        break;
    }
  } else {
    Serial.println("stop");
    stop();
  }
  delay(5);
}

void stop() {

  digitalWrite(rightDirPin, LOW);
  analogWrite(rightPWMPin, 0);
  digitalWrite(leftDirPin, LOW);
  analogWrite(leftPWMPin, 0);
//  servohead.write(0);
  servoarm1.write(0);
  servoarm2.write(0);
}
