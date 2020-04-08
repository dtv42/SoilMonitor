// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TempSensors.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/// <summary>
/// This class implements a list of temperature sensors.
/// </summary>
class TempSensors
{
public:
	static const unsigned short MAX_SENSORS = 6;				// Maximum number of temperature sensors
	static const unsigned short ONE_WIRE_BUS = 4;				// GPIO04 ESP32 pin 5 (default)

private:
	static const int CAPACITY = 								// The maximum size for the JSON document		
		JSON_ARRAY_SIZE(6) +
		JSON_OBJECT_SIZE(5) +
	5 * JSON_OBJECT_SIZE(6) + 565;
	StaticJsonDocument<CAPACITY> _doc;							// The static JSON document

	const int MAX_NAME_LEN = 32;								// The maximum length for the sensor name
	const int GLOBAL_RESOLUTION = 12;							// The sensor resolution settings (global)

	unsigned short _pin = ONE_WIRE_BUS;							// The GPIO pin number
	OneWire _oneWire = OneWire(ONE_WIRE_BUS);					// The OneWire bus instance 
	DallasTemperature _sensors = DallasTemperature(&_oneWire);	// The Dallas DS18B20 sensors

	float _tempC[MAX_SENSORS] = {								// The sensor temperature in °C
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0
	};

	int _resolution[MAX_SENSORS] = {							// The sensor precision (9..12 bit)
		0,
		0,
		0,
		0,
		0,
		0
	};

	String _names[MAX_SENSORS] = {								// The sensor names
		"Sensor 1",
		"Sensor 2",
		"Sensor 3",
		"Sensor 4",
		"Sensor 5",
		"Sensor 6"
	};

	DeviceAddress _addresses[MAX_SENSORS] = {					// The sensor device addresses
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0 }
	};

	bool _connected[MAX_SENSORS] = {							// The sensor connection states
		false,
		false,
		false,
		false,
		false,
		false
	};

	void initialize(unsigned short index);						// Initializes sensor address and connection status
	String convert(DeviceAddress address);						// Returns the device address as a HEX String
public:
	TempSensors();												// Default constructor
	TempSensors(unsigned short pin);							// Initializes the GPIO pin number

	unsigned short getPin();									// Returns the GPIO pin number
	void setPin(unsigned short);								// Sets the GPIO pin number
	String getNameByIndex(unsigned short index);				// Returns the name of a sensor
	void setNameByIndex(unsigned short index, String name);		// Sets the name of a sensor
	String getAddressByIndex(unsigned short index);				// Returns the OneWire address of a sensor
	int getResolutionByIndex(unsigned short index);				// Returns the sensor temperature resolution
	bool isConnectedByIndex(unsigned short index);				// Returns true if sensor is connected
	float getTempCByIndex(unsigned short index);				// Returns the temperature value in Celsius
	float getTempFByIndex(unsigned short index);				// Returns the temperature value in Farenheit

	void begin();												// Initializes all sensors
	void update();												// Updates temperatures on all sensors

	String serializeByIndex(unsigned short index);				// Return a sensor string serialization (JSON)
	String serialize();											// Return a string serialization (JSON)
};

