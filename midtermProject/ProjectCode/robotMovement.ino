
/*
   Example using the nRF24L01 radio module to communicate
   between two Arduinos,
   the transmitter is connected to some pushbuttons
   the receiver is connected to a motor driver
*/

// Pin usage
// nRF24L01 uses SPI which is fixed on pins 11, 12, and 13.
// It also requires two other signals
// (CE = Chip Enable, CSN = Chip Select Not)
// Which can be any pins:

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

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(CEPIN, CSNPIN);  // CE, CSN

#include <Adafruit_NeoPixel.h>

#define LED_PIN    3
#define LED_COUNT 64

// this is for controlling singular LEDs. Being used to create faces on the neopixel
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN 3

// this is for the scrolling text sequence I have set up on my neopixel screen... the one being triggered by the "rex" button
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};


// Byte of array representing the address.
// This is the address where we will send the data.
// This should be same on the receiving side.
const byte address[6] = "00001";

// Motor controller pins
//For motors connected to 1st H-bridge
const int rightDirPin1 = 4;
const int rightPWMPin1 = 5;
const int leftDirPin1 = 7;
const int leftPWMPin1 = 6;

//For motors connected to 2nd H-bridge
const int rightDirPin2 = A0;
const int rightPWMPin2 = A1;
const int leftDirPin2 = A2;
const int leftPWMPin2 = A3;

// variable for neopixel matrix sequence
int x    = matrix.width();
int pass = 0;

void setup() {

  // Motor controller pins
  pinMode(rightDirPin1, OUTPUT);
  pinMode(leftDirPin1, OUTPUT);

  pinMode(rightDirPin2, OUTPUT);
  pinMode(leftDirPin2, OUTPUT);


  Serial.begin(115200);
 
  strip.begin(); //for neopixel "faces"
  strip.show();

  matrix.begin(); //for neopixel scrolling text
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);


  // RF24 setup
  if (!radio.begin()) {
    Serial.println("radio  initialization failed");
    while (1)
      ;
  } else {
    Serial.println("radio successfully initialized");
  }
  radio.openReadingPipe(0, address);  // Destination address
  radio.setPALevel(RF24_PA_MIN);      // Min or max
  radio.startListening();             // sets  module as receiver

}
void loop() {


  uint8_t pipeNum;
  if (radio.available(&pipeNum))  //Looking for the data.
  {
    int data;

    Serial.print("data available on pipe ");
    Serial.println(pipeNum);

    radio.read(&data, sizeof(data));  //Reading the data
    Serial.print("data = ");
    Serial.println( data);

    switch (data) {
      case 0b00000001:
        Serial.println("turning right");
        digitalWrite(rightDirPin1, HIGH);
        digitalWrite(rightPWMPin1, LOW);
        digitalWrite(rightDirPin2, LOW);
        digitalWrite(rightPWMPin2, HIGH);
         
        //triggering face on neopixel when the buttons are pressed
        disp_Clear();
        dogFace();
        break;
      case 0b00000010:
        Serial.println("forward");
        digitalWrite(rightDirPin1, HIGH);
        analogWrite(rightPWMPin1, 0);
        digitalWrite(leftDirPin1, LOW);
        analogWrite(leftPWMPin1, 255);

        digitalWrite(rightDirPin2, LOW);
        analogWrite(rightPWMPin2, 255);
        digitalWrite(leftDirPin2, HIGH);
        analogWrite(leftPWMPin2, 0);

        disp_Clear();
        dogFace();
        break;
      case 0b00000100:
        Serial.println("turning left");
        digitalWrite(leftDirPin1, LOW);
        digitalWrite(leftPWMPin1, HIGH);
        digitalWrite(leftPWMPin2, LOW);
        digitalWrite(leftDirPin2, HIGH);

        disp_Clear();
        dogFace();
        break;

      case 0b00001000:
      Serial.println("Rex");
        //bringing in the scrolling text when the "rex" button is pressed
        disp_Clear();
        rex();
        break;

      default:
        Serial.println("invalid code");
        stop();
        digitalWrite(rightDirPin1, LOW);
        digitalWrite(rightPWMPin1, LOW);

        digitalWrite(rightDirPin2, LOW);
        digitalWrite(rightPWMPin2, LOW);
        break;
    }
  } else {
    //Serial.println("stop");
    stop();
  }
  delay(5);
}

void stop() {

  digitalWrite(rightDirPin1, LOW);
  analogWrite(rightPWMPin1, 0);
  digitalWrite(leftDirPin1, LOW);
  analogWrite(leftPWMPin1, 0);

  digitalWrite(rightDirPin2, LOW);
  analogWrite(rightPWMPin2, 0);
  digitalWrite(leftDirPin2, LOW);
  analogWrite(leftPWMPin2, 0);
}

int dogFace () {
  //the specific LEDs that need to be turned on to create a face
  strip.setPixelColor(9, 0, 0, 255);
  strip.setPixelColor(10, 0, 0, 255);
  strip.setPixelColor(13, 0, 0, 255);
  strip.setPixelColor(18, 0, 0, 255);
  strip.setPixelColor(22, 0, 0, 255);
  strip.setPixelColor(33, 0, 0, 255);
  strip.setPixelColor(34, 0, 0, 255);
  strip.setPixelColor(35, 0, 0, 255);
  strip.setPixelColor(36, 0, 0, 255);
  strip.setPixelColor(37, 0, 0, 255);
  strip.setPixelColor(38, 0, 0, 255);
  strip.setPixelColor(42, 0, 0, 255);
  strip.setPixelColor(45, 0, 0, 255);
  strip.setPixelColor(50, 0, 0, 255);
  strip.setPixelColor(53, 0, 0, 255);
  strip.setPixelColor(59, 0, 0, 255);
  strip.setPixelColor(60, 0, 0, 255);
  strip.show();
}

int rex () {
 //the code responsible for the scrolling text
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Hi I am Rex Woof Woof!")); //the text
  if (--x < -150 ) {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show(); 
  delay(100);
}

void disp_Clear() {
  //very basic code I am using to clear the neopixel screen. That way the LEDs being triggered when the robot moves and the LEDs being triggered with the
  //scrolling text do not overlap or interfere with each other. 
  for (int ii = 0; ii < 65; ++ii) {
    strip.setPixelColor(ii, 0, 0, 0);
  }
  strip.show();
}
