#ifndef INCLUDE_CONFIGURATION_H_
#define INCLUDE_CONFIGURATION_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

// Default SSID and Password here
#define WIFI_SSID ""
#define WIFI_PWD ""

// Pin for communication with DHT sensor
//#define DHT_PIN 1 // UART0 TX pin
#define DHT_PIN 12

// Pin for trigger control output
//#define CONTROL_PIN 3 // UART0 RX pin
#define CONTROL_PIN 15

#define CONFIG_FILE ".conf" // leading point for security reasons :)

struct AppConfig
{
	String NetworkSSID;
	String NetworkPassword;
};

AppConfig loadConfig();
void saveConfig(AppConfig& cfg);
extern void startWebClock();

extern AppConfig ActiveConfig;

#endif /* INCLUDE_CONFIGURATION_H_ */
