// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ServerInfo.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

/// <summary>
/// This class holds the actual HTTP server settings data.
/// </summary>
class ServerInfo
{
private:
	static const int CAPACITY = 			// The maximum size for the JSON document
		JSON_OBJECT_SIZE(5) + 141;
	StaticJsonDocument<CAPACITY> _doc;		// The static JSON document

public:
	static char* HOSTNAME;					// The default hostname (mDNS)
	static int PORT;						// The default web server port (80)

	ServerInfo(WiFiClass wifi);				// Constructor using WiFi instance to initialize fields

	String WiFiAddress;						// The web server IP address on the WiFi
	String ApAddress;						// The web server IP address on the accesspoint
	String Name;							// The web server hostname
	int Port;								// The web server IP port
	String Url;								// The web server URL (mDNS)

	String serialize();						// Return a string serialization (JSON)
};
