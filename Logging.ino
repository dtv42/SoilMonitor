// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Logging.ino" company="DTV-Online">
//  Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//  Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// <summary>
//  All functions relating to logging. Note that this file is merged with all other '.ino' files.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

/// <summary>
///  Printing a timestamp when logging.
/// </summary>
/// <param name="_logOutput">Pointer to print output</param>
void printTimestamp(Print* _logOutput)
{
	Log.verbose("initCommander()" CR);

	unsigned long runMillis = millis();
	unsigned long allSeconds = millis() / 1000;
	int runHours = allSeconds / 3600;
	int secsRemaining = allSeconds % 3600;
	int runMinutes = secsRemaining / 60;
	int runSeconds = secsRemaining % 60;

	_logOutput->printf("%02d:%02d:%02d ", runHours, runMinutes, runSeconds);
}

/// <summary>
///  Initialize custom logging using log settings.
/// </summary>
void initLogging()
{
	Log.trace("initCommander()" CR);

	// Set the esp log level
	esp_log_level_set("*",     settings.LogSettings.getEspLevelAll());
	esp_log_level_set("wifi",  settings.LogSettings.getEspLevelWiFi());
	esp_log_level_set("dhcps", settings.LogSettings.getEspLevelDhcps());
	esp_log_level_set("dhcpc", settings.LogSettings.getEspLevelDhcpc());

	Log.begin(settings.LogSettings.getArduinoLogLevelApp(), &Serial);
	Log.setPrefix(printTimestamp);
}
