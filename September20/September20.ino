const int in1Pin = 11;
const int in2Pin = 10;
const int in3Pin = 9;
const int in4Pin = 6;

void setup() {

  pinMode (in1Pin, OUTPUT);
  pinMode (in2Pin, OUTPUT);
  pinMode (in3Pin, OUTPUT);
  pinMode (in4Pin, OUTPUT);

}

void loop() {

  analogWrite (in1Pin, 150);
  analogWrite (in2Pin, LOW);
  analogWrite (in3Pin, 150);
  analogWrite (in4Pin, LOW);
  delay(1500);
  analogWrite (in1Pin, LOW);
  analogWrite (in2Pin, 150);
  analogWrite (in3Pin, LOW);
  analogWrite (in4Pin, 150);
  delay(1500);
}
