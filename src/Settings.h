// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Settings.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "SystemInfo.h"
#include "ApSettings.h"
#include "StaSettings.h"
#include "LogSettings.h"
#include "CmdSettings.h"
#include "SoilSettings.h"
#include "TempSettings.h"
#include "Sensors.h"

/// <summary>
/// This class holds the all settings data.
/// </summary>
class Settings
{
public:
	static char* SETTINGS_FILE;					// The filename of the stored settings

private:
	static const int CAPACITY =					// The maximum size for the JSON document
	2 * JSON_ARRAY_SIZE(6) +
	6 * JSON_OBJECT_SIZE(1) +
	2 * JSON_OBJECT_SIZE(2) +
	6 * JSON_OBJECT_SIZE(4) +
	2 * JSON_OBJECT_SIZE(5) +
	2 * JSON_OBJECT_SIZE(8) +
		JSON_OBJECT_SIZE(9) + 1395;
	StaticJsonDocument<CAPACITY> _doc;			// The static JSON document

public:
	Settings(Sensors* sensors);					// Constructor using sensors to initialize settings

	class ApSettings  ApSettings;				// The Access Point connection settings 
	class StaSettings StaSettings;				// The WiFi station connection settings
	class LogSettings LogSettings;				// The Log settings
	class CmdSettings CmdSettings;				// The Commander settings
	class TempSettings TempSettings;			// The SoilMonitor temperature sensor settings
	class SoilSettings SoilSettings;			// The SoilMonitor moisture sensor settings

	bool deserialize(String json);				// Read a JSON string and updates the fields.
	String serialize();							// Return a string serialization (JSON)
	void save();								// Save the settings to storage
	void init(SystemInfo& info);				// Initializes the settings from storage
	void reset();								// Resets all settings
};
