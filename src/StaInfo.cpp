// --------------------------------------------------------------------------------------------------------------------
// <copyright file="StaInfo.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "esp_wifi.h"
#include "StaInfo.h"

/// <summary>
///  Using a WiFi instance to get the actual data.
/// </summary>
/// <param name="wifi">The WiFiClass instance</param>
StaInfo::StaInfo(WiFiClass wifi) :
	Active(wifi.isConnected()),
	Hostname(wifi.getHostname()),
	NetworkID(wifi.networkID().toString()),
	Address(wifi.localIP().toString()),
	Gateway(wifi.gatewayIP().toString()),
	Subnet(wifi.subnetMask().toString()),
	DNS(wifi.dnsIP().toString()),
	RSSI(wifi.RSSI()),
	BSSID(wifi.BSSIDstr()),
	MAC(wifi.macAddress())
{
	Log.trace("StaInfo::StaInfo()" CR);

	wifi_config_t config;
	esp_wifi_get_config(WIFI_IF_STA, &config);

	SSID = String(reinterpret_cast<const char*>(config.sta.ssid));
	PASS = String(reinterpret_cast<const char*>(config.sta.password));
}

/// <summary>
///  Serialize the WiFiInfoClass instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String StaInfo::serialize()
{
	Log.trace("StaInfo::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Active"]    = Active;
	_doc["SSID"]      = SSID;
	_doc["PASS"]      = PASS;
	_doc["Hostname"]  = Hostname;
	_doc["NetworkID"] = NetworkID;
	_doc["Address"]   = Address;
	_doc["Gateway"]   = Gateway;
	_doc["Subnet"]    = Subnet;
	_doc["DNS"]       = DNS;
	_doc["RSSI"]      = RSSI;
	_doc["BSSID"]     = BSSID;
	_doc["MAC"]       = MAC;

	serializeJsonPretty(_doc, json);

	return json;
}
