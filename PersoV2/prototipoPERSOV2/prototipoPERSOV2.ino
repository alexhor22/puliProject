#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "tests01.firebaseio.com"
#define FIREBASE_AUTH "0vzFDt05tpGHIzwa1QxiWMrUSO7vPOnwlHUUsUXi"
#define WIFI_SSID "INFINITUMEB9C_2.4"
#define WIFI_PASSWORD "QPfKbXs9kN"
#define RED_LED 16
#define BLUE_LED 14
#define GREEN_LED 12
#define VIBRATOR 13

const byte interruptPin = 0;
volatile byte interruptCounter = 0;
int numberOfInterrupts = 0;
int lastStatus = -1;
bool sentAlert = false;
 
void setup() {
 
  Serial.begin(115200);
  
  //CONNECT TO WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  //INTERRUPT
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

  //GPIO DECLARATION
  pinMode(2,OUTPUT);
  pinMode(RED_LED,OUTPUT); //RED
  pinMode(BLUE_LED,OUTPUT); //BLUE
  pinMode(GREEN_LED,OUTPUT); //GREEN
  pinMode(VIBRATOR,OUTPUT);
 
}
 
void handleInterrupt() {
  interruptCounter++;
  int t = millis();
  while(millis() - t < 250){
    //debounce delay
  }
}
 
void loop() {
sentAlert = false;
//CHECKING CLICKS
bool exitLoop = false;
int clicks = 0;

 while (!exitLoop){
  int t = millis();
  for(int i = millis(); i-t < 1000; i = millis()){
    if (interruptCounter>0){
      interruptCounter--;
      exitLoop = false;
      clicks++;
      Serial.println("CLICK");
      break;
    }
    else{
      exitLoop = true;
    }
  }
  yield();
 }
//SEND TO DB 
 if (clicks > 0 && clicks <  4 && clicks != lastStatus){
   Serial.println("Number of Clicks:");
  Serial.println(clicks);
  Firebase.setInt("LED/status", clicks);
  sentAlert = true;
 }

 clicks = 0;



//CHECKING DB
int dbStatus = Firebase.getInt("LED/status");
if (dbStatus != lastStatus){
  switch(dbStatus){
    case 1: crimeReceived();
            break;
    case 2: peReceived();
            break;
    case 3: suspiciousReceived();
            break;
    default: okReceived();
  }
}
lastStatus = dbStatus;
 
 
}
