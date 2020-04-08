// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TempSensors.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <math.h>
#include <ArduinoLog.h>
#include "TempSensors.h"

/// <summary>
///  Constructor.
/// </summary>
TempSensors::TempSensors()
{
	Log.trace("TempSensors::TempSensors()" CR);
}

/// <summary>
///  Constructor setting the OneWire GPIO pin.
/// </summary>
/// <param name="pin">GPIO pin</param>
TempSensors::TempSensors(unsigned short pin)
{
	Log.trace("TempSensors::TempSensors()" CR);
	setPin(pin);
}

/// <summary>
///  Initializes sensor address and connection status.
/// </summary>
/// <param name="index"></param>
void TempSensors::initialize(unsigned short index)
{
	Log.trace("TempSensors::initialize()" CR);

	DeviceAddress address;
	String hex = String();

	if (index < MAX_SENSORS)
	{
		if (_sensors.getAddress(address, index))
		{
			for (int i = 0; i < 8; i++)
			{
				_addresses[index][i] = address[i];
				_resolution[index] = _sensors.getResolution(address);
			}

			_connected[index] = true;
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				_addresses[index][i] = 0;
			}

			_connected[index] = false;
		}
	}
}

/// <summary>
///  Returns the device address as a HEX String
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
String TempSensors::convert(DeviceAddress address)
{
	String hex = "";

	for (int i = 0; i < 8; i++)
	{
		hex += String(address[i], HEX);
	}

	hex.toUpperCase();
	return hex;
}

/// <summary>
/// Sets the digital GPIO pin for the OneWire communication.
/// </summary>
/// <returns>The OneWire Pin</returns>
unsigned short TempSensors::getPin()
{
	Log.trace("TempSensors::getPin()" CR);
	return _pin;
}

/// <summary>
/// Sets the digital GPIO pin for the OneWire communication.
/// </summary>
/// <param name="pin">The GPIO pin</param>
void TempSensors::setPin(unsigned short pin)
{
	Log.trace("TempSensors::setPin()" CR);
	_pin = pin;
	_oneWire.begin(pin);
}

/// <summary>
///  Returns the name of the specified temperature sensor.
///  Note that an empty string is returned when the sensor can not be found.
/// </summary>
/// <param name="index">Sensor index</param>
/// <returns>The sensor name</returns>
String TempSensors::getNameByIndex(unsigned short index)
{
	Log.trace("TempSensors::getNameByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _names[index];
	}
	else
	{
		Log.error("TempSensors::getNameByIndex() Temp Sensor not found" CR);
	}

	return String();
}

/// <summary>
///  Sets the name of the specified temperature sensor.
/// </summary>
/// <param name="index">Sensor index</param>
/// <param name="name">The sensor name</param>
void TempSensors::setNameByIndex(unsigned short index, String name)
{
	Log.trace("TempSensors::setNameByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		_names[index] = (name.length() > MAX_NAME_LEN) ? name.substring(0, MAX_NAME_LEN) : name;
	}
	else
	{
		Log.error("TempSensors::setNameByIndex() Temp Sensor not found" CR);
	}
}

/// <summary>
///  Returns the OneWire address of the specified temperature sensor.
///  Note that an empty string is returned when the sensor can not be found.
/// </summary>
/// <param name="index">Sensor index</param>
/// <returns>The sensor address</returns>
String TempSensors::getAddressByIndex(unsigned short index)
{
	Log.trace("TempSensors::getAddressByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return convert(_addresses[index]);
	}
	else
	{
		Log.error("TempSensors::getAddressByIndex() Temp Sensor not found" CR);
	}

	return String();
}

/// <summary>
///  Returns the precision of the specified temperature sensor.
///  Note that zero indicates that a sensor can not be found.
/// </summary>
/// <param name="index">Sensor index</param>
/// <returns>The sensor precision</returns>
int TempSensors::getResolutionByIndex(unsigned short index)
{
	Log.trace("TempSensors::getResolutionByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _resolution[index];
	}
	else
	{
		Log.error("TempSensors::getResolutionByIndex() Temp Sensor not found" CR);
	}

	return 0;
}

/// <summary>
///  Returns true if the specified sensor is connected.
/// </summary>
/// <param name="index">Sensor index</param>
/// <returns>True if connected</returns>
bool TempSensors::isConnectedByIndex(unsigned short index)
{
	Log.trace("TempSensors::isConnectedByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _connected[index];
	}
	else
	{
		Log.error("TempSensors::isConnectedByIndex() Temp Sensor not found" CR);
	}

	return false;
}

/// <summary>
///  Returns the temperature in Celsius for the specified sensor.
/// </summary>
/// <param name="index">Sensor index</param>
/// <returns>The temperature value</returns>
float TempSensors::getTempCByIndex(unsigned short index)
{
	Log.trace("TempSensors::getTempCByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return _tempC[index];
	}
	else
	{
		Log.error("TempSensors::getTempCByIndex() Temp Sensor not found" CR);
	}

	return DEVICE_DISCONNECTED_C;
}

/// <summary>
///  Returns the temperature in Fahrenheit for the specified sensor.
/// </summary>
/// <param name="index">Sensor index</param>
/// <returns>The temperature value</returns>
float TempSensors::getTempFByIndex(unsigned short index)
{
	Log.trace("TempSensors::getTempFByIndex()" CR);

	if (index < MAX_SENSORS)
	{
		return DallasTemperature::toFahrenheit(_tempC[index]);
	}
	else
	{
		Log.error("TempSensors::getTempFByIndex() Temp Sensor not found" CR);
	}

	return DEVICE_DISCONNECTED_F;
}

/// <summary>
///  Initializes all temperature sensors (bus initialization).
/// </summary>
void TempSensors::begin()
{
	Log.trace("TempSensors::begin()" CR);

	_sensors.begin();
	_sensors.setResolution(GLOBAL_RESOLUTION);
	_sensors.setWaitForConversion(true);
	_sensors.setCheckForConversion(true);

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		initialize(i);
	}
}

/// <summary>
///  Updates temperatures (and connection state) on all sensors.
/// </summary>
void TempSensors::update()
{
	_sensors.requestTemperatures();

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		_tempC[i] = _sensors.getTempCByIndex(i);

		if (_tempC[i] == DEVICE_DISCONNECTED_C)
		{
			_connected[i] == false;
		}
		else
		{
			_connected[i] == true;
		}
	}
}

/// <summary>
///  Serialize a single TempSensor instance data to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String TempSensors::serializeByIndex(unsigned short index)
{
	Log.trace("TempSensors::serializeByIndex()" CR);
	String json;
	_doc.clear();

	if (index < MAX_SENSORS)
	{
		_doc["Name"]      = getNameByIndex(index);
		_doc["Address"] = getAddressByIndex(index);
		_doc["Connected"] = isConnectedByIndex(index);
		_doc["Resolution"] = getResolutionByIndex(index);
		_doc["TempC"]     = getTempCByIndex(index);
		_doc["TempF"]     = getTempFByIndex(index);
	}
	else
	{
		Log.error("TempSensors::serializeByIndex() Temp Sensor not found" CR);
	}

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Serialize the TemperatureSensors instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String TempSensors::serialize()
{
	Log.trace("TempSensors::serialize()" CR);
	String json;
	_doc.clear();

	for (int i = 0; i < MAX_SENSORS; i++)
	{
		JsonObject obj = _doc.createNestedObject();
		obj["Name"]      = getNameByIndex(i);
		obj["Address"]   = getAddressByIndex(i);
		obj["Connected"] = isConnectedByIndex(i);
		obj["Resolution"] = getResolutionByIndex(i);
		obj["TempC"]     = getTempCByIndex(i);
		obj["TempF"]     = getTempFByIndex(i);
	}

	serializeJsonPretty(_doc, json);
	return json;
}
