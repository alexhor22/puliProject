int getAPIStatus(){ //pass devide ID
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, "http://puliapi.herokuapp.com/getStatus?colonia=Primavera")) { //"colonia" hardcoded, TO BE CHANGED
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String json = http.getString();
            DeserializationError error = deserializeJson(doc, json);
             if (error) {
                return 0;
              }
              else{
                int s = doc["status"];
//                Serial.println(s);
                return s;
    
              }
        }
      } else {
//        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        return 0;
      }

      http.end();
  }
  return 0;
}

void changeAPIStatus(int stat){
  WiFiClient client;
  HTTPClient http;
  char cstat = 48 + stat;  
  
  String url = "http://puliapi.herokuapp.com/changeStatus?colonia=Primavera&status=";
  url += stat;
//  Serial.print("char status");
  Serial.println(url);

//  String temp = strcat("http://puliapi.herokuapp.com/changeStatus?colonia=Primavera&status=", cstat);
//    Serial.println(temp);
  http.begin(client, url); //"colonia" hardcoded, TO BE CHANGED
  http.GET();
//  temp = url;   
  http.end();
}
