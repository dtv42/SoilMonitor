// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ServerInfo.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "ServerInfo.h"

char* ServerInfo::HOSTNAME = "soilmonitor";		// The default hostname (mDNS)
int ServerInfo::PORT = 80;						// The default web server port (80)

/// <summary>
///  Using a WiFi instance to get the actual data.
/// </summary>
/// <param name="commander">Pointer to commander instance</param>
ServerInfo::ServerInfo(WiFiClass wifi) :
	Port(PORT),
	Name(HOSTNAME),
	WiFiAddress(wifi.localIP().toString()),
	ApAddress(wifi.softAPIP().toString()),
	Url("http://" + String(HOSTNAME))
{
	Log.trace("ServerInfo::ServerInfo()" CR);
}

/// <summary>
///  Serialize the ServerInfoClass instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String ServerInfo::serialize()
{
	Log.trace("ServerInfo::serialize()" CR);
	String json;

	_doc.clear();
	_doc["WiFiAddress"] = WiFiAddress;
	_doc["ApAddress"]   = ApAddress;
	_doc["Name"]        = Name;
	_doc["Port"]        = Port;
	_doc["Url"]         = Url;

	serializeJsonPretty(_doc, json);

	return json;
}
