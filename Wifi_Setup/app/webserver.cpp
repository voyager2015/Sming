#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "../include/configuration.h"

bool serverStarted = false;
HttpServer server;

void onConfiguration(HttpRequest &request, HttpResponse &response)
{
	AppConfig cfg = loadConfig();
	if (request.getRequestMethod() == RequestMethod::POST)
	{
		debugf("Update config");
		// Update config
		if (request.getPostParameter("SSID").length() > 0) // Network
		{
			cfg.NetworkSSID = request.getPostParameter("SSID");
			cfg.NetworkPassword = request.getPostParameter("Password");
			debugf("SSID set to %s", cfg.NetworkSSID.c_str());
		}
		saveConfig(cfg);
		response.redirect();

		// TODO: currently need to manually reboot for change to take effect, decide best way to handle change of AP credentials
		//WifiStation.config(cfg.NetworkSSID, cfg.NetworkPassword);
	}

	debugf("Send template");
	TemplateFileStream *tmpl = new TemplateFileStream("config.html");
	auto &vars = tmpl->variables();
	vars["SSID"] = cfg.NetworkSSID;
	response.sendTemplate(tmpl);
}

void onAjaxAPs(HttpRequest &request, HttpResponse &response)
{
	AppConfig cfg = loadConfig();
	debugf("current ssid:", cfg.NetworkSSID.c_str());

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();
	JsonArray& aps = json.createNestedArray("aps");
	debugf("ssid count: %d", cfg.SSIDList.count());
	for (int i = 0; i < cfg.SSIDList.count(); i++) {
		aps.add(cfg.SSIDList[i].c_str());
	}
	response.sendJsonObject(stream);
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

void startWebServer()
{
	if (serverStarted)
		return;

	server.listen(80);
	server.addPath("/", onConfiguration);
	server.addPath("/aps",onAjaxAPs);
	server.setDefaultHandler(onFile);
	serverStarted = true;

	if (WifiAccessPoint.isEnabled())
	{
		debugf("AP: %s", WifiAccessPoint.getIP().toString().c_str());
	}
	else if (WifiStation.isEnabled())
	{
		debugf("STA: %s", WifiStation.getIP().toString().c_str());
	}
}
