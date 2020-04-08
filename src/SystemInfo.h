// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SystemInfo.h" company="DTV-Online">
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
/// This class holds the current system data.
/// Note that if this class is instanciated before the sketch information is available 
/// the getSketchXXX() functions crashes (abort()). So this class has to be instantiated later.
/// </summary>
class SystemInfo
{
private:
	static const int CAPACITY = 			// The maximum size for the JSON document
		JSON_OBJECT_SIZE(12) + 247;
	StaticJsonDocument<CAPACITY> _doc;		// The static JSON document

public:
	static char* SOFTWARE_VERSION;			// The software versionstring with date (see .ino)

	SystemInfo();							// Default constructor

	int ChipRevision;						// The chip revision
	int CpuFreqMHz;							// The CPU frequency in MHz
	int FlashChipSpeed;						// The flash chip speed in MHz
	int FlashChipSize;						// The flash chip size in kB
	int HeapSize;							// The total heap size in kB
	int FreeHeap;							// The amount of free heap kB
	int SketchSize;							// The sketch size in kB
	int FreeSketchSpace;					// The free sketch space in kB
	String SketchMD5;						// The MD5 of the current sketch
	String SdkVersion;						// The espressif SDK version
	String ChipID;							// Board identifier (MAC address)
	String Software;						// Software version and date

	String serialize();						// Return a string serialization (JSON)
	void init();							// Initializes selected values
	void update();							// Updates dynamic values
};
