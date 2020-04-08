// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SoilSettings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "SoilSettings.h"

/// <summary>
///  Constructor settings sensors.
/// </summary>
/// <param name="sensors">Pointer to soil sensors</param>
SoilSettings::SoilSettings(SoilSensors* sensors) :
	_sensors(sensors)
{
	Log.trace("SoilSettings::SoilSettings()" CR);

	for (unsigned short i = 0; i < MAX_SENSORS; i++)
	{
		Names[i] = _sensors->getNameByIndex(i);
		Pins[i] = _sensors->getPinByIndex(i);
		WetValues[i] = _sensors->getWetValueByIndex(i);
		DryValues[i] = _sensors->getDryValueByIndex(i);
		Enabled[i] = _sensors->isEnabledByIndex(i);
	}
}

/// <summary>
///  Deserialize the data fields of the specified sensor from a JSON string.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool SoilSettings::deserializeByIndex(unsigned short index, String json)
{
	Log.trace("SoilSettings::deserializeByIndex()" CR);

	if (json.length() > 0)
	{
		if (index < MAX_SENSORS)
		{
			DeserializationError err = deserializeJson(_doc, json);

			if (err)
			{
				Log.error("SoilSettings::deserializeByIndex() Deserialize JSON failed with code %s", err.c_str());
				return false;
			}

			Names[index]     = _doc["Name"]    | Names[index];
			WetValues[index] = _doc["Wet"]     | WetValues[index];
			DryValues[index] = _doc["Dry"]     | DryValues[index];
			Enabled[index]   = _doc["Enabled"] | Enabled[index];

			_sensors->setDataByIndex(index, Names[index], WetValues[index], DryValues[index], Enabled[index]);

			return true;
		}
		else
		{
			Log.error("SoilSettings::deserializeByIndex() Soil Sensor not found" CR);
		}
	}

	Log.warning("SoilSettings::deserializeByIndex() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Deserialize the data fields from a JSON string.
///  Note that the fixed pin numbers are not changed.
/// </summary>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool SoilSettings::deserialize(String json)
{
	Log.trace("SoilSettings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("SoilSettings::deserialize() Deserialize JSON failed with code %s", err.c_str());
			return false;
		}

		for (int i = 0; i < MAX_SENSORS; i++)
		{
			JsonObject obj = _doc[i];

			Names[i]     = obj["Name"]    | Names[i];
			WetValues[i] = obj["Wet"]     | WetValues[i];
			DryValues[i] = obj["Dry"]     | DryValues[i];
			Enabled[i]   = obj["Enabled"] | Enabled[i];

			_sensors->setDataByIndex(i, Names[i], WetValues[i], DryValues[i], Enabled[i]);
		}

		return true;
	}

	Log.warning("SoilSettings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize a single SoilSensor setting to a JSON string.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The JSON string</returns>
String SoilSettings::serializeByIndex(unsigned short index)
{
	Log.trace("SoilSettings::serializeByIndex()" CR);
	String json;
	
	_doc.clear();

	if (index < MAX_SENSORS)
	{
		_doc["Name"]    = Names[index];
		_doc["Pin"]     = Pins[index];
		_doc["Wet"]     = WetValues[index];
		_doc["Dry"]     = DryValues[index];
		_doc["Enabled"] = Enabled[index];
	}
	else
	{
		Log.error("SoilSettings::serializeByIndex() Soil Sensor not found" CR);
	}

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String SoilSettings::serialize()
{
	Log.trace("SoilSettings::serialize()" CR);
	String json;

	_doc.clear();

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		JsonObject obj = _doc.createNestedObject();
		obj["Name"]    = Names[i];
		obj["Pin"]     = Pins[i];
		obj["Wet"]     = WetValues[i];
		obj["Dry"]     = DryValues[i];
		obj["Enabled"] = Enabled[i];
	}

	serializeJsonPretty(_doc, json);
	return json;
}
