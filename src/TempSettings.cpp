// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TempSettings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "TempSettings.h"

/// <summary>
///  Constructor settings sensors.
/// </summary>
/// <param name="sensors">Pointer to temperature sensors</param>
TempSettings::TempSettings(TempSensors* sensors) :
	_sensors(sensors)
{
	Log.trace("TempSettings::TempSettings()" CR);
	Pin = _sensors->getPin();

	for (unsigned short i = 0; i < MAX_SENSORS; i++)
	{
		Names[i] = _sensors->getNameByIndex(i);
	}
}

/// <summary>
///  Deserialize the data fields of the specified sensor from a JSON string.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool TempSettings::deserializeByIndex(unsigned short index, String json)
{
	Log.trace("TempSettings::deserializeByIndex()" CR);

	if (json.length() > 0)
	{
		if (index < MAX_SENSORS)
		{
			DeserializationError err = deserializeJson(_doc, json);

			if (err)
			{
				Log.error("TempSettings::deserializeByIndex() Deserialize JSON failed with code %s", err.c_str());
				return false;
			}

			Names[index] = _doc["Name"] | Names[index];

			if (Names[index] != _sensors->getNameByIndex(index))
			{
				_sensors->setNameByIndex(index, Names[index]);
			}

			return true;
		}
		else
		{
			Log.error("TempSettings::deserializeByIndex() Temp Sensor not found" CR);
		}
	}

	Log.warning("TempSettings::deserializeByIndex() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Deserialize the data fields from a JSON string.
/// </summary>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool TempSettings::deserialize(String json)
{
	Log.trace("TempSettings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("TempSettings::deserialize() Deserialize JSON failed with code %s", err.c_str());
			return false;
		}

		Pin = _doc["Pin"] | Pin;
		_sensors->setPin(Pin);

		for (int i = 0; i < MAX_SENSORS; i++)
		{
			JsonObject obj = _doc["Sensors"][i];
			Names[i] = obj["Name"] | Names[i];
			_sensors->setNameByIndex(i, Names[i]);
		}

		return true;
	}

	Log.warning("TempSettings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize a single TempSensor setting to a JSON string.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The JSON string</returns>
String TempSettings::serializeByIndex(unsigned short index)
{
	Log.trace("TempSettings::serializeByIndex()" CR);
	String json;

	_doc.clear();

	if (index < MAX_SENSORS)
	{
		_doc["Name"] = Names[index];
	}
	else
	{
		Log.error("TempSettings::serializeByIndex() Temp Sensor not found" CR);
	}

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String TempSettings::serialize()
{
	Log.trace("TempSettings::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Pin"] = Pin;

	JsonArray array = _doc.createNestedArray("Sensors");

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		JsonObject obj = array.createNestedObject();
		obj["Name"] = Names[i];
	}

	serializeJsonPretty(_doc, json);
	return json;
}
