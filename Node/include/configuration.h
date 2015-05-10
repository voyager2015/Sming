#ifndef INCLUDE_CONFIGURATION_H_
#define INCLUDE_CONFIGURATION_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

// Default SSID and Password here
#define WIFI_SSID ""
#define WIFI_PWD ""
#define SOFTAP_SSID "Sming_Device"
#define CONFIG_FILE ".conf" // leading point for security reasons :)

struct AppConfig
{
	String NetworkSSID;
	String NetworkPassword;
	Vector<String> SSIDList;
};

AppConfig loadConfig();
void saveConfig(AppConfig& cfg);
extern void startWebClock();

extern AppConfig ActiveConfig;

#endif /* INCLUDE_CONFIGURATION_H_ */
