// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SystemInfo.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "SystemInfo.h"

/// <summary>
///  Using the global ESP instance to get the actual data.
/// </summary>
/// <param name="commander">Pointer to commander instance</param>
SystemInfo::SystemInfo() :
	Software(SOFTWARE_VERSION)
{
	Log.trace("SystemInfo::SystemInfo()" CR);

	ChipRevision = ESP.getChipRevision();
	CpuFreqMHz = ESP.getCpuFreqMHz();
	FlashChipSpeed = ESP.getFlashChipSpeed() / 1000000;
	FlashChipSize = ESP.getFlashChipSize() / 1000;
	SdkVersion = String(ESP.getSdkVersion());

	uint64_t mac = ESP.getEfuseMac();
	uint8_t bytes[6];
	char chipid[13];

	bytes[0] = (mac >> 40) & 0xFF;
	bytes[1] = (mac >> 32) & 0xFF;
	bytes[2] = (mac >> 24) & 0xFF;
	bytes[3] = (mac >> 16) & 0xFF;
	bytes[4] = (mac >> 8) & 0xFF;
	bytes[5] = (mac >> 0) & 0xFF;

	sprintf(chipid, "%02X%02X%02X%02X%02X%02X", bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0]);

	ChipID = String(chipid);
}

/// <summary>
///  Serialize the SystemInfoClass instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String SystemInfo::serialize()
{
	Log.trace("SystemInfo::serialize()" CR);
	String json;

	_doc.clear();
	_doc["ChipRevision"]    = ChipRevision;
	_doc["CpuFreqMHz"]      = CpuFreqMHz;
	_doc["FlashChipSpeed"]  = FlashChipSpeed;
	_doc["FlashChipSize"]   = FlashChipSize;
	_doc["HeapSize"]        = HeapSize;
	_doc["FreeHeap"]        = FreeHeap;
	_doc["SketchSize"]      = SketchSize;
	_doc["FreeSketchSpace"] = FreeSketchSpace;
	_doc["SketchMD5"]       = SketchMD5;
	_doc["SdkVersion"]      = SdkVersion;
	_doc["ChipID"]          = ChipID;
	_doc["Software"]        = Software;

	serializeJsonPretty(_doc, json);

	return json;
}

/// <summary>
///  Initializes selected values.
/// </summary>
void SystemInfo::init()
{
	HeapSize = ESP.getHeapSize() / 1000;
	FreeHeap = ESP.getFreeHeap() / 1000;
	SketchSize = ESP.getSketchSize() / 1000;
	FreeSketchSpace = ESP.getFreeSketchSpace() / 1000;
	SketchMD5 = ESP.getSketchMD5();
}

/// <summary>
///  Updates dynamic values.
/// </summary>
void SystemInfo::update()
{
	HeapSize = ESP.getHeapSize() / 1000;
	FreeHeap = ESP.getFreeHeap() / 1000;
}
