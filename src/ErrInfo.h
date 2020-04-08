// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ErrInfo.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

/// /// <summary>
/// This class holds Error data.
/// </summary>
class ErrInfo
{
private:
	static const int CAPACITY =				// The maximum size for the JSON document
		JSON_OBJECT_SIZE(2) + 270;	
	StaticJsonDocument<CAPACITY> _doc;		// The static JSON document

public:
	ErrInfo();								// Default constructor

	int Code;								// The Error code
	String Message;							// The Error message

	bool deserialize(String json);			// Read a JSON string and updates the fields
	String serialize();						// Return a string serialization (JSON)
};

