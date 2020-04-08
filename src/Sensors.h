// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Sensors.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include "SoilSensors.h"
#include "TempSensors.h"

class Sensors
{
private:
	static const int CAPACITY = 			// The maximum size for the JSON document
		2 * JSON_ARRAY_SIZE(6) +
		JSON_OBJECT_SIZE(2) +
		6 * JSON_OBJECT_SIZE(4) +
		JSON_OBJECT_SIZE(5) +
		5 * JSON_OBJECT_SIZE(6) + 967;
StaticJsonDocument<CAPACITY> _doc;			// The static JSON document

public:
	Sensors();								// Default constructor

	class TempSensors TempSensors;			// Temperature sensors	
	class SoilSensors SoilSensors;			// Soil moisture sensors

	String serialize();						// Return a string serialization (JSON)
};