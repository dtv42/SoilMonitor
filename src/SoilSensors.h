// --------------------------------------------------------------------------------------------------------------------
// <copyright file="SoilSensors.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "MoistureSensor.h"

/// <summary>
/// This class implements a list of soil moisture sensors.
/// </summary>
class SoilSensors
{
public:
	static const unsigned short MAX_SENSORS = 6;					// Number of soil moisture sensors
	static unsigned short PINS[MAX_SENSORS];						// Analog input pins (ADC1)

private:
	static const int CAPACITY =										// The maximum size for the JSON document
		JSON_ARRAY_SIZE(6) +
	6 * JSON_OBJECT_SIZE(4) + 378;
	StaticJsonDocument<CAPACITY> _doc;								// The static JSON document

	static const unsigned short ADC1_CH0 = A0;						// GPIO36 ESP32 pin 14
	static const unsigned short ADC1_CH3 = A3;						// GPIO39 ESP32 pin 13
	static const unsigned short ADC1_CH6 = A6;						// GPIO34 ESP32 pin 12
	static const unsigned short ADC1_CH7 = A7;						// GPIO35 ESP32 pin 11
	static const unsigned short ADC1_CH4 = A4;						// GPIO32 ESP32 pin 10
	static const unsigned short ADC1_CH5 = A5;						// GPIO33 ESP32 pin  9

	unsigned short _enabled[MAX_SENSORS] = {						// Flag indicating that the sensor is enabled
		false,
		false,
		false,
		false,
		false,
		false
	};

	MoistureSensor _sensors[MAX_SENSORS] = {						
		MoistureSensor(SoilSensors::PINS[0], "Sensor 1"),			// Instance of moisture sensor 1
		MoistureSensor(SoilSensors::PINS[1], "Sensor 2"),			// Instance of moisture sensor 2
		MoistureSensor(SoilSensors::PINS[2], "Sensor 3"),			// Instance of moisture sensor 3
		MoistureSensor(SoilSensors::PINS[3], "Sensor 4"),			// Instance of moisture sensor 4
		MoistureSensor(SoilSensors::PINS[4], "Sensor 5"),			// Instance of moisture sensor 5
		MoistureSensor(SoilSensors::PINS[5], "Sensor 6") 			// Instance of moisture sensor 6
	};

public:
	SoilSensors();													// Default constructor

	void setDataByIndex(unsigned short index, String name,			// Initialize soil sensor data
		float wet, float dry, bool enabled = true);
	unsigned short getPinByIndex(unsigned short index);				// Returns the analog input pin
	String getNameByIndex(unsigned short index);					// Returns the name of a sensor
	void setNameByIndex(unsigned short index, String name);			// Sets the name of a sensor
	void enableByIndex(unsigned short index, bool enabled = true);	// Sets the enabled flag
	float getWetValueByIndex(unsigned short index);					// Returns the wet sensor value
	float getDryValueByIndex(unsigned short index);					// Returns the dry sensor value
	bool isEnabledByIndex(unsigned short index);					// Returns the enabled flag
	int getValueByIndex(unsigned short index);						// Returns the raw sensor value (mV)
	float getVoltageByIndex(unsigned short index);					// Returns the sensor voltage (V)
	int getHumidityByIndex(unsigned short index);					// Returns the humidity sensor value (%)

	void begin();													// Initializes all sensors
	void update();													// Updates all sensors

	String serializeByIndex(unsigned short index);					// Return a string serialization (JSON)
	String serialize();												// Return a string serialization (JSON)
};