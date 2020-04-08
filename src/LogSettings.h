// --------------------------------------------------------------------------------------------------------------------
// <copyright file="LogSettings.h" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <ArduinoJson.h>

/// <summary>
/// This class holds the log level configuration data.
/// 
/// The ArduinoLog levels are:
/// 
///		0 - LOG_LEVEL_SILENT     no output 
///		1 - LOG_LEVEL_FATAL      fatal errors
///		2 - LOG_LEVEL_ERROR      all errors
///		3 - LOG_LEVEL_WARNING    errors, and warnings
///		4 - LOG_LEVEL_NOTICE     errors, warningsand notices
///		5 - LOG_LEVEL_TRACE      errors, warnings, notices& traces
///		6 - LOG_LEVEL_VERBOSE    all
/// 
/// The ESP32 log levels are:
/// 
///     0 - ESP_LOG_NONE        - No log output
///     1 - ESP_LOG_ERROR       - Critical errors, software module can not recover on its own
///     2 - ESP_LOG_WARN        - Error conditions from which recovery measures have been taken
///     3 - ESP_LOG_INFO        - Information messages which describe normal flow of events
///     4 - ESP_LOG_DEBUG       - Extra information which is not necessary for normal use (values, pointers, sizes, etc).
///     5 - ESP_LOG_VERBOSE     - Bigger chunks of debugging information, or frequent messages which can potentially flood the output.
/// 
/// </summary>
class LogSettings
{
private:
	static const int CAPACITY = 						// The maximum size for the JSON document
		JSON_OBJECT_SIZE(5) + 67;
	StaticJsonDocument<CAPACITY> _doc;					// The static JSON document

	int _logLevelApp;									// Log level for the application
	esp_log_level_t _logLevelAll;						// Log level component (all)
	esp_log_level_t _logLevelWiFi;						// Log level component (wifi)
	esp_log_level_t _logLevelDhcps;						// Log level component (dhcps)
	esp_log_level_t _logLevelDhcpc;						// Log level component (dhcpc)

	String convertLogLevel(int level);					// Convert ArduinoLog level to string
	int convertLogLevel(String level);					// Convert ArduinoLog level from string
	String convertEspLevel(esp_log_level_t level);		// Convert ESP log level to string
	esp_log_level_t convertEspLevel(String level);		// Convert ESP log level from string
	esp_log_level_t convertEspLevel(int level);			// Convert ESP log level from integer
	bool isLogLevel(String level);						// Returns true if is a valid ArduinoLog level
	bool isEspLevel(String level);						// Returns true if is a valid ESP log level

public:
	LogSettings();										// Default constructor

	bool setLogLevelApp(String level);					// Log level for the application
	bool setLogLevelAll(String level);					// Log level component (all)
	bool setLogLevelWiFi(String level);					// Log level component (wifi)
	bool setLogLevelDhcps(String level);				// Log level component (dhcps)
	bool setLogLevelDhcpc(String level);				// Log level component (dhcpc)

	String getLogLevelApp();							// Log level for the application
	String getLogLevelAll();							// Log level component (all)
	String getLogLevelWiFi();							// Log level component (wifi)
	String getLogLevelDhcps();							// Log level component (dhcps)
	String getLogLevelDhcpc();							// Log level component (dhcpc)

	int getArduinoLogLevelApp();						// ArduinoLog level for the application
	esp_log_level_t getEspLevelAll();					// ESP log level component (all)
	esp_log_level_t getEspLevelWiFi();					// ESP log level component (wifi)
	esp_log_level_t getEspLevelDhcps();					// ESP log level component (dhcps)
	esp_log_level_t getEspLevelDhcpc();					// ESP log level component (dhcpc)

	bool deserialize(String json);						// Read a JSON string and updates the fields.
	String serialize();									// Return a string serialization (JSON)
	void reset();										// Resets all settings
};

