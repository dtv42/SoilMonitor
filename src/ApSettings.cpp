// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ApSettings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "ApSettings.h"

/// <summary>
/// Initializes selected data fields to default values.
/// </summary>
/// <param name="commander">Pointer to commander instance</param>
ApSettings::ApSettings() :
	Backup(true),
	SSID(WIFI_SSID_AP),
	PASS(""),
	Custom(false),
	Hostname(WIFI_HOSTNAME),
	Address(""),
	Gateway(""),
	Subnet(SUBNET_MASK)
{
	Log.trace("ApSettings::ApSettings()" CR);
}

/// <summary>
///  Deserialize the data fields from a JSON string.
/// </summary>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool ApSettings::deserialize(String json)
{
	Log.trace("ApSettings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("ApSettings::deserialize() Deserialize JSON failed with code %s" CR, err.c_str());
			return false;
		}

		Backup =   _doc["Backup"]   | Backup;
		SSID =     _doc["SSID"]     | SSID;
		PASS =     _doc["PASS"]     | PASS;
		Hostname = _doc["Hostname"] | Hostname;
		Custom =   _doc["Custom"]   | Custom;
		Address =  _doc["Address"]  | Address;
		Gateway =  _doc["Gateway"]  | Gateway;
		Subnet =   _doc["Subnet"]   | Subnet;

		return true;
	}

	Log.warning("ApSettings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String ApSettings::serialize()
{
	Log.trace("ApSettings::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Backup"]   = Backup;
	_doc["SSID"]     = SSID;
	_doc["PASS"]     = PASS;
	_doc["Hostname"] = Hostname;
	_doc["Custom"]   = Custom;
	_doc["Address"]  = Address;
	_doc["Gateway"]  = Gateway;
	_doc["Subnet"]   = Subnet;

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Resets all field to their default values.
/// </summary>
void ApSettings::reset()
{
	Log.trace("ApSettings::reset()" CR);
	Backup = true;
	SSID = WIFI_SSID_AP;
	PASS = "";
	Custom = false;
	Hostname = WIFI_HOSTNAME;
	Address = "";
	Gateway = "";
	Subnet = SUBNET_MASK;
}