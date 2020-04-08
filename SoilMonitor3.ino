// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SoilMonitor3.ino" company="DTV-Online">
//  Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//  Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// <created>02-Apr-20 18:15:49</created>
// <author>peter</author>
// <summary>
//  Global values, Setup() and Loop() functions. Note that this file is merged with all other '.ino' files.
// </summary>
// --------------------------------------------------------------------------------------------------------------------
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <aWOT.h>
#include <Commander.h>
#include <ping.h>
#include <ESP32Ping.h>
#include <ArduinoLog.h>
#include <jled.h>
#include <neotimer.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <esp32-hal-log.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include "src/Sensors.h"
#include "src/Settings.h"
#include "src/ApInfo.h"
#include "src/StaInfo.h"
#include "src/ErrInfo.h"
#include "src/ServerInfo.h"
#include "src/SystemInfo.h"
#include "src/WiFiManager.h"
#include "src/MimeTypes.h"

// Set the software version for the SystemInfoClass.
char* SystemInfo::SOFTWARE_VERSION = "V1.0.2 2020-04-04";

// Message lines to be printed on the serial line during startup.
auto HEADER = "SoilMonitor2";
auto COPYRIGHT = "Copyright (c) 2020 - Dr. Peter Trimmel";

// On board LED.
JLed led = JLed(LED_BUILTIN).Blink(500, 500).Forever();

// Setup update timer (1sec) and reboot timer (5 sec).
Neotimer updateTimer = Neotimer(1000);
Neotimer rebootTimer = Neotimer(5000);

// Bluetooth support (Serial).
BluetoothSerial SerialBT;

// The command processing (Serial)
Commander cmd;

// Setup soil sensors using defaults.
Sensors sensors;

// The global application settings (WiFi, sensors).
Settings settings(&sensors);

// The WiFi manager (using the settings).
WiFiManager manager(&settings);

// Create Webserver at the default port.
WiFiServer server(ServerInfo::PORT);
Application app;

// System infos.
SystemInfo sysInfo;

// Error infos.
ErrInfo error;

/// <summary>
/// The setup() function is called when a sketch starts. It initializes variables, pin modes,
/// starts using libraries, etc. The setup() function will only run once, after each powerup
/// or reset of the Arduino board.
/// </summary>
void setup()
{
	// Initialize non volatile storage and settings.
	esp_err_t err = nvs_flash_init();

	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}

	// Initialize serial and wait 1 second for port to open.
	Serial.begin(115200);
	delay(1000);

	// Print application startup info.
	Serial.println(HEADER);
	Serial.println(COPYRIGHT);
	Serial.println();

	// Mount the SPIFFS.
	if (!SPIFFS.begin())
	{
		Log.fatal("An Error has occurred while mounting SPIFFS" CR);
		return;
	}

	// Initialize system info and settings.
	sysInfo.init();
	settings.init(sysInfo);

	// Initializing Bluetooth serial and wait 1 second.
	SerialBT.begin(settings.CmdSettings.LocalName);
	delay(1000);

	// Initialize logging (Serial).
	initLogging();

	// Try to connect WiFi, and start the sensors.
	manager.connect();
	sensors.SoilSensors.begin();
	sensors.TempSensors.begin();

	// Initialize the commander and the web server.
	initCommander();
	initServer();

	// Start timer.
	updateTimer.start();
}

/// <summary>
/// This function is called consecutively, until powered down or reset (reboot).
/// </summary>
void loop()
{
	WiFiClient client = server.available();

	led.Update();
	cmd.update();

	if (client.connected())
	{
		app.process(&client);
	}

	if (updateTimer.repeat())
	{
		sensors.SoilSensors.update();
		sensors.TempSensors.update();
		sysInfo.update();
	}

	if (rebootTimer.done())
	{
		ESP.restart();
	}
}
