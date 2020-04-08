// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Sensors.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "Sensors.h"

/// <summary>
///  Default constructor.
/// </summary>
Sensors::Sensors()
{
	Log.trace("Sensors::Sensors()" CR);
}

/// <summary>
///  Return a string serialization (JSON).
/// </summary>
String Sensors::serialize()
{
	Log.trace("Sensors::serialize()" CR);
	String json;

	_doc.clear();
	_doc["TempSensors"] = serialized(this->TempSensors.serialize());
	_doc["SoilSensors"] = serialized(this->SoilSensors.serialize());

	serializeJsonPretty(_doc, json);
	return json;

}
