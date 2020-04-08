// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Settings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <vfs_api.h>
#include <FSImpl.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoLog.h>
#include "Settings.h"

char* Settings::SETTINGS_FILE = "/settings.json";

/// <summary>
///  Constructor using sensors for initialization.
/// </summary>
/// <param name="sensors">Pointer to sensors</param>
Settings::Settings(Sensors* sensors) :
	SoilSettings(&sensors->SoilSensors),
	TempSettings(&sensors->TempSensors)
{
	Log.trace("Settings::Settings()" CR);
}

/// <summary>
/// Initializes all data from the non volatile storage.
/// </summary>
void Settings::init(SystemInfo& info)
{
	Log.trace("Settings::init()" CR);
	File file = SPIFFS.open(SETTINGS_FILE, FILE_READ);

	if (!file)
	{
		Log.error("Failed to open settings" CR);
		return;
	}

	String json = file.readString();
	file.close();

	if (!deserialize(json))
	{
		Log.error("Failed to deserialize settings" CR);
	}

	// Update default AP SSID with unique chip ID (MAC address).
	if (ApSettings.SSID == ApSettings.WIFI_SSID_AP)
	{
		ApSettings.SSID = ApSettings.WIFI_SSID_AP + String("_") + info.ChipID;
		save();
	}

	// Update default Bluetooth name with unique chip ID (MAC address).
	if (CmdSettings.LocalName == CmdSettings.BT_LOCAL_NAME)
	{
		CmdSettings.LocalName = CmdSettings.BT_LOCAL_NAME + String("_") + info.ChipID;
		save();
	}
}

/// <summary>
/// Saves all data to the non volatile storage.
/// </summary>
void Settings::save()
{
	Log.trace("Settings::save()" CR);
	File file = SPIFFS.open(SETTINGS_FILE, FILE_WRITE);

	if (!file)
	{
		Log.error("Failed to open settings" CR);
		return;
	}

	String json = serialize();

	if (json.length() > 0)
	{
		if (file.print(json) != json.length())
		{
			Log.error("Failed to write settings" CR);
		}
	}
	else
	{
		Log.error("Failed to serialize settings" CR);
	}

	file.close();
}

/// <summary>
///  Deserialize the data fields from a JSON string.
/// </summary>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool Settings::deserialize(String json)
{
	Log.trace("Settings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("Settings::deserialize() Deserialize JSON failed with code %s" CR, err.c_str());
			return false;
		}

		String ap;
		serializeJson(_doc["AP"], ap);
		ApSettings.deserialize(ap);

		String sta;
		serializeJson(_doc["STA"], sta);
		StaSettings.deserialize(sta);

		String log;
		serializeJson(_doc["Log"], log);
		LogSettings.deserialize(log);

		String cmd;
		serializeJson(_doc["Cmd"], cmd);
		CmdSettings.deserialize(cmd);

		String temp;
		serializeJson(_doc["Temp"], temp);
		TempSettings.deserialize(temp);

		String soil;
		serializeJson(_doc["Soil"], soil);
		SoilSettings.deserialize(soil);

		return true;
	}

	Log.warning("Settings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String Settings::serialize()
{
	Log.trace("Settings::serialize()" CR);
	String json;

	_doc.clear();
	_doc["AP"]   = serialized(ApSettings.serialize());
	_doc["STA"]  = serialized(StaSettings.serialize());
	_doc["Log"]  = serialized(LogSettings.serialize());
	_doc["Cmd"]  = serialized(CmdSettings.serialize());
	_doc["Temp"] = serialized(TempSettings.serialize());
	_doc["Soil"] = serialized(SoilSettings.serialize());

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Resets all field to their default values.
/// </summary>
void Settings::reset()
{
	Log.trace("Settings::reset()" CR);

	ApSettings.reset();
	StaSettings.reset();
	LogSettings.reset();
	CmdSettings.reset();
}