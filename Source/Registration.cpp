/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Registration.cpp,v $
Revision 1.1.10.17  2009/07/22 12:44:26  jay
Make ASCP freeware, so disable Registration code.

Revision 1.1.10.16  2007/07/26 08:44:34  jay
Increase trial period

Revision 1.1.10.15  2007/03/29 18:49:14  jay
Don't check unique ID

Revision 1.1.10.14  2007/03/28 22:58:11  Administrator
A bit more logging

Revision 1.1.10.13  2007/03/28 16:22:05  jay
Unique host ID: don't bother, as a mistake will upset registered users.

Revision 1.1.10.12  2007/03/27 21:17:57  jay
Turn off reg Logging

Revision 1.1.10.11  2007/03/27 16:59:25  jay
Sigh bug fix

Revision 1.1.10.10  2007/03/27 16:55:21  jay
Log reg. status to file to try to track down Windows reg. bug

Revision 1.1.10.9  2006/04/11 17:09:36  jay
New REG_VERSION; remove BOONTY ifdef but keep 1 hour trial period for now

Revision 1.1.10.8  2006/03/24 22:57:14  jay
Trial Period is now internal to Registration.

Revision 1.1.10.7  2006/03/23 08:37:17  jay
Fix for AMJU_FULL_VERSION

Revision 1.1.10.6  2006/03/09 23:18:16  jay
Replace BOONTY ifdef, as it could be more widely useful

Revision 1.1.10.5  2006/03/08 19:01:01  jay
Add BOONTY ifdef

Revision 1.1.10.4  2005/08/26 21:13:52  jay
Updated version no to Pool 1.2. GetStatus() is now available to check
 status of reg file.

Revision 1.1.10.3  2005/07/09 06:43:16  jay
Hide REG_VERSION so client code need not recompile on new version.

Revision 1.1.10.2  2005/07/05 09:46:24  jay
Turned off debug output. Different hash algo for POOL.

Revision 1.1.10.1  2005/03/31 22:27:44  jay
Temp hack to fix registration bug: adding a new app to the Applications folder
would un-register the game! Sheesh!

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include <fstream>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Registration.h"
#include "Directory.h"
#include "AmjuTime.h"
#include "LogFile.h"
#include "StringUtils.h"

#if defined(WIN32)
#include <windows.h>
#endif

// TODO TEMP TEST while we try to track down windows reg bug
//#define REG_LOG

// POOL ONLINE: longer trial period so we get more players online at a time 
static const int TRIAL_PERIOD = 3600 * 24 * 3;

using namespace std;
namespace Amju
{
namespace Registration
{
// Very simple registration code.

// This file is created the first time the application is run.
// When the software is registered the file gets a flag set.
const char* RegFileName()
{
  static std::string s;
  static bool first = true;
  if (first)
  {
    first = false;
    s = GetSaveDir();
    s += "reg.cfg";
  }

  return s.c_str();
}

#if defined(WIN32)
// We get the create time of this file to get a "unique ID" for this host.
static const char* ID_FILE = "c:\\config.sys";
#else
static const char* ID_FILE = "/Applications"; // Finder executable ?
#endif

// You must enter the registration code within 4 weeks of its creation.
static const unsigned int KEY_PERIOD = 60 * 60 * 24 * 7 * 4;

int GetUniqueHostId()
{
  // Don't run the risk of upsetting a registered user
  return 0;
}

bool Init()
{
  return true; // Reg code now does nothing

// BOONTY:  Full version can set registered flag.
// Game will then still be registered if upgraded to newer version
// in future. 
#ifdef AMJU_FULL_VERSION
  int mode = ios::out;
  std::ofstream outfile;
  outfile.open(RegFileName(), (std::_Ios_Openmode)mode);
  if (!outfile.is_open())
  {
    return false;
  }
  // Write unique ID for host, set flag for registered status.
  int id = GetUniqueHostId();
  outfile.write((const char*)&id, 4);
  char c = 0;
  outfile.write(&c, 1); 
  outfile.close();
  return true;

#else
  int version = REG_VERSION;
  struct stat buf;
  if (stat(RegFileName(), &buf) != -1)
  {
    // Reg file exists
    // If registered, do nothing.
    // If not registered, check the version number. If necessary we
    // update the version in the reg file, and give the user more
    // evaluation time.
    int fileVer = GetStatus();
    if (fileVer == 0)
    {
      // Registered.
#ifdef REG_DEBUG
      std::cout << "Init: file status is 0, i.e. registered.\n";
#endif
      return true;
    }
    if (version == fileVer)
    {
      // File is up-to-date with current version number.
      // No need to update the file.
#ifdef REG_DEBUG
      std::cout << "Init: file status is up-to-date.\n";
#endif
      return true;
    }
    if (version <= fileVer)
    {
#ifdef REG_DEBUG
      std::cout << "File version is greater than the version number given. Overwriting with earlier version number!\n";
#endif
    }

    if (fileVer == -1)
    {
#ifdef REG_DEBUG
      std::cout << "Init: problem with reg.cfg file.\n";
#endif
    }
#ifdef REG_DEBUG
    std::cout << "Init: Current version in reg.cfg is " << fileVer << " apparently.\n";
#endif
  }

  // Create reg. file if it doesn't exist.
  // TODO reg. file could contain an ID unique to this host, so it
  // can't be used on another machine.
  int mode = ios::out;
  std::ofstream outfile;
#if defined(MINGW) || defined(MACOSX)
  outfile.open(RegFileName(), (std::_Ios_Openmode)mode);
#else
  outfile.open(RegFileName(), mode);
#endif
  if (!outfile.is_open())
  {
    // Couldn't create file.
#ifdef REG_DEBUG
    std::cout << "Init: Couldn't create reg file " << RegFileName() << "\n";
#endif
    return false;
  }

  // Write unique ID for host, set flag for registered status.
  int id = GetUniqueHostId();
  outfile.write((const char*)&id, 4);

  char c = (int)version;
  outfile.write(&c, 1);
  outfile.close();

  // Verify that the new version was written to the file correctly.
  int newFileVer = GetStatus();
#ifdef REG_DEBUG
  std::cout << "Init: Verification: New reg.cfg file version: " << newFileVer << "\n";
#endif

  return true;
#endif // AMJU_FULL_VERSION
}

bool IsRegistered()
{
  return true; // Reg code now does nothing

  bool b = (GetStatus() == 0);

#ifdef REG_LOG
  // TODO TEMP TEST Find Win reg bug
  LogFile log;
  log.SetName("reg-bug-find-log.txt");
  std::string s = "Reg. IsRegistered(), b: ";
  s += b ? "true" : "false";
  log.Log(s);
#endif

  return b;
}

int GetStatus()
{
  return 0; // registered

#ifdef REG_LOG
  // TODO TEMP TEST Find Win reg bug
  LogFile log;
  log.SetName("reg-bug-find-log.txt");
  std::string s = "Reg. GetStatus(), file name: ";
  s += RegFileName();
  log.Log(s);
#endif

  // Get status character from reg file.
  int mode = ios::in;
  std::ifstream infile;
#if defined(MINGW) || defined(MACOSX)
  infile.open(RegFileName(), (std::_Ios_Openmode)mode);
#else
  infile.open(RegFileName(), mode);
#endif
  if (!infile.is_open())
  {
#ifdef REG_LOG
    log.Log("Could not open reg file. Returning -1.");
#endif

#ifdef REG_DEBUG
    std::cout << "Could not open file " << RegFileName() << "\n";
#endif
    return -1; 
  }
  // Read data from file.
  char buf[6];
  infile.read(buf, 5);
  // Data should match unique ID for host.
  int id = *(int*)&buf;

#ifdef REG_LOG
  s = "Read this data from file: ";
  for (int i = 0; i < 5; i++)
  {
    s += ToString((int)buf[i]);
    s += " ";
  }
  s += " Unique ID (file): ";
  s += ToString(id);
  s += " GetUniqueHostId() returns: ";
  s += ToString(GetUniqueHostId());
  log.Log(s);
#endif

#ifdef REG_DEBUG
  std::cout << "Reg file unique ID: " << id << "\n";
#endif

  // DO NOT check unique host ID.. risk of error, and upset customers

  char c = buf[4];
 
#ifdef REG_LOG
  s = "Returning this value: ";
  s += ToString((int)c);
  log.Log(s); 
#endif

  return (int)c;
}

int GetRemainingEvalPeriod()
{
  return TRIAL_PERIOD; // reg code does nothing

  uint32 periodSecs = TRIAL_PERIOD;

  // Get the create time of the reg. file.
  struct stat buf;
  if (stat(RegFileName(), &buf) == -1)
  {
    // Error
#ifdef REG_DEBUG
    std::cout << "Can't get create time for reg file.\n";
#endif
    return 0;
  }
  uint32 creation = buf.st_ctime; // file create time
  time_t now;
  time(&now);
#ifdef REG_DEBUG
  std::cout << "Create time of reg.cfg file: " 
    << Amju::Time(creation).ToString().c_str() 
    << " now: " << Amju::Time(now).ToString().c_str() << "\n";
#endif
  uint32 rem = now - creation;
  if (rem > periodSecs)
  {
#ifdef REG_DEBUG
    std::cout << "Eval period has expired (" 
        << rem << " vs " << periodSecs << " secs)\n";
#endif
    return 0;
  } 
  return periodSecs - rem;
}

void Swap(unsigned int& k)
{
  unsigned int r = 0;
  r |= ((k & 0x80000000) >> 31);  // 31 -> 0
  r |= ((k & 0x40000000) >> 0);   // 30 -> 30
  r |= ((k & 0x20000000) >> 27);  // 29 -> 2
  r |= ((k & 0x10000000) >> 0);   // 28 -> 28

  r |= ((k & 0x08000000) >> 23);  // 27 -> 4
  r |= ((k & 0x04000000) >> 0);   // 26
  r |= ((k & 0x02000000) >> 19);  // 25 -> 6
  r |= ((k & 0x01000000) >> 0);   // 24

  r |= ((k & 0x00800000) >> 15);  // 23 -> 8
  r |= ((k & 0x00400000) >> 0);   // 22
  r |= ((k & 0x00200000) >> 11);  // 21 -> 10
  r |= ((k & 0x00100000) >> 0);   // 20

  r |= ((k & 0x00080000) >> 7);   // 19 -> 12
  r |= ((k & 0x00040000) >> 0);   // 18
  r |= ((k & 0x00020000) >> 3);   // 17 -> 14
  r |= ((k & 0x00010000) >> 0);   // 16

  r |= ((k & 0x00008000) >> 0);   // 15 
  r |= ((k & 0x00004000) << 3);   // 14 -> 17
  r |= ((k & 0x00002000) >> 0);   // 13 
  r |= ((k & 0x00001000) << 7);   // 12 -> 19

  r |= ((k & 0x00000800) >> 0);   // 11 
  r |= ((k & 0x00000400) << 11);  // 10 -> 21
  r |= ((k & 0x00000200) >> 0);   // 09 
  r |= ((k & 0x00000100) << 15);  // 08 -> 23

  r |= ((k & 0x00000080) >> 0);   // 07 
  r |= ((k & 0x00000040) << 19);  // 06 -> 25
  r |= ((k & 0x00000020) >> 0);   // 05 
  r |= ((k & 0x00000010) << 23);  // 04 -> 27

  r |= ((k & 0x00000008) >> 0);   // 03 
  r |= ((k & 0x00000004) << 27);  // 02 -> 29
  r |= ((k & 0x00000002) >> 0);   // 01 
  r |= ((k & 0x00000001) << 31);  // 00 -> 31

  r ^= ((r & 0xff000000) >> 8);
  r ^= ((r & 0x000000ff) << 8);

  k = r;
}

void Twiddle(unsigned int& k)
{
//  k >>= 8;
//  k |= 0x63000000; // some random number, which is checked in Untwiddle.
  Swap(k);
}

void Untwiddle(unsigned int& k)
{
  Swap(k);
//  if ((k & 0xff000000) != 0x63000000)
//  {
//    k = 0;
//  }
//  k <<= 8;
}

bool IsRegKeyValid(const std::string& key)
{
  return true;

  // To be valid, the key must have been generated within the last KEY_PERIOD.
  // Get the creation date of the key.

  // NB This algorithm must correspond to the one in
  // cgi-bin/keygen-pool.pl.
  // The encoding algo is:
  //  - get the current timestamp
  //  - add the period for which the reg key is valid
  //  - chop lower 16 bits
  //  - shift >> 16
  //  - bits 16-24 are XOR checksum of bottom 16 bits.
  //  - checksum ^= 0x55

  // The decoding algo is:
  //  - get bits 16-24, the checksum
  //  - checksum ^= 0x55
  //  - shift up bits 0-16.
  //  - verify checksum
  //  - timestamp should be less than current time to be valid.

  unsigned int k = atoi(key.c_str());

  unsigned int checksum = (k & 0x00ff0000) >> 16;
  checksum ^= 0x55;

  unsigned int t = (k & 0x0000ffff) << 16;
  // check checksum
  unsigned int myxor = (k & 0x000000ff) ^ ((k & 0x0000ff00) >> 8);
  if (checksum != myxor)
  {
#ifdef _DEBUG
std::cout << "Bad checksum!\n";
#endif
    return false;
  } 

  // Is the creation date within KEY_PERIOD ?
  time_t now;
  time(&now);

  if (now > t)
  {
    return false;
  }

  return true; 
}

void SetRegistered(bool registered)
{
  int mode = ios::out | ios::trunc;
  std::ofstream outfile;
#if defined(MINGW) || defined(MACOSX)
  outfile.open(RegFileName(), (std::_Ios_Openmode)mode);
#else
  outfile.open(RegFileName(), mode);
#endif
  if (!outfile.is_open())
  {
    // Couldn't create file.
    return;
  }
  // Write unique ID for host, set flag for registered status.
  int id = GetUniqueHostId();
  outfile.write((const char*)&id, 4);
  char c = 0;
  if (!registered)
  {
    c = (char)REG_VERSION;
  }
  outfile.write(&c, 1);
  outfile.close();
}

std::string MakeRegKey()
{
  return std::string();

  time_t now;
  time(&now);

  unsigned int k = now;

  Twiddle(k);

  char buf[100];
  sprintf(buf, "%x", k);

  return buf;
}

}
}
