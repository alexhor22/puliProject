//this is master

int state = 0;
const int ledPin = 8;
const int buttonPin = 2;
int buttonState = 1;

void setup()
{
  Serial.begin(38400);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buttonPin, INPUT);
}

void loop()
{
 if(Serial.available() > 0)
 {
    // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }
 // Controlling the LED
 buttonState = digitalRead(buttonPin);
 if (buttonState == LOW)
 {
  Serial.println('1');
 }
 else
 {
  Serial.println('0');
 }

 if (state == '1')
 {
  digitalWrite(ledPin, HIGH); // LED ON
  state = 0;
 }
 else if (state == '0')
 {
  digitalWrite(ledPin, LOW); // LED ON
  state = 0;
 }
}
