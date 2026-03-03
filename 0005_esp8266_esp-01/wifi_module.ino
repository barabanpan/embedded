#include <ESP8266WiFi.h>

const char* ssid = "ssid here";
const char* password = "password here";

WiFiServer server(80);
const int ledPin = 2;  // GPIO2

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // 192.168.0.177

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  // Wait for a full HTTP request
  while (!client.available()) delay(1);
  String req = client.readStringUntil('\r');
  client.flush();

  // Parse LED command
  if (req.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
  } else if (req.indexOf("/LED=OFF") != -1) {
    digitalWrite(ledPin, LOW);
  }

  // Send HTML page
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML><html>");
  client.println("<h2>ESP8266 LED Control</h2>");
  client.println("<a href=\"/LED=ON\"><button>Turn ON</button></a>");
  client.println("<a href=\"/LED=OFF\"><button>Turn OFF</button></a>");
  client.println("</html>");
}
