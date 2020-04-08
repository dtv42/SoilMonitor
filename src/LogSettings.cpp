// --------------------------------------------------------------------------------------------------------------------
// <copyright file="LogSettings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include "LogSettings.h"

/// <summary>
/// Initializes selected data fields to default values.
/// </summary>
LogSettings::LogSettings() :
	_logLevelApp(LOG_LEVEL_ERROR),
	_logLevelAll(ESP_LOG_ERROR),
	_logLevelWiFi(ESP_LOG_ERROR),
	_logLevelDhcps(ESP_LOG_ERROR),
	_logLevelDhcpc(ESP_LOG_ERROR)
{
	Log.trace("LogSettings::LogSettings()" CR);
}

// set log level for the application
bool LogSettings::setLogLevelApp(String level)
{
	if (isLogLevel(level))
	{
		_logLevelApp = convertLogLevel(level);
		return true;
	}

	return false;
}

// Set log level component (all)
bool LogSettings::setLogLevelAll(String level)
{
	if (isEspLevel(level))
	{
		_logLevelAll = convertEspLevel(level);
		return true;
	}

	return false;
}

// Set log level component (wifi)
bool LogSettings::setLogLevelWiFi(String level)
{
	if (isEspLevel(level))
	{
		_logLevelWiFi = convertEspLevel(level);
		return true;
	}

	return false;
}

// Set log level component (dhcps)
bool LogSettings::setLogLevelDhcps(String level)
{
	if (isEspLevel(level))
	{
		_logLevelDhcps = convertEspLevel(level);
		return true;
	}

	return false;
}

// Set log level component (dhcpc)
bool LogSettings::setLogLevelDhcpc(String level)
{
	if (isEspLevel(level))
	{
		_logLevelDhcpc = convertEspLevel(level);
		return true;
	}

	return false;
}

// Set log level for the application
String LogSettings::getLogLevelApp()
{
	return convertLogLevel(_logLevelApp);
}

// Set log level component (all)
String LogSettings::getLogLevelAll()
{
	return convertEspLevel(_logLevelAll);
}

// Set log level component (wifi)
String LogSettings::getLogLevelWiFi()
{
	return convertEspLevel(_logLevelWiFi);
}

// Set log level component (dhcps)
String LogSettings::getLogLevelDhcps()
{
	return convertEspLevel(_logLevelDhcps);
}

// Set log level component (dhcpc)
String LogSettings::getLogLevelDhcpc()
{
	return convertEspLevel(_logLevelDhcpc);
}

// ArduinoLog level for the application
int LogSettings::getArduinoLogLevelApp()
{
	return _logLevelApp;
}

// ESP log level component (all)
esp_log_level_t LogSettings::getEspLevelAll()
{
	return _logLevelAll;
}

// ESP log level component (wifi)
esp_log_level_t LogSettings::getEspLevelWiFi()
{
	return _logLevelWiFi;
}

// ESP log level component (dhcps)
esp_log_level_t LogSettings::getEspLevelDhcps()
{
	return _logLevelDhcps;
}

// ESP log level component (dhcpc)
esp_log_level_t LogSettings::getEspLevelDhcpc()
{
	return _logLevelDhcpc;
}

/// <summary>
///  Deserialize the data fields from a JSON string.
/// </summary>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool LogSettings::deserialize(String json)
{
	Log.trace("LogSettings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("LogSettings::deserialize() Deserialize JSON failed with code %s" CR, err.c_str());
			return false;
		}

		String logLevelApp   = _doc["Level"] | convertLogLevel(_logLevelApp);
		String logLevelAll   = _doc["All"]   | convertEspLevel(_logLevelAll);
		String logLevelWiFi  = _doc["WiFi"]  | convertEspLevel(_logLevelWiFi);
		String logLevelDhcps = _doc["Dhcps"] | convertEspLevel(_logLevelDhcps);
		String logLevelDhcpc = _doc["Dhcpc"] | convertEspLevel(_logLevelDhcpc);

		_logLevelApp   = convertLogLevel(logLevelApp);
		_logLevelAll   = convertEspLevel(logLevelAll);
		_logLevelWiFi  = convertEspLevel(logLevelWiFi);
		_logLevelDhcps = convertEspLevel(logLevelDhcps);
		_logLevelDhcpc = convertEspLevel(logLevelDhcpc);

		return true;
	}

	Log.warning("LogSettings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String LogSettings::serialize()
{
	Log.trace("LogSettings::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Level"] = convertLogLevel(_logLevelApp);
	_doc["All"]   = convertEspLevel(_logLevelAll);
	_doc["WiFi"]  = convertEspLevel(_logLevelWiFi);
	_doc["Dhcps"] = convertEspLevel(_logLevelDhcps);
	_doc["Dhcpc"] = convertEspLevel(_logLevelDhcpc);

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Convert ArduinoLog level to string
/// </summary>
/// <param name="level">The log level</param>
/// <returns>Log level string</returns>
String LogSettings::convertLogLevel(int level)
{
	if (level == LOG_LEVEL_SILENT)  return "SILENT";
	if (level == LOG_LEVEL_FATAL)   return "FATAL";
	if (level == LOG_LEVEL_ERROR)   return "ERROR";
	if (level == LOG_LEVEL_WARNING) return "WARNING";
	if (level == LOG_LEVEL_NOTICE)  return "NOTICE";
	if (level == LOG_LEVEL_TRACE)   return "TRACE";
	if (level == LOG_LEVEL_VERBOSE) return "VERBOSE";

	Log.warning("LogSettings::convertLogLevel() Invalid log level: %d" CR, level);
	return String("INVALID");
}

/// <summary>
///  Convert ArduinoLog level from string
/// </summary>
/// <param name="level">Log level string</param>
/// <returns>Log level</returns>
int LogSettings::convertLogLevel(String level)
{
	level.toUpperCase();
	if (level == "SILENT")  return LOG_LEVEL_SILENT;
	if (level == "FATAL")   return LOG_LEVEL_FATAL;
	if (level == "ERROR")   return LOG_LEVEL_ERROR;
	if (level == "WARNING") return LOG_LEVEL_WARNING;
	if (level == "NOTICE")  return LOG_LEVEL_NOTICE;
	if (level == "TRACE")   return LOG_LEVEL_TRACE;
	if (level == "VERBOSE") return LOG_LEVEL_VERBOSE;

	Log.warning("LogSettings::convertLogLevel() Invalid log level: %s" CR, level);
	return LOG_LEVEL_SILENT;
}

/// <summary>
///  Convert ESP log level to string
/// </summary>
/// <param name="level">Esp log level</param>
/// <returns>Log level string</returns>
String LogSettings::convertEspLevel(esp_log_level_t level)
{
	if (level == ESP_LOG_NONE)    return "NONE";
	if (level == ESP_LOG_ERROR)   return "ERROR";
	if (level == ESP_LOG_WARN)    return "WARN";
	if (level == ESP_LOG_INFO)    return "INFO";
	if (level == ESP_LOG_DEBUG)   return "DEBUG";
	if (level == ESP_LOG_VERBOSE) return "VERBOSE";

	return String("INVALID");
}

/// <summary>
///  Convert ESP log level from string
/// </summary>
/// <param name="level">Esp log level string</param>
/// <returns>Esp log level</returns>
esp_log_level_t LogSettings::convertEspLevel(String level)
{
	level.toUpperCase();
	if (level == "NONE")    return ESP_LOG_NONE;
	if (level == "ERROR")   return ESP_LOG_ERROR;
	if (level == "WARN")    return ESP_LOG_WARN;
	if (level == "INFO")    return ESP_LOG_INFO;
	if (level == "DEBUG")   return ESP_LOG_DEBUG;
	if (level == "VERBOSE") return ESP_LOG_VERBOSE;

	Log.warning("LogSettings::convertEspLevel() Invalid log level: %s" CR, level);
	return ESP_LOG_NONE;
}

/// <summary>
///  Convert ESP log level from integer
/// </summary>
/// <param name="level">Log level</param>
/// <returns>Esp log level</returns>
esp_log_level_t LogSettings::convertEspLevel(int level)
{
	if (level == ESP_LOG_NONE)    return ESP_LOG_NONE;
	if (level == ESP_LOG_ERROR)   return ESP_LOG_ERROR;
	if (level == ESP_LOG_WARN)    return ESP_LOG_WARN;
	if (level == ESP_LOG_INFO)    return ESP_LOG_INFO;
	if (level == ESP_LOG_DEBUG)   return ESP_LOG_DEBUG;
	if (level == ESP_LOG_VERBOSE) return ESP_LOG_VERBOSE;

	Log.warning("LogSettings::convertEspLevel() Invalid log level: %s" CR, level);
	return ESP_LOG_NONE;
}

/// <summary>
///  Returns true if is a valid ArduinoLog level.
/// </summary>
/// <param name="level">Log level string</param>
/// <returns>True if level is valid</returns>
bool LogSettings::isLogLevel(String level)
{
	level.toUpperCase();

	if (level == "SILENT")  return true;
	if (level == "FATAL")   return true;
	if (level == "ERROR")   return true;
	if (level == "WARNING") return true;
	if (level == "NOTICE")  return true;
	if (level == "TRACE")   return true;
	if (level == "VERBOSE") return true;

	return false;
}

/// <summary>
///  Returns true if is a valid ESP log level.
/// </summary>
/// <param name="level">Esp log level string</param>
/// <returns>True if level is valid</returns>
bool LogSettings::isEspLevel(String level)
{
	level.toUpperCase();

	if (level == "NONE")    return true;
	if (level == "ERROR")   return true;
	if (level == "WARN")    return true;
	if (level == "INFO")    return true;
	if (level == "DEBUG")   return true;
	if (level == "VERBOSE") return true;

	return false;
}

/// <summary>
///  Resets all field to their default values.
/// </summary>
void LogSettings::reset()
{
	Log.trace("LogSettings::reset()" CR);

	_logLevelApp = LOG_LEVEL_ERROR;
	_logLevelAll = ESP_LOG_ERROR;
	_logLevelWiFi = ESP_LOG_ERROR;
	_logLevelDhcps = ESP_LOG_ERROR;
	_logLevelDhcpc = ESP_LOG_ERROR;
}