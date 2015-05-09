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
		Serial.println("Update config");
		// Update config
		if (request.getPostParameter("SSID").length() > 0) // Network
		{
			cfg.NetworkSSID = request.getPostParameter("SSID");
			cfg.NetworkPassword = request.getPostParameter("Password");
		}

		saveConfig(cfg);
		response.redirect();
	}

	debugf("Send template");
	TemplateFileStream *tmpl = new TemplateFileStream("config.html");
	auto &vars = tmpl->variables();
	vars["SSID"] = cfg.NetworkSSID;
	response.sendTemplate(tmpl);
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

/// FileSystem Initialization ///

	HttpClient downloadClient;
	int dowfid = 0;
	Timer downloadTimer;
void downloadContentFiles()
{
	if (!downloadTimer.isStarted())
	{
		downloadTimer.initializeMs(3000, downloadContentFiles).start();
	}

	if (downloadClient.isProcessing()) return; // Please, wait.
	Serial.println("DownloadContentFiles");

	if (downloadClient.isSuccessful())
	dowfid++;// Success. Go to next file!
	downloadClient.reset();// Reset current download status

	if (dowfid == 0)
	downloadClient.downloadFile("http://danielwalters.duckdns.org/config.html", "config.html");
	else if (dowfid == 1)
	downloadClient.downloadFile("http://danielwalters.duckdns.org/bootstrap.css.gz","bootstrap.css.gz");
	else if (dowfid == 2)
	downloadClient.downloadFile("http://danielwalters.duckdns.org/jquery.js.gz","jquery.js.gz");
	else
	{
		// Content download was completed
		downloadTimer.stop();
		startWebServer();
	}
}
