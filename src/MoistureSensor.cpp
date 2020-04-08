// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MoistureSensor.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "MoistureSensor.h"

/// <summary>
/// Initialize default wet and dry values.
/// </summary>
const float MoistureSensor::WET_VALUE = 1.500;		// The default minimum voltage level in V (100% humidity - in water)
const float MoistureSensor::DRY_VALUE = 3.400;		// The default maximum voltage level in V (0% humidity   - on air)

/// <summary>
/// Default constructor.
/// </summary>
MoistureSensor::MoistureSensor()
{
	Log.trace("MoistureSensor::MoistureSensor()" CR);
}

/// <summary>
/// Constructor setting the analog input GPIO pin number.
/// </summary>
/// <param name="pin">The analog input GPIO pin number</param>
MoistureSensor::MoistureSensor(unsigned short pin)
{
	Log.trace("MoistureSensor::MoistureSensor()" CR);

	setPin(pin);
}

/// <summary>
/// Constructor setting the analog input GPIO pin number.
/// </summary>
/// <param name="pin">The analog input GPIO pin number</param>
/// <param name="name">The sensor name</param>
MoistureSensor::MoistureSensor(unsigned short pin, String name)
{
	Log.trace("MoistureSensor::MoistureSensor()" CR);

	setPin(pin);
	setName(name);
}

/// <summary>
/// Constructor setting the analog input GPIO pin number and calibration data.
/// </summary>
/// <param name="pin">The analog input GPIO pin number</param>
/// <param name="name">The sensor name</param>
/// <param name="wet">The minimum voltage level</param>
/// <param name="dry">The maximum voltage level</param>
MoistureSensor::MoistureSensor(unsigned short pin, String name, float wet, float dry)
{
	Log.trace("MoistureSensor::MoistureSensor()" CR);

	setPin(pin);
	setName(name);
	setWetValue(wet);
	setDryValue(dry);
}

/// <summary>
///  Gets the analog input GPIO pin number.
/// </summary>
/// <returns>The GPIO pin</returns>
const unsigned short MoistureSensor::getPin() const
{
	return _pin;
}

/// <summary>
///  Returns the name of a sensor
/// </summary>
/// <returns>The sensor name</returns>
const String MoistureSensor::getName() const
{
	return _name;
}

/// <summary>
/// Gets the minimum voltage level (in water).
/// </summary>
/// <returns>The wet value</returns>
const float MoistureSensor::getWetValue() const
{
	return (double)_wet / 1000.0;
}

/// <summary>
/// Gets the maximum voltage level (on air).
/// </summary>
/// <returns>The dry value</returns>
const float MoistureSensor::getDryValue() const
{
	return (double)_dry / 1000.0;
}

/// <summary>
/// Returns the current analog input value (mV).
/// </summary>
/// <returns>The analog input value</returns>
const int MoistureSensor::getValue() const
{
	return _value;
}

/// <summary>
/// Returns the current analog input voltage (V).
/// </summary>
/// <returns>The analog input voltage</returns>
const float MoistureSensor::getVoltage() const
{
	return _voltage;
}

/// <summary>
/// Returns the current moisture value (%).
/// </summary>
/// <returns>The humidity in percent</returns>
const int MoistureSensor::getHumidity() const
{
	return _percentage;
}

/// <summary>
///  Sets the analog input GPIO pin number.
/// </summary>
/// <param name="pin">The analog input GPIO pin number</param>
void MoistureSensor::setPin(unsigned short pin)
{
	_pin = pin;
}

/// <summary>
///  Sets the name of a sensor
/// </summary>
/// <param name="name">The sensor name</param>
void MoistureSensor::setName(String name)
{
	_name = (name.length() > MAX_NAME_LEN) ? name.substring(0, MAX_NAME_LEN) : name;
}

/// <summary>
/// Sets the minimum voltage level (in water).
/// </summary>
/// <param name="wet">The minimum voltage level</param>
void MoistureSensor::setWetValue(float wet)
{
	_wet = (int)(wet * 1000);
}

/// <summary>
/// Sets the maximum voltage level (on air).
/// </summary>
/// <param name="dry">The maximum voltage level</param>
void MoistureSensor::setDryValue(float dry)
{
	_dry = (int)(dry * 1000);
}

/// <summary>
/// Initializes the sensor (smoothing).
/// </summary>
void MoistureSensor::begin()
{
	Log.trace("MoistureSensor::begin()" CR);

	_sensor.begin(SMOOTHED_EXPONENTIAL, 10);
}

/// <summary>
/// Updates the sensor data (smoothing).
/// </summary>
void MoistureSensor::update()
{
	Log.verbose("MoistureSensor::update()" CR);

	_value = analogRead(_pin);
	_sensor.add(_value);
	_voltage = _sensor.get() / 1000.0;
	_percentage = (int)min(100L, max(0L, map(_sensor.get(), _wet, _dry, 100, 0)));
}
