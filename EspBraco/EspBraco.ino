#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "Constants.h"

//#include "meArm_Adafruit.h"
//#include <Adafruit_PWMServoDriver.h>
//#include <Wire.h>

/*
#define WIFI_SSID "UFRN_CERES"
#define WIFI_PASSWORD "UFRN@1306"
#define FIREBASE_HOST "esp8266-90991.firebaseio.com"
#define FIREBASE_AUTH ""
*/

//meArm arm;

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
  Firebase.setInt("led", 0);
  Serial.println(Firebase.success());
  //Firebase.stream("meArm");
  Firebase.stream("led");
}

void setupBraco()
{
  //arm.begin();
  //arm.goToPoint(0, 150, 80);
}

void handleEvent(FirebaseObject firebase) {
  String eventType = firebase.getString("type");
  Serial.println(firebase.getString("path"));
  eventType.toLowerCase();
  //if (eventType == "put") {
    Serial.print("LED: ");
    Serial.print(Firebase.getInt("led"));
    digitalWrite(3, LOW);
    //int x = firebase.getInt("x");
    //int y = firebase.getInt("y");
    //int z = firebase.getInt("z");
    //arm.goToPoint(x, y, z);
  //  }
}

void updatePosition() {
//  Firebase.setInt("meArm/realPosition/x", arm.getX());
//  Firebase.setInt("meArm/realPosition/y", arm.getY());
//    Firebase.setInt("meArm/realPosition/z", arm.getZ());
}

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(3, OUTPUT);

  setupWifi();
  setupFirebase();
  //setupBraco();
}

void loop() {
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }

  if (Firebase.available()) {
    handleEvent(Firebase.readEvent());
    //updatePosition();
  }
}
