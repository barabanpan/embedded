#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>

// photoresistor-capacitor pin for Capacitor Charging Curve
#define lightMeasurePin 0

// at start write to RTC (Real-Time Clock) memory finishedSuccesfully=0, at finish =1.
// to start waiting for OTA - click RST button twice. At start it'll have this value 0.
// It will know to wait for OTA for 1 minute, then go to sleep as usual
#define waitForOTAtime 60e6
#define deepSleepTime 300e6
// 60e6 - 60s, 300e6 - 5min

const char* ssid = "SSID_for_IoT";
const char* password = "my_cool_password";
const char* API_URL = "the-link-from-the-deploy-of-the-aws-template//measurement";

ADC_MODE(ADC_VCC);  // for reading voltage value
uint32_t finishedSuccessfully;  // function uses 32 bits

void setup() {
  float voltage = getVoltage();

  Serial.begin(74880);
  Serial.print("VOLTAGE ");
  Serial.println(voltage);

  // check RTC memory, register 64
  ESP.rtcUserMemoryRead(64, &finishedSuccessfully, sizeof(finishedSuccessfully));
  
  if (finishedSuccessfully == 0) {  // WAIT FOR OTA MODE
      // it's RST clicked twice (assumingly), wait for OTA for 1 min
      Serial.println("Wait for OTA.....");
      finishedSuccessfully = 1;
      ESP.rtcUserMemoryWrite(64, &finishedSuccessfully, sizeof(finishedSuccessfully));
      
      WiFi.mode(WIFI_STA); // only client (station), without its own network, saves battery
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
          delay(10);
      }   
          
      ArduinoOTA.begin();
      long startTime = micros(); 
      while (micros() < startTime + waitForOTAtime) {
          ArduinoOTA.handle();
          delay(10);
      }
      Serial.println("No updates");
  } else {   // MEASURE LIGHT MODE
      Serial.println("Measure light....");
      finishedSuccessfully = 0;  // takes some time to get here, wait a sec between clicks
      ESP.rtcUserMemoryWrite(64, &finishedSuccessfully, sizeof(finishedSuccessfully));
   
      WiFi.mode(WIFI_STA); // only client (station), without its own network, saves battery
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
          delay(10);
      }   
      
      // gotta click in such way to get in between those writes???   
      // hence wifi connection part repeats for both if options, to give some time
      // to click so as to catch finishedSuccessfully at 0
      long m = measureLight();
      sendData(m, voltage);
      
      Serial.println("Sent!");
      finishedSuccessfully = 1;
      ESP.rtcUserMemoryWrite(64, &finishedSuccessfully, sizeof(finishedSuccessfully));
  }
  
  Serial.println("Gonna sleep now");
  // deep sleep (GPIO16 and RST have to be connected for this)
  ESP.deepSleep(deepSleepTime); 
}

void loop() {
  // empty, deepSleep restarts the board in setup(), so loop() never runs
}

float getVoltage() {
  float voltage = 0.00f;
  voltage = ESP.getVcc();
  return voltage / 1024.00f;  // in V
}

long measureLight() {
  // Resistor-capacitor timing circuit for Capacitor Charging Curve
  // Emptying the capacitor
  pinMode(lightMeasurePin, OUTPUT);       // set pin as OUTPUT
  digitalWrite(lightMeasurePin, LOW);     // Send 0V (GND)
  delay(50);                // Wait for 50 ms, for capacitor to release all the current into ground

  // Time it
  pinMode(lightMeasurePin, INPUT);        // set pin to INPUT 
  long startTime = micros(); 

  // Wait for capacitor to get filled via photoresistor
  while (digitalRead(lightMeasurePin) == LOW) {
     // just wait for V to get to HIGH
  }

  // Stop, bright light -> faster -> smaller number, dim light -> slower -> bigger number 
  long duration = micros() - startTime;  // Numeric measurement of light
  return duration;
}

void sendData(long light_value, float voltage) {
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();   // skip certificate verification (debugging), without it returns -1 as status code

  http.begin(client, API_URL);
  http.addHeader("Content-Type", "application/json");

  char body[64]; 
  snprintf(body, sizeof(body), "{\"light\":%lu,\"volt\":%.2f}", light_value, voltage);

  int code = http.POST(body);

  http.end();
}

