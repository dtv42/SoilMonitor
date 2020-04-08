// --------------------------------------------------------------------------------------------------------------------
// <copyright file="WiFiManager.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "Settings.h"

/// <summary>
/// This class creates an WiFi access point or connects to an existing WiFi access point.
/// </summary>
class WiFiManager
{
public:
	struct AccessPoint
	{
		String SSID;
		int RSSI;
		int Channel;
		String Encryption;
	};

private:
	Settings* _settings;
	unsigned short _numberAP;

	bool wait4Mode(wifi_mode_t mode, int count = 10);	// Wait for WiFi mode change
	bool wait4WiFi(int count = 20);						// Wait for WiFi connection
public:
	WiFiManager(Settings* settings);					// Constructor using a pointer to a settings instance

	unsigned short scan();								// Scan the WiFi network for access points
	AccessPoint getAP(unsigned short index);			// Returns the AP data at the specified index (after scan)
	bool connect();										// Attempt to connect to WiFi network or create access point
	bool createAP();									// Create a WiFi access point using id
	bool connectAP();									// Connect to a WiFi access point
	void disconnect();									// Disconnect WiFi
};