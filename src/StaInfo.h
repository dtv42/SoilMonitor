// --------------------------------------------------------------------------------------------------------------------
// <copyright file="WiFiInfo.h" company="DTV-Online">
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
/// This class holds the actual WiFi station connection data.
/// </summary>
class StaInfo
{
private:
	static const int CAPACITY =								// The maximum size for the JSON document
		JSON_OBJECT_SIZE(12) + 325;	
	StaticJsonDocument<CAPACITY> _doc;						// The static JSON document

public:
	StaInfo(WiFiClass wifi);								// Constructor using WiFi instance to initialize fields

	bool Active;											// True if WiFi connection is active
	String SSID;											// The WiFi SSID
	String PASS;											// The WiFi passphrase
	String Hostname;										// The WiFi hostname
	String NetworkID;										// The WiFi network ID
	String Address;											// The WiFi IP address
	String Gateway;											// The Gateway address
	String Subnet;											// The Subnet mask
	String DNS;												// The domain name server
	int RSSI;												// The signal strength
	String BSSID;											// The MAC address of the router
	String MAC;												// The MAC address

	String serialize();										// Return a string serialization (JSON)
};