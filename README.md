# ESP32-AW: Read data from different sensor and store them in Amazon Web Service IoT
***
Board Manager Libraries:
1. ESP32 by Espressif Systems

Required Libraries:
1. MQTT by Joel Gaehwiler
2. ArduinoJson by Benoit Blanchon
3. DHT sensor library by Adafruit
4. Adafruit BMP280 Library by Adafruit

## Wiring schema
![image description](Wiring/Weather_station_wiring.png)

## File to be modified

ESP32-AWS/ESP32/secrets.h

### FILL YOUR SSID and PASSWORD
const char WIFI_SSID[] = "[YOUR SSIS]";
const char WIFI_PASSWORD[] = "[YOUR PASSWORD]"; 