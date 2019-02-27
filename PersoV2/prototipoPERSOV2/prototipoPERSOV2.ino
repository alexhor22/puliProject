#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "community-fbbae.firebaseio.com"
#define FIREBASE_AUTH "sho28REDBEcTXyEwxyMGBaI3Zk5lc7jnEhjj2yYJ"
#define WIFI_SSID "CelAlex"//"iPhone de Isabela"//"CelAlex"
#define WIFI_PASSWORD "1234567890"//"isaelizceb"//"1234567890"
#define RED_LED 16
#define BLUE_LED 14
#define GREEN_LED 12
#define VIBRATOR 13

const byte interruptPin = 0;
volatile byte interruptCounter = 0;
int numberOfInterrupts = 0;
int lastStatus = -1;
bool sentAlert = false;
bool mainMode = true;
 
void setup() {

  //GPIO DECLARATION
  pinMode(2,OUTPUT);
  pinMode(RED_LED,OUTPUT); //RED
  pinMode(BLUE_LED,OUTPUT); //BLUE
  pinMode(GREEN_LED,OUTPUT); //GREEN
  pinMode(VIBRATOR,OUTPUT);
  
/////////////CONFIGURACION BASICA//////////////// 
  Serial.begin(115200);
  
  //CONNECT TO WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  int cont = 0;
  bool up = true;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if (up){
      cont = cont + 20;
      if (cont > 255) up = false;
    }
    else{
      cont = cont - 20;
      if (cont < 20) up = true;
    }
    analogWrite(RED_LED,cont);
    analogWrite(BLUE_LED,cont);
    analogWrite(GREEN_LED,cont);
    delay(100);
  }
//  blinkAll();
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  //INTERRUPT
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);




  ///////////////////OTA///////////////////////////////
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(2,LOW);
}
 
void handleInterrupt() {
  interruptCounter++;
  int t = millis();
  while(millis() - t < 250){
    //debounce delay
  }
}
 
void loop() {

///////////////////////////

if (millis() < 60000){
  ArduinoOTA.handle();
}

////////////////



//CHECKING DB
int dbStatus = Firebase.getInt("Board/Status");
if (dbStatus != lastStatus){
   vibrate();
   mainMode = false;
  switch(dbStatus){
    case 1: crimeReceived();
            break;
    case 2: peReceived();
            break;
    case 3: suspiciousReceived();
            break;
    default: okReceived();
             mainMode = true;
  }
}
lastStatus = dbStatus;


if (mainMode){
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
  Firebase.setInt("Board/Status", clicks);
  sentAlert = true;
 }

 clicks = 0;
}
else{
  int t = millis();
  while(digitalRead(interruptPin) == LOW){
    yield();
    if(sentAlert && (millis() - t) > 1500){
       Firebase.setInt("Board/Status", 0);
       vibrateCancel();
       okReceived();
//       vibrate();
       break;
    }
  }
  
  delay(1000);
  interruptCounter = 0;
}



 
}
