#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>

///////////////////////////////////////////////////////////////////
// Set your SSID & Pass for initial configuration
#include "../include/configuration.h" // application configuration
///////////////////////////////////////////////////////////////////

#include "special_chars.h"
#include "webserver.h"

FTPServer ftp;

void connectOk();
void connectFail();

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Debug output to serial
	AppConfig myConfig = loadConfig();
	WifiStation.config(myConfig.NetworkSSID, myConfig.NetworkPassword);
	WifiStation.enable(true);
	WifiAccessPoint.enable(false);
	WifiStation.waitConnection(connectOk, 15, connectFail);
}

void connectOk()
{
	debugf("Connect to AP successful");
	WifiAccessPoint.enable(false);

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123"); // FTP account
	startWebServer();
}

void connectFail()
{
	debugf("Connect to AP failed");
	WifiAccessPoint.config("Sming_AP", "", AUTH_OPEN);
	WifiAccessPoint.enable(true);

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123"); // FTP account
	startWebServer();
}
