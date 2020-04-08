// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SoilSettings.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "SoilSensors.h"

/// <summary>
/// This class holds the various moisture sensor settings.
/// </summary>
class SoilSettings
{
private:
	static const unsigned short MAX_SENSORS = 6;				// Number of supported soil moisture sensors
	
	static const int CAPACITY =									// The maximum size for the JSON document
		JSON_ARRAY_SIZE(6) +
	6 * JSON_OBJECT_SIZE(4) + 324;
	StaticJsonDocument<CAPACITY> _doc;							// The static JSON document

	SoilSensors* _sensors;										// Pointer to soil moisture sensors

public:
	SoilSettings(SoilSensors* sensors);							// Constructor using sensors to initialize settings

	String Names[MAX_SENSORS];									// The sensor names
	unsigned short Pins[MAX_SENSORS];							// The sensor input pins
	float WetValues[MAX_SENSORS];								// The sensor wet values
	float DryValues[MAX_SENSORS];								// The sensor dry values
	bool Enabled[MAX_SENSORS];									// The sensor enabled flags

	bool deserializeByIndex(unsigned short index, String json);	// Read a JSON string and updates the sensor fields
	bool deserialize(String json);								// Read a JSON string and updates the fields
	String serializeByIndex(unsigned short index);				// Return a string serialization (JSON)
	String serialize();											// Return a string serialization (JSON)
};
