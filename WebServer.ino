// --------------------------------------------------------------------------------------------------------------------
// <copyright file="WebServer.ino" company="DTV-Online">
//  Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//  Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// <summary>
//  All functions relating to the Web server. Note that this file is merged with all other '.ino' files.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

/// <summary>
///  Helper function to convert Request::MethodType enums to String.
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <returns>Method type string</returns>
String getMethod(Request& request)
{
	Request::MethodType method = request.method();

	if (method == Request::GET) return String("GET");
	if (method == Request::HEAD) return String("HEAD");
	if (method == Request::POST) return String("POST");
	if (method == Request::PUT) return String("PUT");
	if (method == Request::DELETE) return String("DELETE");
	if (method == Request::PATCH) return String("PATCH");
	if (method == Request::OPTIONS) return String("OPTIONS");
	if (method == Request::ALL) return String("ALL");
	if (method == Request::USE) return String("USE");

	return String();
}

/// <summary>
///  Helper function to convert selected Request paths to SPIFFS file path.
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <returns>SPIIFS file path</returns>
String getFilePath(Request& request)
{
	String path(request.path());

	if (path == "/") return String("/index.html");
	if (path == "/home") return String("/index.html");
	if (path == "/info") return String("/info.html");
	if (path == "/config") return String("/config.html");
	if (path == "/about") return String("/about.html");
	if (path == "/error") return String("/error.html");

	return String(path);
}

/// <summary>
///  Sets the global error info and redirects to the error page.
/// </summary>
/// <param name="response">Reference to the Response instance</param>
/// <param name="code">The error code</param>
/// <param name="message">The error message</param>
void setError(Response& response, int code, String message)
{
	String str = "Error " + String(code) + String(": ") + message + CR;
	Log.error(str.c_str());

	error.Code = code;
	error.Message = message;
	String path("/error.html");

	if (!SPIFFS.exists(path.c_str())) {
		Log.warning("getFile() file does not exist" CR);
		response.sendStatus(404);
		return;
	}

	File file = SPIFFS.open(path);

	if (file.isDirectory()) {
		Log.warning("getFile() directory not supported" CR);
		response.sendStatus(400);
		file.close();
		return;
	}

	response.set("Content-Type", "text/html");
	response.set("Cache-Control", "no-cache");

	while (file.available()) {
		response.write(file.read());
	}

	response.end();
	file.close();
}

/// <summary>
///  Middleware handler to log request infos, and redirect to error page for not found error (404).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void checkRequest(Request& request, Response& response)
{
	String path(request.path());

	Log.trace("checkRequest() %s %s" CR, getMethod(request), request.path());

	// Ignore map files.
	if ((path == "/js/bootstrap.min.js.map") ||
		(path == "/css/bootstrap.min.css.map") ||
		(path == "/css/bootstrap-grid.min.css.map") ||
		(path == "/css/bootstrap-reboot.min.css.map") ||
		(path == "/css/soilmonitor.min.css.map") ||
		(path == "/js/bootstrap.min.js.map") ||
		(path == "/js/bootstrap.bundle.min.js.map") ||
		(path == "/js/popper.min.js.map") ||
		(path == "/js/jquery-3.4.1.min.js.map") ||
		(path == "/js/jquery.inputmask.min.js.map") ||
		(path == "/js/raphael-2.1.4.min.js.map") ||
		(path == "/js/justgage.min.js.map"))
	{
		return;
	}

	// Accept files and JSON requests.
	if ((path == "/") ||
		(path == "/home") ||
		(path == "/info") ||
		(path == "/config") ||
		(path == "/about") ||
		(path == "/error") ||
		(path == "/favicon.ico") ||
		(path == "/css/bootstrap.min.css") ||
		(path == "/css/bootstrap-grid.min.css") ||
		(path == "/css/bootstrap-reboot.min.css") ||
		(path == "/css/soilmonitor.min.css") ||
		(path == "/js/bootstrap.min.js") ||
		(path == "/js/bootstrap.bundle.min.js") ||
		(path == "/js/popper.min.js") ||
		(path == "/js/jquery-3.4.1.min.js") ||
		(path == "/js/jquery.inputmask.min.js") ||
		(path == "/js/raphael-2.1.4.min.js") ||
		(path == "/js/justgage.min.js") ||
		(path == "/ap") ||
		(path == "/sta") ||
		(path == "/err") ||
		(path == "/server") ||
		(path == "/system") ||
		(path == "/data") ||
		(path == "/soil") ||
		(path == "/temp") ||
		 path.startsWith("/soil/") ||
		 path.startsWith("/temp/") ||
		(path == "/settings") ||
		(path == "/settings/ap") ||
		(path == "/settings/sta") ||
		(path == "/settings/log") ||
		(path == "/settings/cmd") ||
		(path == "/settings/soil") ||
		(path == "/settings/temp") ||
		 path.startsWith("/settings/soil/") ||
		 path.startsWith("/settings/temp/") ||
		(path == "/save") ||
		(path == "/reset") ||
		(path == "/reboot"))
	{
		return;
	}
	else
	{
		// Redirect to error page.
		setError(response, 404, "Sorry, an error has occured: The requested resource '" + path + String("' has not been found!"));
	}
}

/// <summary>
///  Middleware handler to return a SPIFFS file.
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getFile(Request& request, Response& response)
{
	if (request.method() != Request::GET)
	{
		Log.warning("getFile() only GET supported" CR);
		response.sendStatus(400);
		return;
	}

	String path = getFilePath(request);

	if (!SPIFFS.exists(path.c_str())) {
		Log.warning("getFile() file does not exist" CR);
		response.sendStatus(404);
		return;
	}

	File file = SPIFFS.open(path);

	if (file.isDirectory()) {
		Log.warning("getFile() directory not supported" CR);
		response.sendStatus(400);
		file.close();
		return;
	}

	const char* mimeType = MimeTypes::getType(file.name());
	response.set("Content-Type", mimeType);

	if (mimeType == "text/html") response.set("Cache-Control", "no-cache");
	if (mimeType == "text/css")response.set("Cache-Control", "max-age=31536000");
	if (mimeType == "image/x-icon")response.set("Cache-Control", "max-age=31536000");
	if (mimeType == "application/javascript")response.set("Cache-Control", "max-age=31536000");

	while (file.available()) {
		response.write(file.read());
	}

	response.end();
	file.close();
}

/// <summary>
///  Middleware handler to return ApInfo (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getApInfo(Request& request, Response& response)
{
	ApInfo info(WiFi);
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(info.serialize());
}

/// <summary>
///  Middleware handler to return StaInfo (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getStaInfo(Request& request, Response& response)
{
	StaInfo info(WiFi);
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(info.serialize());
}

/// <summary>
///  Middleware handler to return ErrInfo (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getErrInfo(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(error.serialize());
}

/// <summary>
///  Middleware handler to return ServerInfo (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getServerInfo(Request& request, Response& response)
{
	ServerInfo info(WiFi);
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(info.serialize());
}

/// <summary>
///  Middleware handler to return SystemInfo (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getSystemInfo(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(sysInfo.serialize());
}

/// <summary>
///  Middleware handler to return all sensor data (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getData(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(sensors.serialize());
}

/// <summary>
///  Middleware handler to return soil sensor data (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getSoil(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(sensors.SoilSensors.serialize());
}

/// <summary>
///  Middleware handler to return temperature sensor data (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getTemp(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(sensors.TempSensors.serialize());
}

/// <summary>
///  Middleware handler to return single soil sensor data (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getSoilByIndex(Request& request, Response& response)
{
	char index[64];

	request.route("i", index, 64);
	short i = index[0] - 48;
	Log.trace("getSoilByIndex() => %s" CR, index);

	if ((i >= 0) && (i < SoilSensors::MAX_SENSORS))
	{
		response.status(200);
		response.set("Content-Type", "application/json");
		response.print(sensors.SoilSensors.serializeByIndex(i));
	}
	else
	{
		Log.warning("getSoilByIndex() invalid index" CR);
		response.sendStatus(404);
	}
}

/// <summary>
///  Middleware handler to return single temperature sensor data (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getTempByIndex(Request& request, Response& response)
{
	char index[64];

	request.route("i", index, 64);
	short i = index[0] - 48;
	Log.trace("getTempByIndex() => %s" CR, index);

	if ((i >= 0) && (i < TempSensors::MAX_SENSORS))
	{
		response.status(200);
		response.set("Content-Type", "application/json");
		response.print(sensors.TempSensors.serializeByIndex(i));
	}
	else
	{
		Log.warning("getTempByIndex() invalid index" CR);
		response.sendStatus(404);
	}
}

/// <summary>
///  Middleware handler to return all settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.serialize());
}

/// <summary>
///  Middleware handler to return access point settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getApSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.ApSettings.serialize());
}

/// <summary>
///  Middleware handler to return WiFi station settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getStaSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.StaSettings.serialize());
}

/// <summary>
///  Middleware handler to return log settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getLogSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.LogSettings.serialize());
}

/// <summary>
///  Middleware handler to return commander settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getCmdSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.CmdSettings.serialize());
}

/// <summary>
///  Middleware handler to return soil sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getSoilSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.SoilSettings.serialize());
}

/// <summary>
///  Middleware handler to return temperature sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getTempSettings(Request& request, Response& response)
{
	response.status(200);
	response.set("Content-Type", "application/json");
	response.print(settings.TempSettings.serialize());
}

/// <summary>
///  Middleware handler to return single soil sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getSoilSettingsByIndex(Request& request, Response& response)
{
	char index[64];

	request.route("i", index, 64);
	short i = index[0] - 48;
	Log.trace("getSoilSettingsByIndex() => %s" CR, index);

	if ((i >= 0) && (i < SoilSensors::MAX_SENSORS))
	{
		response.status(200);
		response.set("Content-Type", "application/json");
		response.print(settings.SoilSettings.serializeByIndex(i));
	}
	else
	{
		Log.warning("getSoilSettingsByIndex() invalid index" CR);
		response.sendStatus(404);
	}
}

/// <summary>
///  Middleware handler to return single temperature sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void getTempSettingsByIndex(Request& request, Response& response)
{
	char index[64];

	request.route("i", index, 64);
	short i = index[0] - 48;
	Log.trace("getTempSettingsByIndex() => %s" CR, index);

	if ((i >= 0) && (i < TempSensors::MAX_SENSORS))
	{
		response.status(200);
		response.set("Content-Type", "application/json");
		response.print(settings.TempSettings.serializeByIndex(i));
	}
	else
	{
		Log.warning("getTempSettingsByIndex() invalid index" CR);
		response.sendStatus(404);
	}
}

/// <summary>
///  Middleware handler to set ErrInfo (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postErrInfo(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postErrInfo() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (error.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(error.serialize());
	}
	else
	{
		Log.error("postErrInfo() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to save settings.
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postSave(Request& request, Response& response)
{
	response.status(202);
	response.set("Content-Type", "text/plain");
	response.print("SoilMonitor save settings");
	settings.ApSettings.reset();
	settings.StaSettings.reset();
	settings.save();
}

/// <summary>
///  Middleware handler to reset WiFi settings.
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postReset(Request& request, Response& response)
{
	response.status(202);
	response.set("Content-Type", "text/plain");
	response.print("SoilMonitor reset WiFi");
	settings.ApSettings.reset();
	settings.StaSettings.reset();
	settings.save();
}

/// <summary>
///  Middleware handler to reset ESP32 (reboot).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postReboot(Request& request, Response& response)
{
	response.status(202);
	response.set("Content-Type", "text/plain");
	response.print("SoilMonitor rebooting");
	led = JLed(LED_BUILTIN).Blink(100, 100).Forever();
	rebootTimer.start();
}

/// <summary>
///  Middleware handler to set all settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postSettings(Request& request, Response& response)
{
	byte buffer[4096];
	int len = request.left();

	if (!request.body(buffer, 4096))
	{
		Log.error("postSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.serialize());
	}
	else
	{
		Log.error("postSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set access point settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postApSettings(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postApSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.ApSettings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.ApSettings.serialize());
	}
	else
	{
		Log.error("postApSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set WiFi station settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postStaSettings(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postStaSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.StaSettings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.StaSettings.serialize());
	}
	else
	{
		Log.error("postStaSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set log settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postLogSettings(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postLogSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.LogSettings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.LogSettings.serialize());
	}
	else
	{
		Log.error("postLogSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set commander settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postCmdSettings(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postCmdSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.CmdSettings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.CmdSettings.serialize());
	}
	else
	{
		Log.error("postCmdSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set soil sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postSoilSettings(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postSoilSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.SoilSettings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.SoilSettings.serialize());
	}
	else
	{
		Log.error("postSoilSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set temperature sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postTempSettings(Request& request, Response& response)
{
	byte buffer[1024];
	int len = request.left();

	if (!request.body(buffer, 1024))
	{
		Log.error("postTempSettings() error in reading body" CR);
		return response.sendStatus(400);
	}

	String json = String((char*)buffer).substring(0, len);

	if (settings.TempSettings.deserialize(json))
	{
		response.status(202);
		response.set("Content-Type", "application/json");
		response.print(settings.TempSettings.serialize());
	}
	else
	{
		Log.error("postTempSettings() error in reading JSON" CR);
		return response.sendStatus(400);
	}
}

/// <summary>
///  Middleware handler to set single soil sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postSoilSettingsByIndex(Request& request, Response& response)
{
	char index[64];

	request.route("i", index, 64);
	short i = index[0] - 48;
	Log.trace("postSoilSettingsByIndex() => %s" CR, index);

	if ((i >= 0) && (i < SoilSensors::MAX_SENSORS))
	{
		byte buffer[1024];
		int len = request.left();

		if (!request.body(buffer, 1024))
		{
			Log.error("postSoilSettingsByIndex() error in reading body" CR);
			return response.sendStatus(400);
		}

		String json = String((char*)buffer).substring(0, len);

		if (settings.SoilSettings.deserializeByIndex(i, json))
		{
			response.status(202);
			response.set("Content-Type", "application/json");
			response.print(settings.SoilSettings.serializeByIndex(i));
		}
		else
		{
			Log.error("postSoilSettingsByIndex() error in reading JSON" CR);
			return response.sendStatus(400);
		}
	}
	else
	{
		Log.warning("postSoilSettingsByIndex() invalid index" CR);
		response.sendStatus(404);
	}
}

/// <summary>
///  Middleware handler to set single temperature sensor settings (JSON).
/// </summary>
/// <param name="request">Reference to the Request instance</param>
/// <param name="response">Reference to the Response instance</param>
void postTempSettingsByIndex(Request& request, Response& response)
{
	char index[64];

	request.route("iPostTemp", index, 64);
	short i = index[0] - 48;
	Log.trace("postTempSettingsByIndex() => %s" CR, index);

	if ((i >= 0) && (i < TempSensors::MAX_SENSORS))
	{
		byte buffer[1024];
		int len = request.left();

		if (!request.body(buffer, 1024))
		{
			Log.error("postTempSettingsByIndex() error in reading body" CR);
			return response.sendStatus(400);
		}

		String json = String((char*)buffer).substring(0, len);

		if (settings.TempSettings.deserializeByIndex(i, json))
		{
			response.status(202);
			response.set("Content-Type", "application/json");
			response.print(settings.TempSettings.serializeByIndex(i));
		}
		else
		{
			Log.error("postTempSettingsByIndex() error in reading JSON" CR);
			return response.sendStatus(400);
		}
	}
	else
	{
		Log.warning("postTempSettingsByIndex() invalid index" CR);
		response.sendStatus(404);
	}
}

/// <summary>
///  Initializes and starts the HTTP web server.                 
/// </summary>
void initServer()
{
	// Setup middleware handler for logging and not found error handling.
	app.use(&checkRequest);

	// Setup handlers for bootstrap Web pages.
	app.get("/", &getFile);
	app.get("/home", &getFile);
	app.get("/info", &getFile);
	app.get("/config", &getFile);
	app.get("/about", &getFile);
	app.get("/error", &getFile);

	// Setup handlers for various resources.
	app.get("/favicon.ico", &getFile);

	app.get("/css/bootstrap.min.css", &getFile);
	app.get("/css/bootstrap-grid.min.css", &getFile);
	app.get("/css/bootstrap-reboot.min.css", &getFile);
	app.get("/css/soilmonitor.min.css", &getFile);

	app.get("/js/bootstrap.min.js", &getFile);
	app.get("/js/bootstrap.bundle.min.js", &getFile);
	app.get("/js/popper.min.js", &getFile);
	app.get("/js/jquery-3.4.1.min.js", &getFile);
	app.get("/js/jquery.inputmask.min.js", &getFile);
	app.get("/js/raphael-2.1.4.min.js", &getFile);
	app.get("/js/justgage.min.js", &getFile);

	// Setup handlers for JSON GET requests.
	app.get("/ap", &getApInfo);
	app.get("/sta", &getStaInfo);
	app.get("/err", &getErrInfo);
	app.get("/server", &getServerInfo);
	app.get("/system", &getSystemInfo);
	app.get("/data", &getData);
	app.get("/soil", &getSoil);
	app.get("/temp", &getTemp);
	app.get("/soil/:i", &getSoilByIndex);
	app.get("/temp/:i", &getTempByIndex);

	app.get("/settings", &getSettings);
	app.get("/settings/ap", &getApSettings);
	app.get("/settings/sta", &getStaSettings);
	app.get("/settings/log", &getLogSettings);
	app.get("/settings/cmd", &getCmdSettings);
	app.get("/settings/soil", &getSoilSettings);
	app.get("/settings/temp", &getTempSettings);
	app.get("/settings/soil/:i", &getSoilSettingsByIndex);
	app.get("/settings/temp/:i", &getTempSettingsByIndex);

	// Setup handlers for JSON POST requests.
	app.post("/err", &postErrInfo);
	app.post("/save", &postSave);
	app.post("/reset", &postReset);
	app.post("/reboot", &postReboot);

	app.post("/settings", &postSettings);
	app.post("/settings/ap", &postApSettings);
	app.post("/settings/sta", &postStaSettings);
	app.post("/settings/log", &postLogSettings);
	app.post("/settings/cmd", &postCmdSettings);
	app.post("/settings/soil", &postSoilSettings);
	app.post("/settings/temp", &postTempSettings);
	app.post("/settings/soil/:i", &postSoilSettingsByIndex);
	app.post("/settings/temp/:i", &postTempSettingsByIndex);

	server.begin();
}
