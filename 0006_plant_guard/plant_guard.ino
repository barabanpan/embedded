// load this to ESP8266 ESP-01
// then keep GPIO0-3 disconnected, touch RESET to GND, connect GPIO0-3 where they are needed
#define BUZZER 3
#define TRIG 0
#define ECHO 2
#define LED 1
#define measureEveryMs 500
#define blinkEveryMs 200
#define distanceThreshold 30
#define DEVICE_API_KEY "same-key-as-in-lambda"
#define API_Endpoint "<APIEndpoint from `sam deploy`>/event"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

uint8_t cm;
unsigned long lastMeasureTime;
unsigned long lastLEDTime;
bool LEDOn;
bool closeSession;  // something is closer than threshold right now
unsigned long closeSessionStart;  // millis() of something getting close
uint8_t minDistance;

const char ssid[] = "wifi-ssid";
const char pass[] = "wifi-pass";

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
  cm = measureDistance();
  lastMeasureTime = millis();
  LEDOn = false;
  closeSession = false;
  lastLEDTime = millis();
  minDistance = distanceThreshold;

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void sendData(uint8_t min_dist, uint16_t duration) {
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();   // skip certificate verification (debugging)

  http.begin(client, API_Endpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("DEVICE_API_KEY", DEVICE_API_KEY);

  char body[64]; 
  snprintf(body, sizeof(body), "{\"min_dist\":%d,\"duration\":%lu}", min_dist, duration);

  int code = http.POST(body);
  http.end();
}

float measureDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  float d = duration * 0.034 / 2;
  return d;
}

void loop() {
  // measure every 500ms
  if (millis() >= lastMeasureTime + measureEveryMs) {
    cm = measureDistance();
    lastMeasureTime = millis();
  }

  // keep light blinking if something is close
  if (cm < distanceThreshold) {
    if (minDistance > cm) minDistance = cm;
    if (!closeSession) {
      // something got close, session starts
      closeSessionStart = millis();
      closeSession = true;
    }
    if (millis() >= lastLEDTime + blinkEveryMs) {
      if (LEDOn == true) {
        digitalWrite(LED, LOW);
        noTone(BUZZER);
        LEDOn = false;
      } else {
        digitalWrite(LED, HIGH);
        tone(BUZZER, 432);
        LEDOn = true;
      }
      lastLEDTime = millis();
    }
  } else {
    if (closeSession) {
      // close session is over
      // TODO: wait a sec for it to continue, maybe
      unsigned long duration = millis() - closeSessionStart;
      closeSession = false;
      sendData(minDistance, (duration + 500) / 1000);
      minDistance = distanceThreshold;
    }
    if (LEDOn == true) {
      digitalWrite(LED, LOW);
      noTone(BUZZER);
      LEDOn = false;
    }
  }
}

