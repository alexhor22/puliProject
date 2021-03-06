void blinkAll(){
  for(int i=0; i < 2; i++){
    digitalWrite(RED_LED,HIGH);
    digitalWrite(BLUE_LED,HIGH);
    digitalWrite(GREEN_LED,HIGH);
    delay(400);
    digitalWrite(RED_LED,LOW);
    digitalWrite(BLUE_LED,LOW);
    digitalWrite(GREEN_LED,LOW);
    delay(400);
  }
}



void vibrate(){
  Serial.println("vibrate");
  digitalWrite(VIBRATOR,HIGH);
  delay(250);
  digitalWrite(VIBRATOR,LOW);
  delay(250);
  digitalWrite(VIBRATOR,HIGH);
  delay(250);
  digitalWrite(VIBRATOR,LOW);
  delay(250);
}

void vibrateCancel(){
  Serial.println("vibrate");
  digitalWrite(VIBRATOR,HIGH);
  delay(500);
  digitalWrite(VIBRATOR,LOW);

}



void crimeReceived(){
  Serial.println("CRIME");
  digitalWrite(RED_LED,HIGH);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  if (!sentAlert){
    vibrate();
  }
}
void peReceived(){
  Serial.println("PE");
  digitalWrite(RED_LED,LOW);
  digitalWrite(BLUE_LED,HIGH);
  digitalWrite(GREEN_LED,LOW);
  if (!sentAlert){
    vibrate();
  }
}
void suspiciousReceived(){
  Serial.println("SUSPICIOUS");
  digitalWrite(RED_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,HIGH);
  if (!sentAlert){
    vibrate();
  }
}
void okReceived(){
  digitalWrite(RED_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
}
