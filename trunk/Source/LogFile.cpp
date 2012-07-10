/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LogFile.cpp,v $
Revision 1.1.10.2  2007/03/10 23:12:20  jay
Log file: make time optional, for diffing files

Revision 1.1.10.1  2007/03/10 21:53:01  jay
Don't change File::Root, no need and can accumulate slashes

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include <iostream>
#include <time.h>
#include "LogFile.h"
#include "File.h"
#include "Directory.h"
#include "StringUtils.h"

namespace Amju
{
void LogFile::SetName(const std::string& s)
{
  m_filename = s;
}

std::string LogFile::GetName() const
{
  return m_filename;
}

void LogFile::Log(const std::string& s, bool prependTime)
{
  File f(false, File::STD); // no version info, no Glue.
  f.OpenWrite(
    GetDesktopDir() + "/" + m_filename, 
    File::CURRENT_VERSION, 
    false, // => not binary 
    false,  // => don't use root
    false); // false => don't truncate

  if (prependTime)
  {
#ifdef WIN32
    // Get time/date 
    // TODO totally not thread safe - use _r versions instead
    time_t now;
    time(&now);
    tm myTm;
    localtime_s(&myTm, &now);
    static const int BUF_SIZE = 100;
    char buf[BUF_SIZE];
    asctime_s(buf, BUF_SIZE, &myTm);
    std::string t = buf;
    t = Replace(t, "\n", "");
    t += ": ";
    t += s; 
    f.Write(t);
#else
    f.Write(s);
#endif
  }
  else
  {
    f.Write(s);
  }
}
}

