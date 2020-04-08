// --------------------------------------------------------------------------------------------------------------------
// <copyright file="CmdSettings.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "CmdSettings.h"

/// <summary>
/// Initializes selected data fields to default values.
/// </summary>
CmdSettings::CmdSettings() :
	Prompt(CMDR_PROMPT),
	PassPhrase(""),
	LocalName(BT_LOCAL_NAME),
	UseBluetooth(false),
	HardLock(false),
	Locked(false),
	ErrorMessages(true),
	CommandPrompt(true)
{
	Log.trace("CmdSettings::CmdSettings()" CR);
}

/// <summary>
///  Deserialize the data fields from a JSON string.
/// </summary>
/// <param name="json">The JSON string</param>
/// <returns>True if successful</returns>
bool CmdSettings::deserialize(String json)
{
	Log.trace("CmdSettings::deserialize()" CR);

	if (json.length() > 0)
	{
		DeserializationError err = deserializeJson(_doc, json);

		if (err)
		{
			Log.error("CmdSettings::deserialize() Deserialize JSON failed with code %s" CR, err.c_str());
			return false;
		}

		Prompt        = _doc["Prompt"]        | Prompt;
		PassPhrase    = _doc["PassPhrase"]    | PassPhrase;
		LocalName     = _doc["LocalName"]     | LocalName;
		UseBluetooth  = _doc["UseBluetooth"]  | UseBluetooth;
		HardLock      = _doc["HardLock"]      | HardLock;
		Locked        = _doc["Locked"]        | Locked;
		ErrorMessages = _doc["ErrorMessages"] | ErrorMessages;
		CommandPrompt = _doc["CommandPrompt"] | CommandPrompt;

		return true;
	}

	Log.warning("CmdSettings::deserialize() Invalid JSON string" CR);
	return false;
}

/// <summary>
///  Serialize the class instance to a JSON string.
/// </summary>
/// <returns>The JSON string</returns>
String CmdSettings::serialize()
{
	Log.trace("CmdSettings::serialize()" CR);
	String json;

	_doc.clear();
	_doc["Prompt"]        = Prompt;
	_doc["PassPhrase"]    = PassPhrase;
	_doc["LocalName"]     = LocalName;
	_doc["UseBluetooth"]  = UseBluetooth;
	_doc["HardLock"]      = HardLock;
	_doc["Locked"]        = Locked;
	_doc["ErrorMessages"] = ErrorMessages;
	_doc["CommandPrompt"] = CommandPrompt;

	serializeJsonPretty(_doc, json);
	return json;
}

/// <summary>
///  Resets all field to their default values.
/// </summary>
void CmdSettings::reset()
{
	Log.trace("CmdSettings::reset()" CR);

	Prompt = CMDR_PROMPT;
	PassPhrase = "";
	LocalName = BT_LOCAL_NAME;
	UseBluetooth = false;
	HardLock = false;
	Locked = false;
	ErrorMessages = true;
	CommandPrompt = true;
}