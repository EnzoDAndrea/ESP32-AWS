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

### FILL THE NAME of YOUR DEVICE
#define THINGNAME "[YOUR THINGNAME]" 

### FILL AWS ENDPOINT
const char MQTT_HOST[] = "[YOUR AWS MQTT]";


### Download your root certificate, open it in read mode and replace the section between -----BEGIN CERTIFICATE----- and -----END CERTIFICATE-----

static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

### Download your device certificate, open it in read mode and replace the section between -----BEGIN CERTIFICATE----- and -----END CERTIFICATE-----
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";

### Download your device private key, open it in read mode and replace the section between -----BEGIN CERTIFICATE----- and -----END CERTIFICATE-----
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY----- 
)KEY";