// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ApSettings.h" company="DTV-Online">
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
/// This class holds the WiFi access point configuration data.
/// </summary>
class ApSettings
{
private:
	const char* SUBNET_MASK = "255.255.255.0";		// The default network mask

	static const int CAPACITY =						// The maximum size for the JSON document
		JSON_OBJECT_SIZE(8) + 235;	
	StaticJsonDocument<CAPACITY> _doc;				// The static JSON document

public:	
	const char* WIFI_SSID_AP = "ESP32";				// The default access point SSID
	const char* WIFI_HOSTNAME = "esp32";			// The default access point hostname

	ApSettings();									// Default constructor

	bool Backup;									// Flag indicating to create AP as a backup
	String SSID;									// The WiFi Access Point SSID
	String PASS;									// The WiFi Access Point Passphrase
	bool Custom;									// The WiFi Custom mode (default: false)
	String Hostname;								// The WiFi Access Point Hostname
	String Address;									// The WiFi Access Point Address
	String Gateway;									// The Gateway address
	String Subnet;									// The SubnetMask

	bool deserialize(String json);					// Read a JSON string and updates the fields.
	String serialize();								// Return a string serialization (JSON)
	void reset();									// Resets all settings
};

