// load this to EXP8266 ESP-01
// then keep GPIO0 disconnected, touch RESET to GND, connect GPIO0 where it's needed
// No LED or buzzer to keep RX and TX for debug prints
#define TRIG 0
#define ECHO 2
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
bool closeSession;  // something is closer than threshold right now
unsigned long closeSessionStart;  // millis() of something getting close
uint8_t minDistance;

const char ssid[] = "wifi-ssid";
const char pass[] = "wifi-pass";

void setup() {
  Serial.begin(74880);
  Serial.print("SETUP");

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  cm = measureDistance();
  lastMeasureTime = millis();
  closeSession = false;
  minDistance = distanceThreshold;

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("Connected to WiFi!");
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

  Serial.print("body=");
  Serial.println(body);
  int code = http.POST(body);

  if (code == 201) {
    Serial.println("Sent, 201");
  } else {
    Serial.print("ERROR, status code = ");
    Serial.println(code);
  }

  http.end();
}

float measureDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  float d = duration * 0.034 / 2;
  Serial.print(duration);
  Serial.print(" duration -> cm ");
  Serial.println(d);
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
      Serial.println("Close session starts...");
    }
  } else {
    if (closeSession) {
      // close session is over
      // TODO: wait a sec for it to continue, maybe
      unsigned long duration = millis() - closeSessionStart;
      Serial.print("Close session duration = ");
      Serial.print(duration);
      Serial.print("ms, minDistance = ");
      Serial.print(minDistance);
      Serial.println("cm");
      closeSession = false;
      sendData(minDistance, (duration + 500) / 1000);
      minDistance = distanceThreshold;
    }
  }
}

