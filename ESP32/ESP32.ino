#include <esp_now.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "telemetry_components.h"
#include <MQTTClient.h>
#include <ArduinoJson.h>

#define SLEEP_FACTOR 900000000
RTC_DATA_ATTR int bootCount = 0;

#define AWS_IOT_PUBLISH_TOPIC   "device/0/data"
#define AWS_IOT_SUBSCRIBE_TOPIC "device/0/sub"
 
WiFiClientSecure net;
MQTTClient client = MQTTClient(1024);


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

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
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
  Serial.println("Telemetry to be published:");
  Serial.println(telemetry);
  int str_len = telemetry.length() + 1; 
  // Copy it over 
  telemetry.toCharArray(jsonBuffer, str_len);
  Serial.println("Telemetry to be sent:");
  Serial.println(jsonBuffer);

  if(jsonBuffer == NULL){
    Serial.println("No telemetry data");
    return;      
  }

  String isEmptyTelemtryValue = String(jsonBuffer);
  isEmptyTelemtryValue.trim();
  if (isEmptyTelemtryValue.isEmpty()) {
    Serial.println("Telemetry data is empty");
    return;
  }

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println("Published Telemetry:");
  Serial.println(jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  
  delay(1000);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  
  print_wakeup_reason();
  
  esp_sleep_enable_timer_wakeup(SLEEP_FACTOR);

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
  delay(1000);
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    Serial.println("Publishing in AWS!");
    publishMessage();
    esp_deep_sleep_start();
  }
}