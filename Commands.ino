// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Commands.ino" company="DTV-Online">
//  Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//  Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// <summary>
//  All functions relating to the Commander. Note that this file is merged with all other '.ino' files.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

/// <summary>
///  Command Handler Function showing AP info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool apHandler(Commander& cmdr)
{
	Log.trace("apHandler()" CR);
	ApInfo info(WiFi);

	cmdr.println("WiFi Accesspoint Info:");
	cmdr.print("    Active:    "); cmdr.println(ApInfo::Active ? "true" : "false");
	cmdr.print("    SSID:      "); cmdr.println(info.SSID);
	cmdr.print("    PASS:      "); cmdr.println(info.PASS);
	cmdr.print("    Hostname:  "); cmdr.println(info.Hostname);
	cmdr.print("    NetworkID: "); cmdr.println(info.NetworkID);
	cmdr.print("    Address:   "); cmdr.println(info.Address);
	cmdr.print("    Clients:   "); cmdr.println(info.Clients);
	cmdr.print("    MAC:       "); cmdr.println(info.MAC);

	return 0;
}

/// <summary>
///  Command Handler Function showing WiFi station info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool staHandler(Commander& cmdr)
{
	Log.trace("staHandler()" CR);
	StaInfo info(WiFi);

	cmdr.println("WiFi Connection Info:");
	cmdr.print("    Active:    "); cmdr.println(info.Active ? "true" : "false");
	cmdr.print("    SSID:      "); cmdr.println(info.SSID);
	cmdr.print("    PASS:      "); cmdr.println(info.PASS);
	cmdr.print("    NetworkID: "); cmdr.println(info.NetworkID);
	cmdr.print("    Hostname:  "); cmdr.println(info.Hostname);
	cmdr.print("    Address:   "); cmdr.println(info.Address);
	cmdr.print("    Gateway:   "); cmdr.println(info.Gateway);
	cmdr.print("    Subnet:    "); cmdr.println(info.Subnet);
	cmdr.print("    DNS:       "); cmdr.println(info.DNS);
	cmdr.print("    RSSI(dBm): "); cmdr.println(info.RSSI);
	cmdr.print("    BSSID:     "); cmdr.println(info.BSSID);
	cmdr.print("    MAC:       "); cmdr.println(info.MAC);

	return 0;
}

/// <summary>
///  Command Handler Function showing REST API info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool apiHandler(Commander& cmdr)
{
	Log.trace("apiHandler()" CR);

	cmdr.println("HTTP Web API:");
	cmdr.println("    GET:");
	cmdr.println("        /ap              ");
	cmdr.println("        /sta             ");
	cmdr.println("        /err             ");
	cmdr.println("        /server          ");
	cmdr.println("        /system          ");
	cmdr.println("        /soil            ");
	cmdr.println("        /temp            ");
	cmdr.println("        /data            ");
	cmdr.println("        /settings        ");
	cmdr.println("        /settings/ap     ");
	cmdr.println("        /settings/sta    ");
	cmdr.println("        /settings/log    ");
	cmdr.println("        /settings/cmd    ");
	cmdr.println("        /settings/soil   ");
	cmdr.println("        /settings/temp   ");
	cmdr.println("    POST:");
	cmdr.println("        /err             ");
	cmdr.println("        /save            ");
	cmdr.println("        /reset           ");
	cmdr.println("        /reboot          ");
	cmdr.println("        /settings        ");
	cmdr.println("        /settings/ap     ");
	cmdr.println("        /settings/sta    ");
	cmdr.println("        /settings/log    ");
	cmdr.println("        /settings/cmd    ");
	cmdr.println("        /settings/soil   ");
	cmdr.println("        /settings/temp   ");
	return 0;
}

/// <summary>
///  Command Handler Function initiating a WiFi scan. and printing results.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool scanHandler(Commander& cmdr)
{
	Log.trace("scanHandler()" CR);
	unsigned short networks = manager.scan();

	cmdr.println("WiFi:");

	for (unsigned short i = 0; i < networks; i++)
	{
		WiFiManager::AccessPoint ap = manager.getAP(i);
		cmdr.print("    Network["); cmdr.print(i + 1, DEC); cmdr.println("]");
		cmdr.print("        SSID:       "); cmdr.println(ap.SSID);
		cmdr.print("        Signal:     "); cmdr.print(ap.RSSI, DEC); cmdr.println(" dBm");
		cmdr.print("        Channel:    "); cmdr.println(ap.Channel, DEC);
		cmdr.print("        Encryption: "); cmdr.println(ap.Encryption);
	}

	return 0;
}

/// <summary>
///  Command Handler Function reinitiating WiFi connections.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool wifiHandler(Commander& cmdr)
{
	Log.trace("wifiHandler()" CR);
	manager.disconnect();
	manager.connect();

	if (ApInfo::Active)
	{
		cmdr.println("AP running");
	}
	else if (WiFi.isConnected())
	{
		cmdr.println("Connected to AP");
	}
	else
	{
		cmdr.println("No WiFi");
	}

	return 0;
}

/// <summary>
///  Command Handler Function performing a ping.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool pingHandler(Commander& cmdr)
{
	Log.trace("pingHandler()" CR);
	String address("127.0.0.1");

	if (cmdr.hasPayload())
	{
		if (!cmdr.getString(address))
		{
			Log.error("    No address found");
			return 0;
		}
	}

	Log.trace("    Address: %s" CR, address);

	if (Ping.ping(address.c_str(), 3))
	{
		cmdr.println("    Ping successful");
	}
	else
	{
		cmdr.println("    Ping not successful");
	}

	return 0;
}

/// <summary>
///  Command Handler Function initializing all settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool initHandler(Commander& cmdr)
{
	Log.trace("initHandler()" CR);
	settings.init(sysInfo);
	return 0;
}

/// <summary>
///  Command Handler Function saving all settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool saveHandler(Commander& cmdr)
{
	Log.trace("saveHandler()" CR);
	settings.save();
	return 0;
}

/// <summary>
///  Command Handler Function showing all current sensor data.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool dataHandler(Commander& cmdr)
{
	Log.trace("dataHandler()" CR);
	cmdr.println(sensors.serialize());
	return 0;
}

/// <summary>
///  Command Handler Function showing current soil sensor data.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool soilHandler(Commander& cmdr)
{
	Log.trace("soilHandler()" CR);
	int index;

	if (cmdr.getInt(index))
	{
		if ((index >= 0) && (index < SoilSensors::MAX_SENSORS))
		{
			cmdr.print("Sensor["); cmdr.print(index + 1); cmdr.println("]: ");
			cmdr.print("    Pin:       "); cmdr.println(sensors.SoilSensors.getPinByIndex(index));
			cmdr.print("    Name:      "); cmdr.println(sensors.SoilSensors.getNameByIndex(index));
			cmdr.print("    Wet Value: "); cmdr.println(sensors.SoilSensors.getWetValueByIndex(index));
			cmdr.print("    Dry Value: "); cmdr.println(sensors.SoilSensors.getDryValueByIndex(index));
			cmdr.print("    Voltage:   "); cmdr.println(sensors.SoilSensors.getVoltageByIndex(index));
			cmdr.print("    Humidity:  "); cmdr.println(sensors.SoilSensors.getHumidityByIndex(index));
			cmdr.print("    Enabled:   "); cmdr.println(sensors.SoilSensors.isEnabledByIndex(index) ? "true" : "false");
		}
		else
		{
			cmdr.println("Invalid index");
		}
	}
	else
	{
		cmdr.println("Soil Moisture Sensors:" CR);

		for (unsigned short i = 0; i < SoilSensors::MAX_SENSORS; i++)
		{
			cmdr.print("Sensor["); cmdr.print(i + 1); cmdr.println("]: ");
			cmdr.print("    Pin:       "); cmdr.println(sensors.SoilSensors.getPinByIndex(i));
			cmdr.print("    Name:      "); cmdr.println(sensors.SoilSensors.getNameByIndex(i));
			cmdr.print("    Wet Value: "); cmdr.println(sensors.SoilSensors.getWetValueByIndex(i));
			cmdr.print("    Dry Value: "); cmdr.println(sensors.SoilSensors.getDryValueByIndex(i));
			cmdr.print("    Voltage:   "); cmdr.println(sensors.SoilSensors.getVoltageByIndex(i));
			cmdr.print("    Humidity:  "); cmdr.println(sensors.SoilSensors.getHumidityByIndex(i));
			cmdr.print("    Enabled:   "); cmdr.println(sensors.SoilSensors.isEnabledByIndex(i) ? "true" : "false");
		}
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current temperature sensor data.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool tempHandler(Commander& cmdr)
{
	Log.trace("tempHandler()" CR);
	int index;

	if (cmdr.getInt(index))
	{
		if ((index >= 0) && (index < TempSensors::MAX_SENSORS))
		{
			cmdr.print("Sensor["); cmdr.print(index + 1); cmdr.println("]: ");
			cmdr.print("    Name:       "); cmdr.println(sensors.TempSensors.getNameByIndex(index));
			cmdr.print("    Address:    "); cmdr.println(sensors.TempSensors.getAddressByIndex(index));
			cmdr.print("    Connected:  "); cmdr.println(sensors.TempSensors.isConnectedByIndex(index) ? "true" : "false");
			cmdr.print("    Resolution: "); cmdr.println(sensors.TempSensors.getResolutionByIndex(index));
			cmdr.print("    Temp C:     "); cmdr.println(sensors.TempSensors.getTempCByIndex(index));
			cmdr.print("    Temp F:     "); cmdr.println(sensors.TempSensors.getTempFByIndex(index));
		}
		else
		{
			cmdr.println("Invalid index");
		}
	}
	else
	{
		cmdr.println("Temperature Sensors:" CR);

		for (unsigned short i = 0; i < SoilSensors::MAX_SENSORS; i++)
		{
			cmdr.print("Sensor["); cmdr.print(i + 1); cmdr.println("]: ");
			cmdr.print("    Name:       "); cmdr.println(sensors.TempSensors.getNameByIndex(i));
			cmdr.print("    Address:    "); cmdr.println(sensors.TempSensors.getAddressByIndex(i));
			cmdr.print("    Connected:  "); cmdr.println(sensors.TempSensors.isConnectedByIndex(i) ? "true" : "false");
			cmdr.print("    Resolution: "); cmdr.println(sensors.TempSensors.getResolutionByIndex(i));
			cmdr.print("    Temp C:     "); cmdr.println(sensors.TempSensors.getTempCByIndex(i));
			cmdr.print("    Temp F:     "); cmdr.println(sensors.TempSensors.getTempFByIndex(i));
		}
	}

	return 0;
}

/// <summary>
///  Command Handler Function performing a WiFi settings reset.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool resetHandler(Commander& cmdr)
{
	Log.trace("resetHandler()" CR);
	settings.ApSettings.reset();
	settings.StaSettings.reset();
	settings.save();
	return 0;
}

/// <summary>
///  Command Handler Function to get/set the log level.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool levelHandler(Commander& cmdr)
{
	Log.trace("levelHandler()" CR);

	if (cmdr.hasPayload())
	{
		String level;
		cmdr.getString(level);

		Log.trace("    Level: %s" CR, level);

		if (settings.LogSettings.setLogLevelApp(level))
		{
			Log.begin(settings.LogSettings.getArduinoLogLevelApp(), &Serial);
		}
		else
		{
			cmdr.println("Invalid log level");
		}
	}
	else
	{
		cmdr.print("Log Level: ");
		cmdr.println(settings.LogSettings.getLogLevelApp());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing SPIFFS info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool spiffsHandler(Commander& cmdr)
{
	Log.trace("spiffsHandler()" CR);

	if (cmdr.hasPayload())
	{
		String name;

		if (cmdr.getString(name))
		{
			Log.trace("    File: %s" CR, name.c_str());

			if (!name.startsWith("/")) name = "/" + name;

			if (SPIFFS.exists(name.c_str()))
			{
				// Check if the file can be printed.
				if (name.endsWith(".json") || name.endsWith(".css") || name.endsWith(".js") || name.endsWith(".html"))
				{
					File file = SPIFFS.open(name.c_str());
					String text = file.readString();
					file.close();
					cmdr.println(text);
				}
				else
				{
					File file = SPIFFS.open(name.c_str());
					int size = file.size();
					file.close();
					cmdr.print("    File ");
					cmdr.print(name);
					cmdr.print(" Size: ");
					cmdr.print(size, DEC);
					cmdr.println(" bytes");
				}
			}
			else
			{
				cmdr.print("    File "); cmdr.print(name); cmdr.println(" does not exist");
			}
		}
		else
		{
			Log.error("    No file name found");
			return 0;
		}
	}
	else
	{
		cmdr.println("SPIFFS:");
		File root = SPIFFS.open("/");
		File file = root.openNextFile();

		while (file)
		{
			cmdr.print("    ");
			cmdr.print(file.name());
			cmdr.print(", ");
			cmdr.print(file.size());
			cmdr.println(" bytes");
			file = root.openNextFile();
		}

		cmdr.println();
	}

	return 0;
}

/// <summary>
///  Command Handler Function reformatting SPIFFS.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool formatHandler(Commander& cmdr)
{
	Log.trace("formatHandler()" CR);

	bool formatted = SPIFFS.format();

	cmdr.println("SPIFFS:");

	if (formatted)
	{
		cmdr.println("    Format successful");
	}
	else
	{
		cmdr.println("    Format not successful");
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing server info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool serverHandler(Commander& cmdr)
{
	Log.trace("serverHandler()" CR);
	ServerInfo info(WiFi);

	cmdr.println("Web Server Info:");
	cmdr.print("    WiFi Address: "); cmdr.println(info.WiFiAddress);
	cmdr.print("    AP Address:   "); cmdr.println(info.ApAddress);
	cmdr.print("    Name:         "); cmdr.println(info.Name);
	cmdr.print("    Port:         "); cmdr.println(info.Port);
	cmdr.print("    URL:          "); cmdr.println(info.Url);

	return 0;
}

/// <summary>
///  Command Handler Function showing system info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool systemHandler(Commander& cmdr)
{
	Log.trace("systemHandler()" CR);

	cmdr.println("System Info:");
	cmdr.print("    ChipRevision:    "); cmdr.println(sysInfo.ChipRevision);
	cmdr.print("    CpuFreqMHz:      "); cmdr.println(sysInfo.CpuFreqMHz);
	cmdr.print("    FlashChipSpeed:  "); cmdr.println(sysInfo.FlashChipSpeed);
	cmdr.print("    FlashChipSize:   "); cmdr.println(sysInfo.FlashChipSize);
	cmdr.print("    HeapSize:        "); cmdr.println(sysInfo.HeapSize);
	cmdr.print("    FreeHeap:        "); cmdr.println(sysInfo.FreeHeap);
	cmdr.print("    SketchSize:      "); cmdr.println(sysInfo.SketchSize);
	cmdr.print("    FreeSketchSpace: "); cmdr.println(sysInfo.FreeSketchSpace);
	cmdr.print("    SketchMD5:       "); cmdr.println(sysInfo.SketchMD5);
	cmdr.print("    SdkVersion:      "); cmdr.println(sysInfo.SdkVersion);
	cmdr.print("    ChipID:          "); cmdr.println(sysInfo.ChipID);
	cmdr.print("    Software:        "); cmdr.println(sysInfo.Software);

	return 0;
}

/// <summary>
///  Command Handler Function performing a reboot.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool rebootHandler(Commander& cmdr)
{
	Log.trace("rebootHandler()" CR);
	ESP.restart();
	return 0;
}

/// <summary>
///  Command Handler Function showing commander version info.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool versionHandler(Commander& cmdr)
{
	Log.trace("versionHandler()" CR);
	cmdr.printCommanderVersion();
	return 0;
}

/// <summary>
///  Command Handler Function showing current settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsHandler(Commander& cmdr)
{
	Log.trace("settingsHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current WiFi access point settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsApHandler(Commander& cmdr)
{
	Log.trace("settingsApHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.ApSettings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.ApSettings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current WiFi station settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsStaHandler(Commander& cmdr)
{
	Log.trace("settingsStaHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.StaSettings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.StaSettings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current Log settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsLogHandler(Commander& cmdr)
{
	Log.trace("settingsLogHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.LogSettings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.LogSettings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current Commander settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsCmdHandler(Commander& cmdr)
{
	Log.trace("settingsCmdHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.CmdSettings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.CmdSettings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current soil sensor settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsSoilHandler(Commander& cmdr)
{
	Log.trace("settingsSoilHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.SoilSettings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.SoilSettings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function showing current temperature sensor settings.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool settingsTempHandler(Commander& cmdr)
{
	Log.trace("settingsTempHandler()" CR);

	if (cmdr.hasPayload())
	{
		String json = cmdr.getPayloadString();

		if (json.length() > 0)
		{
			settings.TempSettings.deserialize(json);
		}
		else
		{
			Log.error("    No JSON found");
		}
	}
	else
	{
		cmdr.println(settings.TempSettings.serialize());
	}

	return 0;
}

/// <summary>
///  Command Handler Function to get/set the log level.
/// </summary>
/// <param name="cmdr">Reference to Commander instance</param>
/// <returns>Boolean</returns>
bool bluetoothHandler(Commander& cmdr)
{
	Log.trace("bluetoothHandler()" CR);

	if (cmdr.hasPayload())
	{
		if (cmdr.containsTrue() || cmdr.containsOn())
		{
			settings.CmdSettings.UseBluetooth = true;
		}
		else if (cmdr.containsFalse() || cmdr.containsOff())
		{
			settings.CmdSettings.UseBluetooth = false;
		}
		else
		{
			cmdr.println("Invalid boolean value (On|Off|True|False)");
		}

		if (settings.CmdSettings.UseBluetooth)
		{
			cmd.attachInputPort(&SerialBT);
			cmd.attachOutputPort(&SerialBT);
			cmd.attachAltPort(&Serial);
			cmd.echoToAlt(true);
		}
		else
		{
			cmd.attachInputPort(&Serial);
			cmd.attachOutputPort(&Serial);
			cmd.echoToAlt(false);
			cmd.deleteAltPort();
		}
	}
	else
	{
		cmdr.print("Use Bluetooth: ");
		cmdr.println(settings.CmdSettings.UseBluetooth ? "true" : "false");
	}

	return 0;
}

/// <summary>
///  Command Array
/// </summary>
const commandList_t commands[] = {
	{"ap",		      apHandler,		   "show AP info"},
	{"sta",		      staHandler,		   "show STA info"},
	{"api",		      apiHandler,		   "show REST API"},
	{"scan",	      scanHandler,		   "scan WiFi network"},
	{"wifi",	      wifiHandler,		   "restart WiFi"},
	{"ping",	      pingHandler,		   "pinging IP address"},
	{"init",	      initHandler,		   "intialize settings"},
	{"save",	      saveHandler,		   "save settings"},
	{"data",	      dataHandler,		   "show sensor data"},
	{"soil",	      soilHandler,		   "show soil sensor data"},
	{"temp",	      tempHandler,		   "show temp sensor data"},
	{"reset",	      resetHandler,		   "reset WiFi settings"},
	{"level",	      levelHandler,		   "get/set log level"},
	{"spiffs",	      spiffsHandler,       "show SPIFFS info"},
	{"server",	      serverHandler,	   "show server info"},
	{"system",	      systemHandler,	   "show system info"},
	{"reboot",	      rebootHandler,	   "reboot system"},
	{"version",	      versionHandler,      "show commander version"},
	{"bluetooth",	  bluetoothHandler,    "get/set bluetooth"},
	// Hidden commands
	{"format",	      formatHandler,	   "-reformat SPIFFS"},
	{"settings",      settingsHandler,	   "-get/set all settings"},
	{"settings-ap",	  settingsApHandler,   "-get/set Ap settings"},
	{"settings-sta",  settingsStaHandler,  "-get/set Sta settings"},
	{"settings-log",  settingsLogHandler,  "-get/set Log settings"},
	{"settings-cmd",  settingsCmdHandler,  "-get/set Cmd settings"},
	{"settings-soil", settingsSoilHandler, "-get/set Soil settings"},
	{"settings-temp", settingsTempHandler, "-get/set Temp settings"},
};

/// <summary>
///  Initialize command processing using settings and print a prompt.
/// </summary>
void initCommander()
{
	Log.trace("initCommander()" CR);

	if (settings.CmdSettings.UseBluetooth)
	{
		cmd.begin(&SerialBT, commands, sizeof(commands));
		cmd.attachAltPort(&Serial);
		cmd.echoToAlt(true);
	}
	else
	{
		cmd.begin(&Serial, commands, sizeof(commands));
		cmd.echoToAlt(false);
	}

	cmd.echo(true);
	cmd.echoToAlt(false);
	cmd.copyRepyAlt(false);
	cmd.setDelimiters(" \t");
	cmd.commanderName = settings.CmdSettings.Prompt;
	cmd.setPassPhrase(settings.CmdSettings.PassPhrase);
	cmd.setLockType(settings.CmdSettings.HardLock);
	settings.CmdSettings.Locked ? cmd.lock() : cmd.unlock();
	cmd.commandPrompt(settings.CmdSettings.CommandPrompt);
	cmd.errorMessages(settings.CmdSettings.ErrorMessages);
	cmd.printCommandPrompt();
}
