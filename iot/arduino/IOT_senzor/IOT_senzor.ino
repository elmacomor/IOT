#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
const int sensor_pin = A0;
const int led_pin = D4;
const int led_pin2 = D3;
 


#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define WIFI_SSID "Dzenan"
#define WIFI_PASSWORD "dzenan123"
#define API_KEY "AIzaSyCtzvpoNxHFkB8QkjSaU1L4GuDv0E5Xn-8"
#define DATABASE_URL "iotprojekt-3e594-default-rtdb.firebaseio.com"

 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
//if (Firebase.RTDB.getInt(&fbdo, "/LED/status"))

 

 

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

 

int ledStatus;

void setup(){
   pinMode(led_pin,OUTPUT);
  pinMode(led_pin2,OUTPUT);
  pinMode(sensor_pin, OUTPUT);
  digitalWrite(led_pin,LOW);
  digitalWrite(led_pin2,LOW);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  config.api_key = API_KEY;

 
  config.database_url = DATABASE_URL;

 
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop() {
 int soilMoistureValue = 0;
int percentage = 0;
soilMoistureValue = analogRead(sensor_pin);
percentage = map(soilMoistureValue, 490, 1023, 100, 0);
Serial.println(percentage);
Firebase.RTDB.setFloat(&fbdo,"/Vlaznost/var1",percentage);
delay(1000);

if(percentage<25){
digitalWrite(led_pin,HIGH);
digitalWrite(led_pin2,HIGH);
Firebase.RTDB.setFloat(&fbdo,"/Prskalice/obavijest",1);
}
else{
  Firebase.RTDB.setFloat(&fbdo,"/Prskalice/obavijest",0);
}


if (Firebase.RTDB.getInt(&fbdo, "/LED/status")) {
      if (fbdo.dataType() == "int") {
        ledStatus = fbdo.intData();
        if(ledStatus == 1){
          digitalWrite(led_pin, HIGH);
          digitalWrite(led_pin2, HIGH);
        }
        else if(ledStatus == 0){
          digitalWrite(led_pin, LOW);
          digitalWrite(led_pin2, LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }


    


}
