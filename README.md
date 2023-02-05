# ESP32-AW: Read data from different sensor and store them in Amazon Web Service IoT (Arduino Sketch)
***
Board Manager Libraries:
1. ESP32 by Espressif Systems

Required Libraries:
1. MQTT by Joel Gaehwiler
2. ArduinoJson by Benoit Blanchon
3. DHT sensor library by Adafruit
4. Adafruit BMP280 Library by Adafruit
5. BH1750 library by Christopher Laws

## Wiring schema
![image description](Wiring/Weather_station_wiring.png)

## File to be modified

ESP32-AWS/ESP32/secrets.h

1. FILL YOUR SSID and PASSWORD
```c++
const char WIFI_SSID[] = "[YOUR SSIS]";
const char WIFI_PASSWORD[] = "[YOUR PASSWORD]";
```
2. FILL THE NAME of YOUR DEVICE
```c++
#define THINGNAME "[YOUR THINGNAME]" 
```
3. FILL AWS ENDPOINT
```c++
const char MQTT_HOST[] = "[YOUR AWS MQTT]";
```
4. Download your root certificate, open it in read mode and replace the section between "BEGIN CERTIFICATE" and "END CERTIFICATE"
```c++
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
[PUT YOUR CERTIFICATE HERE]
-----END CERTIFICATE-----
)EOF";
```

5. Download your device certificate, open it in read mode and replace the section between "BEGIN CERTIFICATE" and "END CERTIFICATE"
```c++
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
[PUT YOUR CERTIFICATE HERE]
-----END CERTIFICATE-----
)KEY";
```

6. Download your device private key, open it in read mode and replace the section between "BEGIN CERTIFICATE" and "END CERTIFICATE"
```c++
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
[PUT YOUR KEY HERE]
-----END RSA PRIVATE KEY----- 
)KEY";
```


7. Replace this section with your registred API KEY from https://openweathermap.org/
```c++
const char OPEN_WEATHER_MAP_API_KEY[] = "[YOUR API KEY]"; 
```
