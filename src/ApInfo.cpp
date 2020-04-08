// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ApInfo.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <esp_wifi.h>
#include <ArduinoLog.h>
#include "ApInfo.h"

bool ApInfo::Active = false;

/// <summary>
///  Using a WiFi instance to get the actual data.
/// </summary>
/// <param name="wifi">The WiFiClass instance</param>
ApInfo::ApInfo(WiFiClass wifi)
{
	Log.trace("ApInfo::ApInfo()" CR);
	wifi_config_t config;
	esp_wifi_get_config(WIFI_IF_AP, &config);
	
	SSID      = String(reinterpret_cast<const char*>(config.ap.ssid));
	PASS      = String(reinterpret_cast<const char*>(config.ap.password));
	Hostname  = String(wifi.softAPgetHostname());
	NetworkID = wifi.softAPNetworkID().toString();
	Address   = wifi.softAPIP().toString();
	Clients   = wifi.softAPgetStationNum();
	MAC       = wifi.softAPmacAddress();
}

/// <summary>
///  Serialize the ApInfoClass instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String ApInfo::serialize()
{
	Log.trace("ApInfo::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Active"]    = ApInfo::Active;
	_doc["SSID"]      = SSID;
	_doc["PASS"]      = PASS;
	_doc["Hostname"]  = Hostname;
	_doc["NetworkID"] = NetworkID;
	_doc["Address"]   = Address;
	_doc["Clients"]   = Clients;
	_doc["MAC"]       = MAC;

	serializeJsonPretty(_doc, json);
	return json;
}
