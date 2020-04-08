// --------------------------------------------------------------------------------------------------------------------
// <copyright file="StaSettings.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

/// <summary>
/// This class holds the WiFi STA station connection configuration data.
/// </summary>
class StaSettings
{
private:
	const char* WIFI_HOSTNAME = "esp32";			// The default access point hostname
	const char* SUBNET_MASK = "255.255.255.0";		// The default network mask

	static const int CAPACITY = 					// The maximum size for the JSON document
		JSON_OBJECT_SIZE(9) + 268;
	StaticJsonDocument<CAPACITY> _doc;				// The static JSON document

public:
	StaSettings();									// Default constructor

	String SSID;									// The WiFi SSID
	String PASS;									// The WiFi Passphrase
	String Hostname;								// The WiFi Hostname
	bool DHCP;										// The WiFi DHCP mode (default: true)
	String Address;									// The static Address
	String Gateway;									// The Gateway address
	String Subnet;									// The SubnetMask
	String DNS1;									// The primary domain name server
	String DNS2;									// The secondary domain name server

	bool deserialize(String json);					// Read a JSON string and updates the fields.
	String serialize();								// Return a string serialization (JSON)
	void reset();									// Resets all settings
};
