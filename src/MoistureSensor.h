// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MoistureSensor.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <math.h>
#include <Arduino.h>
#include <Smoothed.h>

/// <summary>
/// This class implements a moisture sensor using a capacitive soil moisture sensor.
/// </summary>
class MoistureSensor
{
public:
	static const float WET_VALUE;								// The default minimum voltage level in V (100% humidity - in water)
	static const float DRY_VALUE;								// The default maximum voltage level in V (0% humidity   - on air)

private:
	const int MAX_NAME_LEN = 32;								// The maximum length for the sensor name

	String _name;												// The sensor name
	int _wet = MoistureSensor::WET_VALUE * 1000;				// The minimum calibration voltage level in mV (in water)
	int _dry = MoistureSensor::DRY_VALUE * 1000;				// The maximum calibration voltage level in mV (on air)
	unsigned short _pin = A0;									// The analog input pin (default ADC1_CH0)
	int _value = 0;												// The raw analog input value
	float _voltage = 0.0;										// The analog input voltage value
	int _percentage = 0;										// The humidity value in percent (0..100)
	Smoothed<int> _sensor;										// Smoothed sensor value (int)

public:
	MoistureSensor();											// Default constructor
	MoistureSensor(unsigned short pin);							// Constructor setting analog input pin
	MoistureSensor(unsigned short pin, String name);			// Constructor setting analog input pin and sensor name
	MoistureSensor(unsigned short pin, String name,				// Constructor initializing analog input pin, sensor name,
				   float wet, float dry);						// and calibration data

	const unsigned short getPin() const;						// Returns the analog input pin
	const String getName() const;								// Returns the name of a sensor
	const float getWetValue() const;							// Returns the wet calibration value
	const float getDryValue() const;							// Returns the dry calibration value
	const int getValue() const;									// Returns the raw sensor value (mV)
	const float getVoltage() const;								// Returns the sensor value (V)
	const int getHumidity() const;								// Returns the humidity in %

	void setPin(unsigned short pin);							// Sets the analog input pin
	void setName(String name);									// Sets the name of a sensor
	void setWetValue(float wet);								// Sets the wet calibration value
	void setDryValue(float dry);								// Sets the dry calibration value

	void begin();												// Initializes the sensor (smoothing)
	void update();												// Updates the sensor values
};

