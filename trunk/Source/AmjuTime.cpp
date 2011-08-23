/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: AmjuTime.cpp,v $
Revision 1.1.2.2  2005/08/31 20:06:42  Administrator
Fix for WIN32

Revision 1.1.2.1  2005/08/26 21:14:52  jay
Added Time classes

Revision 1.3  2005/02/28 20:34:22  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.2  2005/01/19 10:19:58  jay
Added MakeTime() to construct a time from days, months, etc.

Revision 1.1  2005/01/09 12:34:30  jay
New Time class - but calling the file Time.h causes problems.

*/

#include <iostream>
#include <time.h>
#include "AmjuTime.h"
#include "StringUtils.h"
#include "TimePeriod.h"

namespace Amju
{
Time Time::Now()
{
  time_t now;
  time(&now);
  return Time((unsigned int)now); 
}

Time Time::MakeTime(int secs, int mins, int hours, int days, int months, int years)
{
  // TODO error checking

  tm myTm;
  myTm.tm_sec = secs;
  myTm.tm_min = mins;
  myTm.tm_hour = hours;
  myTm.tm_mday = days; // 1 - 31 incl.
  myTm.tm_mon = months - 1; // convert to 0 - 11 incl.
  myTm.tm_year = years - 1900;
  // Ignore tm_wday and tm_yday
  myTm.tm_isdst = -1; // try to work out summer time

  time_t t = mktime(&myTm);
  if (t == (time_t)-1)
  {
std::cout << "Failed to create time.\n";
  }
  else 
  {
std::cout << "Successfully generated time: " << Time((unsigned int)t).ToString().c_str() << "\n";
  }
  return Time((unsigned int)t);
}

Time::Time(unsigned int secs) : m_secs(secs)
{
}

std::string Time::ToString() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  std::string t = asctime(&mytm);
  t = Replace(t, "\n", "");
  return t;
}

int Time::GetSecs() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  return mytm.tm_sec;
}

int Time::GetMins() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  return mytm.tm_min;
}

int Time::GetHours() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  return mytm.tm_hour;
}

int Time::GetDays() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  return mytm.tm_mday;
}

int Time::GetMonths() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  return mytm.tm_mon + 1;
}

int Time::GetYears() const
{
  tm mytm;
#ifdef MACOSX
  localtime_r((const time_t*)&m_secs, &mytm);
#else
  mytm = *(localtime((const time_t*)&m_secs));
#endif
  return mytm.tm_year + 1900;
}

unsigned int Time::ToSeconds() const
{
  return m_secs;
}

Time& Time::operator+=(const TimePeriod& tp)
{
  m_secs += tp.m_secs;
  return *this;
}

Time& Time::operator-=(const TimePeriod& tp)
{
  m_secs -= tp.m_secs;
  return *this;
}

TimePeriod Time::operator-(const Time& rhs) const
{
  unsigned int diff = 0;
  if (m_secs > rhs.m_secs)
  {
    diff = m_secs - rhs.m_secs;
  }
  else
  {
    diff = rhs.m_secs - m_secs;
  }
  return TimePeriod(diff);
}

bool Time::operator==(const Time& rhs) const
{
  return (m_secs == rhs.m_secs);
}

Time& Time::RoundUp(const TimePeriod& tp)
{
//std::cout << "TIME rounding UP " << ToString().c_str() << " to ";

  m_secs = tp.m_secs * ((m_secs + tp.m_secs) / tp.m_secs);

//std::cout << ToString().c_str() << "\n";
  return *this; 
}

Time& Time::RoundDown(const TimePeriod& tp)
{
//std::cout << "TIME rounding DOWN " << ToString().c_str() << " to ";

  m_secs = tp.m_secs * (m_secs / tp.m_secs);

//std::cout << ToString().c_str() << "\n";

  return *this; 
}
}


