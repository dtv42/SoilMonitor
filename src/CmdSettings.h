// --------------------------------------------------------------------------------------------------------------------
// <copyright file="CmdSettings.h" company="DTV-Online">
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
/// This class holds the commander configuration data.
/// This also includes settings for the Bluetooth Serial communication.
/// </summary>
class CmdSettings
{
private:
	const char* CMDR_PROMPT = "cmd";			// The default prompt

	static const int CAPACITY =					// The maximum size for the JSON document
		JSON_OBJECT_SIZE(8) + 216;				
	StaticJsonDocument<CAPACITY> _doc;			// The static JSON document

public:
	const char* BT_LOCAL_NAME = "ESP32";		// The default bluetooth name

	CmdSettings();								// Default constructor

	String Prompt;								// The Commander prompt
	String PassPhrase;							// The Commander passphrase
	String LocalName;							// The Bluetooth local name
	bool UseBluetooth;							// Flag indicating that bluetooth serial is used
	bool HardLock;								// Flag indicating that a hard lock is used
	bool Locked;								// Flag indicating locking set at startup
	bool ErrorMessages;							// Flag indicating that error messages are enabled
	bool CommandPrompt;							// Flag indicating that the command prompt is enabled

	bool deserialize(String json);				// Read a JSON string and updates the fields
	String serialize();							// Return a string serialization (JSON)
	void reset();								// Resets all settings
};

