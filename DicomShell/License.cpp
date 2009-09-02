#include "StdAfx.h"
#include "License.h"
#include <time.h>

time_t dateToTime(const char* date)
{
	char month[5];
	struct tm t;
	ZeroMemory(&t, sizeof(t));
	sscanf_s(date, "%s %d %d", month, 4, &t.tm_mday, &t.tm_year);
	t.tm_year -= 1900;
	
	char* mon_name[12] =
	{
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

	for (t.tm_mon; t.tm_mon < 12 && 0 != strcmp(mon_name[t.tm_mon], month); ++t.tm_mon)
	{
	}

	return mktime(&t);
}

time_t License::expirationDate()
{
	return dateToTime(__DATE__) + 86400 * 30;
}

bool License::hasExpirationDate()
{
	return false;
}

bool License::isRegistered()
{
	return true;
}

static volatile LONG registerReminderShown = -1;

bool License::isLicenseValid()
{
	if (License::isRegistered())
	{
		return true;
	}

	if (InterlockedIncrement(&registerReminderShown) % 1000 == 0)
	{
		int result = MessageBox(NULL, TEXT("You are using an unregistered copy of DicomShell. Would you like to go to\r\n\r\nhttp://andreas-grimme.gmxhome.de/dicomshell.html\r\n\r\nto register now?"), TEXT("DicomShell"), MB_SETFOREGROUND | MB_SYSTEMMODAL | MB_YESNO);
		switch (result)
		{
		case IDYES:
			ShellExecute(NULL, TEXT("open"), TEXT("http://andreas-grimme.gmxhome.de/dicomshell.html"), NULL, NULL, 0);
			break;
		default:
			break;
		}
	}
	
	if (hasExpirationDate())
	{
		__time64_t n;
		_time64(&n);
		return n < expirationDate();
	}
	else
	{
		return true;
	}
}
