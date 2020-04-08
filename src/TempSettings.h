// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TempSettings.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "TempSensors.h"

/// <summary>
/// This class holds the various sensor settings.
/// </summary>
class TempSettings
{
private:
	static const unsigned short MAX_SENSORS = 6;				// Number of temperature sensors

	static const int CAPACITY =									// The maximum size for the JSON document
		JSON_ARRAY_SIZE(6) +
	6 * JSON_OBJECT_SIZE(1) +
		JSON_OBJECT_SIZE(2) + 240;
	StaticJsonDocument<CAPACITY> _doc;							// The static JSON document

	TempSensors* _sensors;										// Pointer to temperature sensors

public:
	TempSettings(TempSensors* sensors);							// Constructor using sensors to initialize settings

	unsigned short Pin;											// The OneWire input pin
	String Names[MAX_SENSORS];									// The sensor names

	bool deserializeByIndex(unsigned short index, String json);	// Read a JSON string and updates the sensor fields
	bool deserialize(String json);								// Read a JSON string and updates the fields
	String serializeByIndex(unsigned short index);				// Return a string serialization (JSON)
	String serialize();											// Return a string serialization (JSON)
};
