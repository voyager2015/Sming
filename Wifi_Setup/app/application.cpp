#include <user_config.h>
#include <SmingCore/SmingCore.h>

///////////////////////////////////////////////////////////////////
// Set your SSID & Pass for initial configuration
#include "../include/configuration.h" // application configuration
///////////////////////////////////////////////////////////////////

#include "special_chars.h"
#include "webserver.h"

FTPServer ftp;

void connectOk();
void connectFail();
void listNetworks(bool succeeded, BssList list);

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // SERIAL_BAUD_RATE has been set to 74880 in user_config.h
	Serial.systemDebugOutput(true); // Debug output to serial

	// load saved config for AP;enable STA mode ensure softAP mode is disabled
	AppConfig myConfig = loadConfig();
	WifiStation.config(myConfig.NetworkSSID, myConfig.NetworkPassword);
	WifiStation.enable(true);
	WifiAccessPoint.enable(false);

	// start scan for wifi networks and wait to see if we can connect or not
	WifiStation.startScan(listNetworks);
	WifiStation.waitConnection(connectOk, 15, connectFail);
}

void connectOk()
{
	debugf("Connect to AP successful");

	// disable softAP mode
	WifiAccessPoint.enable(false);

	// start webserver
	startWebServer();
}

void connectFail()
{
	debugf("Connect to AP failed");

	// config softAP mode
	WifiAccessPoint.config(SOFTAP_SSID, "", AUTH_OPEN);
	WifiAccessPoint.enable(true);

	// start webserver
	startWebServer();
}

// Will be called when WiFi station network scan was completed
void listNetworks(bool succeeded, BssList list)
{
	if (!succeeded)
	{
		debugf("Failed to scan networks");
		return;
	}

	AppConfig myConfig = loadConfig();
	myConfig.SSIDList.clear();

	for (int i = 0; i < list.count(); i++)
	{
		if (list[i].ssid != "") {
			myConfig.SSIDList.add(list[i].ssid);
		}
	}
	saveConfig(myConfig);
}
