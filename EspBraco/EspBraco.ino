#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <meArm_Adafruit.h>
#include "Constants.h"

/*
 * SDA 4 -> D2 || SCL 5 -> D1
 *
 * #define WIFI_SSID "UFRN_CERES"
 * #define WIFI_PASSWORD "UFRN@1306"
 * #define FIREBASE_HOST "esp8266-90991.firebaseio.com"
 * #define FIREBASE_AUTH ""
*/

meArm arm;

int x = 0;
int y = 180;
int z = 100;
boolean openGripper = true;

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
  Serial.println(Firebase.success());
  Firebase.stream("meArm");
}

void setupBraco()
{
  arm.begin();
  arm.gotoPoint(x, y, z);
  arm.closeGripper();
}

void handleEvent(FirebaseObject firebase) {
    if (Firebase.getBool("meArm/openGripper") != openGripper) {
      openGripper = !openGripper;
      if (openGripper)
        Serial.println("Abrir Garra");
        //arm.openGripper();
      else
        Serial.println("Fechar Garra");
        //arm.closeGripper();
    } else {
      x = Firebase.getInt("meArm/position/x");
      y = Firebase.getInt("meArm/position/y");
      z = Firebase.getInt("meArm/position/z");
      Serial.print("arm.gotoPoint(");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.print(", ");
      Serial.print(z);
      Serial.println(");");

      //arm.gotoPoint(x, y, z);
    }
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
  }
}
