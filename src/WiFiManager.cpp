// --------------------------------------------------------------------------------------------------------------------
// <copyright file="WiFiManager.cpp" company="DTV-Online">
//   Copyright(c) 2020 Dr. Peter Trimmel. All rights reserved.
// </copyright>
// <license>
//   Licensed under the MIT license. See the LICENSE file in the project root for more information.
// </license>
// --------------------------------------------------------------------------------------------------------------------
#include <ArduinoLog.h>
#include "time.h"
#include "ApInfo.h"
#include "WiFiManager.h"

/// <summary>
///  Constructor setting a reference to a settings instance.
/// </summary>
/// <param name="settings">Pointer to settings instance</param>
WiFiManager::WiFiManager(Settings* settings) :
	_settings(settings)
{
	Log.trace("WiFiManager::WiFiManager()" CR);
}

/// <summary>
///  Scan the WiFi network for access points.
/// </summary>
/// <returns>Number of access points found</returns>
unsigned short WiFiManager::scan()
{
	Log.trace("WiFiManager::scan()" CR);

	_numberAP = 0;
	WiFi.mode(WIFI_OFF);

	if (wait4Mode(WIFI_STA))
	{
		_numberAP = WiFi.scanNetworks();
	}

	return _numberAP;
}

/// <summary>
///  Returns the AP data at the specified index.
/// </summary>
/// <param name="index">Network item index</param>
/// <returns>Access point info</returns>
WiFiManager::AccessPoint WiFiManager::getAP(unsigned short index)
{
	Log.trace("WiFiManager::getAP()" CR);
	WiFiManager::AccessPoint ap;

	if ((_numberAP > 0) && (index < _numberAP))
	{
		ap.SSID = WiFi.SSID(index);
		ap.RSSI = WiFi.RSSI(index);
		ap.Channel = WiFi.channel(index);

		switch (WiFi.encryptionType(index))
		{
		case WIFI_AUTH_OPEN: ap.Encryption = String("Open"); break;
		case WIFI_AUTH_WEP: ap.Encryption = String("WEP"); break;
		case WIFI_AUTH_WPA_PSK: ap.Encryption = String("WPA_PSK"); break;
		case WIFI_AUTH_WPA2_PSK: ap.Encryption = String("WPA2_PSK"); break;
		case WIFI_AUTH_WPA_WPA2_PSK: ap.Encryption = String("WPA_WPA2_PSK"); break;
		case WIFI_AUTH_WPA2_ENTERPRISE: ap.Encryption = String("WPA2_ENTERPRISE"); break;
		}
	}
	
	return ap;
}

/// <summary>
///  Attempt to connect to WiFi network or create access point.
/// </summary>
/// <returns>True if successful</returns>
bool WiFiManager::connect()
{
	Log.trace("WiFiManager::connect()" CR);

	if (_settings != NULL)
	{
		if (connectAP())
		{
			if (_settings->ApSettings.Backup)
			{
				return createAP();
			}

			return true;
		}
		else
		{
			return createAP();
		}
	}

	return false;
}

/// <summary>
///  Create a WiFi access point.
/// </summary>
/// <returns>True if successful</returns>
bool WiFiManager::createAP()
{
	Log.trace("WiFiManager::createAP()" CR);
	bool ok = false;
	wifi_mode_t mode = WIFI_AP;

	if (_settings != NULL)
	{
		if (_settings->ApSettings.Backup)
		{
			mode = WIFI_AP_STA;
		}

		if (wait4Mode(mode))
		{
			if (_settings->ApSettings.Custom)
			{
				IPAddress address;
				IPAddress gateway;
				IPAddress subnet;

				bool addressOK = address.fromString(_settings->ApSettings.Address);
				bool gatewayOK = gateway.fromString(_settings->ApSettings.Gateway);
				bool subnetOK = subnet.fromString(_settings->ApSettings.Subnet);

				if (addressOK && gatewayOK && subnetOK)
				{
					ok = WiFi.softAPConfig(address, gateway, subnet);

					if (!ok)
					{
						return ok;
					}
				}
				else
				{
					return ok;
				}
			}

			// Create WiFi access point with passphrase.
			if (_settings->ApSettings.PASS != "")
			{
				ok = WiFi.softAP(_settings->ApSettings.SSID.c_str(), _settings->ApSettings.PASS.c_str());
			}
			// Create open WiFi access point. 
			else
			{
				ok = WiFi.softAP(_settings->ApSettings.SSID.c_str());
			}

			delay(100);

			if (ok)
			{
				ok = WiFi.softAPsetHostname(_settings->ApSettings.Hostname.c_str());
			}
		}
	}

	ApInfo::Active = ok;
	return ok;
}

/// <summary>
///  Connect to a WiFi access point.
/// </summary>
/// <returns>True if successful</returns>
bool WiFiManager::connectAP()
{
	Log.trace("WiFiManager::connectAP()" CR);
	bool ok = false;
	wifi_mode_t mode = WIFI_STA;

	if (_settings != NULL)
	{
		if (_settings->ApSettings.Backup)
		{
			mode = WIFI_AP_STA;
		}

		if (wait4Mode(mode))
		{
			ok = WiFi.setHostname(_settings->StaSettings.Hostname.c_str());

			if (ok)
			{
				// Check if WiFi network SSID is defined and try to connect.
				if (_settings->StaSettings.SSID != "")
				{
					// Using DHCP for the connection.
					if (_settings->StaSettings.DHCP)
					{
						// attempt to connect to Wifi network:
						if (_settings->StaSettings.PASS != "")
						{
							WiFi.begin(_settings->StaSettings.SSID.c_str(), _settings->StaSettings.PASS.c_str());
						}
						else
						{
							WiFi.begin(_settings->StaSettings.SSID.c_str());
						}

						ok = wait4WiFi();
					}
					else
					{
						IPAddress address;
						IPAddress gateway;
						IPAddress subnet;
						IPAddress dns1;
						IPAddress dns2;

						bool addressOK = address.fromString(_settings->StaSettings.Address);
						bool gatewayOK = gateway.fromString(_settings->StaSettings.Gateway);
						bool subnetOK = subnet.fromString(_settings->StaSettings.Subnet);
						bool dns1OK = dns1.fromString(_settings->StaSettings.DNS1);
						bool dns2OK = dns2.fromString(_settings->StaSettings.DNS2);

						if (addressOK && gatewayOK && subnetOK)
						{
							if (dns1OK && dns2OK)
							{
								WiFi.config(address, gateway, subnet, dns1, dns2);
							}
							else if (dns1OK)
							{
								WiFi.config(address, gateway, subnet, dns1);
							}
							else
							{
								WiFi.config(address, gateway, subnet);
							}

							if (_settings->StaSettings.PASS != "")
							{
								WiFi.begin(_settings->StaSettings.SSID.c_str(), _settings->StaSettings.PASS.c_str());
							}
							else
							{
								WiFi.begin(_settings->StaSettings.SSID.c_str());
							}

							ok = wait4WiFi();
						}
					}
				}
			}
		}
	}

	// Init and get the time if connected to WiFi access point.
	if (ok)
	{
		configTime(3600, 0, "pool.ntp.org");
	}

	return ok;
}

/// <summary>
///  Disconnect WiFi.
/// </summary>
void WiFiManager::disconnect()
{
	Log.trace("WiFiManager::disconnect()" CR);
	WiFi.disconnect();
}

/// <summary>
/// Try to change the WiFi mode (MODE_AP, MODE_STA, MODE_AP_STA).
/// After every try wait for 50 msec.
/// </summary>
/// <param name="mode">The wifi mode</param>
/// <param name="count">The number of tries</param>
/// <returns>True if sucessful</returns>
bool WiFiManager::wait4Mode(wifi_mode_t mode, int count)
{
	WiFi.mode(mode);

	while ((WiFi.getMode() != mode) && (--count > 0))
	{
		delay(50);
	}

	return (WiFi.getMode() == mode);
}

/// <summary>
/// Try to connect to WiFi (returns true if the WiFi status == WL_CONNECTED).
/// After every try wait for 500 msec.
/// </summary>
/// <param name="count">The number of tries</param>
/// <returns>True if sucessful</returns>
bool WiFiManager::wait4WiFi(int count)
{
	//Wait for WiFi to connect
	while ((WiFi.status() != WL_CONNECTED) && (--count > 0))
	{
		delay(500);
	}

	return (WiFi.status() == WL_CONNECTED);
}