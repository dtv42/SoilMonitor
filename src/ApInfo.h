// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ApInfo.h" company="DTV-Online">
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

/// /// <summary>
/// This class holds the actual WiFi access point data.
/// </summary>
class ApInfo
{
private:
	static const int CAPACITY =					// The maximum size for the JSON document
		JSON_OBJECT_SIZE(8) + 237;	
	StaticJsonDocument<CAPACITY> _doc;			// The static JSON document

public:
	ApInfo(WiFiClass wifi);						// Constructor using WiFi instance to initialize fields

	static bool Active;							// True if Access Point is active
	String SSID;								// The WiFi Access Point SSID
	String PASS;								// The WiFi Access Point passphrase
	String Hostname;							// The WiFi Access Point hostname
	String NetworkID;							// The WiFi Access Point network ID
	String Address;								// The WiFi Access Point IP address
	int Clients;								// The number of clients (max. 4)
	String MAC;									// The WiFi Access Point MAC address

	String serialize();							// Return a string serialization (JSON)
};

