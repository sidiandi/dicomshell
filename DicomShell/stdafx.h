// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define _ATL_DEBUG_QI
#define _ATL_DEBUG_INTERFACES 

#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimgle/didocu.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcpixel.h>     /* for class DcmPixelData */
#include "dcmtk/dcmimage/dipalimg.h"
#include "dcmtk/dcmimage/dirgbimg.h"
#include "dcmtk/dcmimage/dihsvimg.h"
#include "dcmtk/dcmimage/diargimg.h"
#include "dcmtk/dcmimage/dicmyimg.h"
#include "dcmtk/dcmimage/diybrimg.h"
#include "dcmtk/dcmimage/diyf2img.h"
#include "dcmtk/dcmimage/diyp2img.h"
#include "dcmtk/dcmimage/dicomot.h"
#include "dcmtk/dcmimgle/didocu.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcrledrg.h"      /* for DcmRLEDecoderRegistration */

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <shlguid.h> 
#include <ShlObj.h>
#include <comdef.h>
#include <filter.h>
#include <filterr.h>

#include <vector>

using namespace ATL;