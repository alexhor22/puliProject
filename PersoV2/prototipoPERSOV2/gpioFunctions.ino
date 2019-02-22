



void vibrate(){
//  Serial.println("vibrate");
//  digitalWrite(VIBRATOR,LOW);
//  delay(250);
//  digitalWrite(VIBRATOR,HIGH);
//  delay(250);
//  digitalWrite(VIBRATOR,LOW);
//  delay(250);
//  digitalWrite(VIBRATOR,HIGH);
//  delay(250);
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
