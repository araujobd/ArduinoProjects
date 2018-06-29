#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "Constants.h"

/*
#define WIFI_SSID "Hao123"
#define WIFI_PASSWORD ""
#define FIREBASE_HOST "esp8266-90991.firebaseio.com"
#define FIREBASE_AUTH ""
*/

int led = 0;
void setupWifi()
{
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupFirebase()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("led", led);
  Serial.println(Firebase.success());
}

void setup() {
  Serial.begin(115200);
  delay(10);

  setupWifi();
  setupFirebase();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  led = Firebase.getInt("led");
  Serial.println(led);
  if(!led)
    digitalWrite(LED_BUILTIN, HIGH);
  else 
    digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
