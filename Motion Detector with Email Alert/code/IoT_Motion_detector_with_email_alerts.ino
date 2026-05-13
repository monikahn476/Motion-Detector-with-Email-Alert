#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "AshwiniP";
const char* password = "12345678";
const char* iftttKey = "9N_-DaKJJz5Tpu4lJeQF8";
const char* event = "motion_detected";

#define PIR_PIN D2
#define LED_PIN D1

unsigned long lastAlert = 0;
const unsigned long cooldown = 30000; // 30 second cooldown

void sendAlert() {
WiFiClientSecure client;
client.setInsecure(); // Skip SSL verification for demo
HTTPClient http;
String url = "https://maker.ifttt.com/trigger/" + String(event) +
"/with/key/" + String(iftttKey);
http.begin(client, url);
http.addHeader("Content-Type", "application/json");
int code = http.POST("{"value1":"Motion detected!"}");
Serial.println("IFTTT response: " + String(code));
http.end();
}

void setup() {
Serial.begin(115200);
pinMode(PIR_PIN, INPUT);
pinMode(LED_PIN, OUTPUT);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
Serial.println("\nWi-Fi connected. System armed.");
}

void loop() {
if (digitalRead(PIR_PIN) == HIGH) {
digitalWrite(LED_PIN, HIGH);
if (millis() - lastAlert > cooldown) {
Serial.println("Motion detected! Sending alert...");
sendAlert();
lastAlert = millis();
}
} else {
digitalWrite(LED_PIN, LOW);
}
delay(200);
}
