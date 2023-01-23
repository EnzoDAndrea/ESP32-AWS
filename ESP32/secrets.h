#include <pgmspace.h>
 
#define SECRET
 
const char WIFI_SSID[] = "[YOUR SSIS]";
const char WIFI_PASSWORD[] = "[YOUR PASSWORD]"; 
#define THINGNAME "[YOUR THINGNAME]"

// OPENWEATHEMAP Client
const char OPEN_WEATHER_MAP_API_KEY[] = "[YOUR API KEY]"; 


int8_t TIME_ZONE = 1;
 
const char MQTT_HOST[] = "[YOUR AWS MQTT]";

// Copy IOT [ROOT CERTIFICATE]
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
 
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY----- 
)KEY";
