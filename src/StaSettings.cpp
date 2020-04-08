// --------------------------------------------------------------------------------------------------------------------
// <copyright file="StaSettings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "StaSettings.h"

/// <summary>
/// Initializes selected data fields to default values.
/// </summary>
/// <param name="commander">Pointer to commander instance</param>
StaSettings::StaSettings() :
	SSID(""),
	PASS(""),
	Hostname(WIFI_HOSTNAME),
	DHCP(true),
	Address(""),
	Gateway(""),
	Subnet(SUBNET_MASK),
	DNS1(""),
	DNS2("")
{
	Log.trace("StaSettings::StaSettings()" CR);
}

/// <summary>
///  Deserialize the data fields from a JSON string.
/// </summary>
/// <param name="text">The JSON string</param>
/// <returns>True if successful</returns>
bool StaSettings::deserialize(String json)
{
	Log.trace("StaSettings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("StaSettings::deserialize() Deserialize JSON failed with code %s", err.c_str());
			return false;
		}

		SSID =     _doc["SSID"]     | SSID;
		PASS =     _doc["PASS"]     | PASS;
		Hostname = _doc["Hostname"] | Hostname;
		DHCP =     _doc["DHCP"]     | DHCP;
		Address =  _doc["Address"]  | Address;
		Gateway =  _doc["Gateway"]  | Gateway;
		Subnet =   _doc["Subnet"]   | Subnet;
		DNS1 =     _doc["DNS1"]     | DNS1;
		DNS2 =     _doc["DNS2"]     | DNS2;

		return true;
	}

	Log.warning("StaSettings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String StaSettings::serialize()
{
	Log.trace("StaSettings::serialize()" CR);
	String json;

	_doc.clear();
	_doc["SSID"]     = SSID;
	_doc["PASS"]     = PASS;
	_doc["Hostname"] = Hostname;
	_doc["DHCP"]     = DHCP;
	_doc["Address"]  = Address;
	_doc["Gateway"]  = Gateway;
	_doc["Subnet"]   = Subnet;
	_doc["DNS1"]     = DNS1;
	_doc["DNS2"]     = DNS2;

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Resets all field to their default values.
/// </summary>
void StaSettings::reset()
{
	Log.trace("StaSettings::reset()" CR);

	SSID = "";
	PASS = "";
	Hostname = WIFI_HOSTNAME;
	DHCP = true;
	Address = "";
	Gateway = "";
	Subnet = SUBNET_MASK;
	DNS1 = "";
	DNS2 = "";
}