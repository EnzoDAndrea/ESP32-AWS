#include <esp_now.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "telemetry_components.h"
#include <MQTTClient.h>
#include <ArduinoJson.h>



#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
 
WiFiClientSecure net;
MQTTClient client = MQTTClient(256);


time_t now;
time_t nowish = 1510592825;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;

void NTPConnect(void)
{
  Serial.println();
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.print(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  NTPConnect();
 
  net.setCACert(cacert);
  net.setCertificate(client_cert);
  net.setPrivateKey(privkey);
 
  client.begin(MQTT_HOST, 8883, net);
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  client.onMessage(messageHandler);
  Serial.println();
 
  Serial.print("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  Serial.println();
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  Serial.println();
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println();
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  char jsonBuffer[1024];
  String telemetry = GetTelemetry();

  int str_len = telemetry.length() + 1; 
  // Copy it over 
  telemetry.toCharArray(jsonBuffer, str_len);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  
  delay(60000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("Assigned IP: ");
  Serial.println(WiFi.localIP());
  connectAWS();
  SetupTelemetry();
}

void loop() {
  delay(10000);
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();
      publishMessage();
    }
  }
}