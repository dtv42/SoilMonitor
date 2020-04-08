// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ErrInfo.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "ErrInfo.h"

/// <summary>
///  Default constructor
/// </summary>
/// <param name="code">The error code</param>
/// <param name="message">The error message</param>
ErrInfo::ErrInfo() :
	Code(0),
	Message("No Error")
{
	Log.trace("ErrInfo::ErrInfo()" CR);
}

bool ErrInfo::deserialize(String json)
{
	Log.trace("ErrInfo::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("ErrInfo::deserialize() Deserialize JSON failed with code %s" CR, err.c_str());
			return false;
		}

		Code    = _doc["Code"] | 0;
		Message = _doc["Message"] | "No Error";

		return true;
	}

	Code = _doc["Code"] | 400;
	Message = _doc["Message"] | "Invalid JSON string";

	Log.warning("ErrInfo::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize the ErrInfoClass instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String ErrInfo::serialize()
{
	Log.trace("ErrInfo::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Code"] = Code;
	_doc["Message"] = Message;

	serializeJsonPretty(_doc, json);
	return json;
}

