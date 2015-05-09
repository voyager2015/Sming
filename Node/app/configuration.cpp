#include "../include/configuration.h"

#include <SmingCore/SmingCore.h>

AppConfig ActiveConfig;

AppConfig loadConfig()
{
	DynamicJsonBuffer jsonBuffer;
	AppConfig cfg;
	if (fileExist(CONFIG_FILE))
	{
		int size = fileGetSize(CONFIG_FILE);
		char* jsonString = new char[size + 1];
		fileGetContent(CONFIG_FILE, jsonString, size + 1);
		JsonObject& root = jsonBuffer.parseObject(jsonString);

		JsonObject& network = root["network"];
		cfg.NetworkSSID = String((const char*)network["ssid"]);
		cfg.NetworkPassword = String((const char*)network["password"]);

		delete[] jsonString;
	}
	else
	{
		cfg.NetworkSSID = WIFI_SSID;
		cfg.NetworkPassword = WIFI_PWD;
	}
	return cfg;
}

void saveConfig(AppConfig& cfg)
{
	ActiveConfig = cfg;

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& network = jsonBuffer.createObject();
	root["network"] = network;
	network["ssid"] = cfg.NetworkSSID.c_str();
	network["password"] = cfg.NetworkPassword.c_str();

	char buf[3048];
	root.prettyPrintTo(buf, sizeof(buf));
	fileSetContent(CONFIG_FILE, buf);
}


