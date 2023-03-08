#pragma once
#include <DHT.h>
#include "time.h"
#include <i2cdetect.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "secrets.h"

#include <esp_now.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define dataPin 27
#define DHTType DHT22

DHT dht = DHT(dataPin, DHTType);

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

#define BMP_SDA 21
#define BMP_SCL 22

#define LATITUDE 45.51
#define LONGITUDE 9.23

Adafruit_BMP280 bmp; // I2C
#define BH1750_ADDRESS 0x23
BH1750 bh1750;

static const char rootCAOpenWeatherkey[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIEMjCCAxqgAwIBAgIBATANBgkqhkiG9w0BAQUFADB7MQswCQYDVQQGEwJHQjEb
MBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYDVQQHDAdTYWxmb3JkMRow
GAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UEAwwYQUFBIENlcnRpZmlj
YXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4MTIzMTIzNTk1OVowezEL
MAkGA1UEBhMCR0IxGzAZBgNVBAgMEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE
BwwHU2FsZm9yZDEaMBgGA1UECgwRQ29tb2RvIENBIExpbWl0ZWQxITAfBgNVBAMM
GEFBQSBDZXJ0aWZpY2F0ZSBTZXJ2aWNlczCCASIwDQYJKoZIhvcNAQEBBQADggEP
ADCCAQoCggEBAL5AnfRu4ep2hxxNRUSOvkbIgwadwSr+GB+O5AL686tdUIoWMQua
BtDFcCLNSS1UY8y2bmhGC1Pqy0wkwLxyTurxFa70VJoSCsN6sjNg4tqJVfMiWPPe
3M/vg4aijJRPn2jymJBGhCfHdr/jzDUsi14HZGWCwEiwqJH5YZ92IFCokcdmtet4
YgNW8IoaE+oxox6gmf049vYnMlhvB/VruPsUK6+3qszWY19zjNoFmag4qMsXeDZR
rOme9Hg6jc8P2ULimAyrL58OAd7vn5lJ8S3frHRNG5i1R8XlKdH5kBjHYpy+g8cm
ez6KJcfA3Z3mNWgQIJ2P2N7Sw4ScDV7oL8kCAwEAAaOBwDCBvTAdBgNVHQ4EFgQU
oBEKIz6W8Qfs4q8p74Klf9AwpLQwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQF
MAMBAf8wewYDVR0fBHQwcjA4oDagNIYyaHR0cDovL2NybC5jb21vZG9jYS5jb20v
QUFBQ2VydGlmaWNhdGVTZXJ2aWNlcy5jcmwwNqA0oDKGMGh0dHA6Ly9jcmwuY29t
b2RvLm5ldC9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDANBgkqhkiG9w0BAQUF
AAOCAQEACFb8AvCb6P+k+tZ7xkSAzk/ExfYAWMymtrwUSWgEdujm7l3sAg9g1o1Q
GE8mTgHj5rCl7r+8dFRBv/38ErjHT1r0iWAFf2C3BUrz9vHCv8S5dIa2LX1rzNLz
Rt0vxuBqw8M0Ayx9lt1awg6nCpnBBYurDC/zXDrPbDdVCYfeU0BsWO/8tqtlbgT2
G9w84FoVxp7Z8VlIMCFlA2zs6SFz7JsDoeA3raAVGI/6ugLOpyypEBMs1OUIJqsi
l2D4kF501KKaU73yqWjgom7C12yxow+ev+to51byrvLjKzg6CYG1a4XXvi3tPxq3
smPi9WIsgtRqAEFQ8TmDn5XpNpaYbg==
-----END CERTIFICATE-----
)KEY";

// Preform HTTPS Request
void OpenWeather(String& openweather) {
  const char* host = "api.openweathermap.org";

  WiFiClientSecure *client = new WiFiClientSecure;
  // Assign Root Certificate
  client->setCACert(rootCAOpenWeatherkey);

  //create an HTTPClient instance
  HTTPClient https;
  String url = String("https://") + String(host) + "/data/2.5/weather?lat=" + String(LATITUDE) + "&lon=" + String(LONGITUDE) + "&appid=" + String(OPEN_WEATHER_MAP_API_KEY);
  Serial.println(url);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected WiFi x OpenWeatherAPI!");
    if (https.begin(*client, url)) {
      int httpCode = https.GET();
      if (httpCode > 0) {
        Serial.println("HTTP " + String(httpCode) + " on OpenWeatherAPI " );
        // HTTP header has been send and Server response header has been handled
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // print server response payload
          String payload = https.getString();
          openweather = String(payload.c_str());
        }
      }
      else {
        Serial.println("[HTTPS] GET> api.openweathermap.org ... failed, error");
      }
      https.end();
    }
  }
  client->stop();
}

void SetWeatherCondition(int& weatherId, String& weatherDescription, double& speed, double& deg, double& temperatureow, int& pressureow, int& humidityow) {
    String currentWeather;
    OpenWeather(currentWeather);
    Serial.println(currentWeather);
    StaticJsonDocument<512> filter;

    StaticJsonDocument<512> docOpenWeather;
    filter["weather"] = true;
    filter["wind"] = true;
    filter["main"] = true;
    deserializeJson(docOpenWeather, currentWeather, DeserializationOption::Filter(filter));
    weatherId = docOpenWeather["weather"][0]["id"].as<uint32_t>();
    const char* description = docOpenWeather["weather"][0]["description"];

    weatherDescription = String(description);

    speed = docOpenWeather["wind"]["speed"].as<double>();
    deg = (int) docOpenWeather["wind"]["deg"].as<uint32_t>();
    temperatureow = docOpenWeather["main"]["temp"].as<double>();
    pressureow = (int) docOpenWeather["main"]["pressure"].as<uint32_t>();
    humidityow = (int) docOpenWeather["main"]["humidity"].as<uint32_t>();
}

void SetupTelemetry() {
  Serial.begin(115200);
  Wire.begin();
  i2cdetect();

  dht.begin();

  bh1750.begin();

  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
          "try a different address!"));
      Serial.print("SensorID was: 0x"); 
      Serial.println(bmp.sensorID(), 16);
      Serial.print("        ID of 0x77 probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("        ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
  }

    /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  return now;
}

String GetTelemetry() {
  String telemetry = "";
  // humidity from dht22
  float h = dht.readHumidity();
  Serial.println("Humidity sensors:");
  Serial.println(h);
  // temperature from mbp208
  float t = bmp.readTemperature();
  Serial.println("Temperature sensors:");
  Serial.println(t);
  // pressure from mbp208
  float p = bmp.readPressure();
  Serial.println("Pressure sensors:");
  Serial.println(p);

  float l = bh1750.readLightLevel();
  Serial.println("Light sensors: ");
  Serial.println(l);

  if (isnan(h) || isnan(t) || isnan(p) || isnan(l)) {
      Serial.println("Error sensors.");
      Serial.println("Please check wiring configuration.");
      return "";
  }

  int weatherId = 0;
  String  weatherDescription;

  double speed = 0.0;
  double deg = 0.0;
  double temperatureow = 0.0;
  int pressureow = 0;
  int humidityow = 0;

  SetWeatherCondition(weatherId, weatherDescription, speed, deg, temperatureow, pressureow, humidityow);
  if (weatherId > 0) {
    StaticJsonDocument<256> doc;
    // Variable to save current epoch time
    unsigned long epochTime;
    epochTime = getTime();

    doc["time"] = epochTime;
    doc["city"] = "Milan";
    doc["latitude"] = LATITUDE;
    doc["longitude"] = LONGITUDE;

    doc["speed"] = speed;
    doc["deg"] = deg;

    doc["humidity"] = h;
    doc["temp"] = t;
    doc["pressure"] = p;
    doc["lux"] = l;

    doc["temperatureow"] = temperatureow;
    doc["pressureow"] = pressureow;
    doc["humidityow"] = humidityow;

    doc["weathercode"] = weatherId;
    doc["weatherdescr"] = weatherDescription.c_str();

    serializeJson(doc, telemetry);
  }

  return telemetry;
}