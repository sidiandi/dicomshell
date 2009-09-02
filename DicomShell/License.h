#pragma once

class License
{
public:
	time_t expirationDate();
	bool hasExpirationDate();
	bool isLicenseValid();
	bool isRegistered();
};
