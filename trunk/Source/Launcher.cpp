/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Launcher.cpp,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include "Launcher.h"
#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif
#ifdef MACOSX
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFUrl.h>
#include <Carbon/Carbon.h>
#endif

namespace Amju
{
bool LaunchBrowser(const char* url)
{
#ifdef WIN32
  int r =  (int)ShellExecute(0, "open", url, "", "", 1);
  return (r > 32);

#else 

#ifdef MACOSX

  CFStringRef strurl =                //CFSTR("http://www.amju.com"); // TODO HACK 
    CFStringCreateWithBytes(
    0, 
    (const unsigned char*)url, 
    strlen(url),
    kCFStringEncodingMacRoman,
    false);

  CFURLRef cfurl = CFURLCreateWithString(0, strurl, 0);
  OSStatus ret = LSOpenCFURLRef(cfurl, 0);

std::cout << "Launch URL: " << CFStringGetCStringPtr(strurl, kCFStringEncodingMacRoman) << "\n";
std::cout << "Launch result: " << ret << "\n";

  // TODO deal with ret
  return true;

#else
  // Not implemented for this platform
  return false;
#endif
#endif

}
}

