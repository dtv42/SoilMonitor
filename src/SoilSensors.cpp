// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SoilSensors.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "SoilSensors.h"

/// <summary>
/// Initialize fixed analog input pins (ADC1).
/// </summary>
unsigned short SoilSensors::PINS[MAX_SENSORS] = {
	ADC1_CH0,
	ADC1_CH3,
	ADC1_CH6,
	ADC1_CH7,
	ADC1_CH4,
	ADC1_CH5
};

/// <summary>
///  Default constructor.
/// </summary>
SoilSensors::SoilSensors()
{
	Log.trace("SoilSensors::SoilSensors()" CR);
}

/// <summary>
///  Sets calibration and enable data for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <param name="wet">The wet calibration value (V)</param>
/// <param name="dry">The dry calibration value (V)</param>
/// <param name="enabled">The enabled flag</param>
void SoilSensors::setDataByIndex(unsigned short index, String name, float wet, float dry, bool enabled)
{
	Log.trace("SoilSensors::setDataByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		_sensors[index].setName(name);
		_sensors[index].setWetValue(wet);
		_sensors[index].setDryValue(dry);
		_enabled[index] = enabled;
	}
	else
	{
		Log.error("SoilSensors::setDataByIndex() Soil Sensor not found" CR);
	}
}

/// <summary>
///  Returns the analog input pin.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The analog input pin</returns>
unsigned short SoilSensors::getPinByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getPinByIndex()" CR);
	return _sensors[index].getPin();
}

/// <summary>
///  Returns the name of the specified sensor.
///  Note that an empty string is returned when the sensor can not be found.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The sensor name</returns>
String SoilSensors::getNameByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getNameByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _sensors[index].getName();
	}
	else
	{
		Log.error("SoilSensors::getNameByIndex() Soil Sensor not found" CR);
	}

	return String();
}

/// <summary>
///  Sets the name of the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <param name="name">The sensor name</param>
void SoilSensors::setNameByIndex(unsigned short index, String name)
{
	Log.trace("SoilSensors::setNameByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		_sensors[index].setName(name);
	}
	else
	{
		Log.error("SoilSensors::setNameByIndex() Soil Sensor not found" CR);
	}
}

/// <summary>
///  Sets the enabled flag for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <param name="enabled">The enabled flag</param>
void SoilSensors::enableByIndex(unsigned short index, bool enabled)
{
	Log.trace("SoilSensors::enableByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		_enabled[index] = enabled;
	}
	else
	{
		Log.error("SoilSensors::enableByIndex() Soil Sensor not found" CR);
	}
}

/// <summary>
///  Returns the wet calibration value (V) for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The wet calibration value</returns>
float SoilSensors::getWetValueByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getWetValueByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _sensors[index].getWetValue();
	}
	else
	{
		Log.error("SoilSensors::getWetValueByIndex() Soil Sensor not found" CR);
	}

	return 0.0;
}

/// <summary>
///  Returns the dry calibration value (V) for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The dry calibration value</returns>
float SoilSensors::getDryValueByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getDryValueByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _sensors[index].getDryValue();
	}
	else
	{
		Log.error("SoilSensors::getDryValueByIndex() Soil Sensor not found" CR);
	}

	return 0.0;
}

/// <summary>
///  Returns true if the specified sensor is enabled.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The enabled flag</returns>
bool SoilSensors::isEnabledByIndex(unsigned short index)
{
	Log.trace("SoilSensors::isEnabledByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _enabled[index];
	}
	else
	{
		Log.error("SoilSensors::isEnabledByIndex() Soil Sensor not found" CR);
	}

	return false;
}

/// <summary>
///  Returns the current analog input value (mV) for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The input value</returns>
int SoilSensors::getValueByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getValueByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _sensors[index].getValue();
	}
	else
	{
		Log.error("oilSensors::getValueByIndex() Soil Sensor not found" CR);
	}

	return 0;
}

/// <summary>
///  Returns the analog input value (V) for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The analog input value</returns>
float SoilSensors::getVoltageByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getVoltageByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _sensors[index].getVoltage();
	}
	else
	{
		Log.error("SoilSensors::getVoltageByIndex() Soil Sensor not found" CR);
	}

	return 0.0;
}

/// <summary>
///  Returns the moisture value (%) for the specified sensor.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The humidity in percent</returns>
int SoilSensors::getHumidityByIndex(unsigned short index)
{
	Log.trace("SoilSensors::getHumidityByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _sensors[index].getHumidity();
	}
	else
	{
		Log.error("SoilSensors::getHumidityByIndex() Soil Sensor not found" CR);
	}

	return 0;
}

/// <summary>
///  Initializes all soil moisture sensors.
/// </summary>
void SoilSensors::begin()
{
	Log.trace("SoilSensors::begin()" CR);

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		_sensors[i].begin();
	}
}

/// <summary>
///  Updates all soil moisture sensors.
/// </summary>
void SoilSensors::update()
{
	Log.verbose("SoilSensors::update()" CR);

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		_sensors[i].update();
	}
}

/// <summary>
///  Serialize a single SoilSensor instance to a JSON string.
/// </summary>
/// <param name="index">Sensor index (0..5)</param>
/// <returns>The JSON string</returns>
String SoilSensors::serializeByIndex(unsigned short index)
{
	Log.trace("SoilSensors::serializeByIndex()" CR);
	String json;

	_doc.clear();

	if (index < MAX_SENSORS)
	{
		_doc["Name"]     = getNameByIndex(index);
		_doc["Humidity"] = getHumidityByIndex(index);
		_doc["Voltage"]  = getVoltageByIndex(index);
		_doc["Enabled"]  = isEnabledByIndex(index);
	}
	else
	{
		Log.error("SoilSensors::serializeByIndex() Soil Sensor not found" CR);
	}

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Serialize the SoilSensors instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String SoilSensors::serialize()
{
	Log.trace("SoilSensors::serialize()" CR);
	String json;
	
	_doc.clear();

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		JsonObject obj = _doc.createNestedObject();
		obj["Name"]     = getNameByIndex(i);
		obj["Humidity"] = getHumidityByIndex(i);
		obj["Voltage"]  = getVoltageByIndex(i);
		obj["Enabled"]  = isEnabledByIndex(i);
	}

	serializeJsonPretty(_doc, json);
	return json;
}
