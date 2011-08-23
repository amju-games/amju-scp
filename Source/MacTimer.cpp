/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacTimer.cpp,v $
Revision 1.1.10.1  2006/08/07 20:44:43  jay
Added fixed-interval option to MacTimer

Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#include <iostream>
#include <time.h>
#include "MacTimer.h"
//#include <Carbon/Carbon.h>
#include <Glut/Glut.h>

namespace Amju
{
MacTimer::MacTimer() : m_interval(0), m_fixed(false)
{
}

void MacTimer::SetInterval(double interval)
{
  m_interval = interval;
  m_fixed = true;
}

double MacTimer::GetElapsedTime()
{
  // GLUT version
  int millisecs = glutGet(GLUT_ELAPSED_TIME);
  double d = double(millisecs) / 1024;

  static double lastd = d;

  double elapsed = d - lastd;
  lastd = d;

  if (!m_fixed)
  {
//std::cout << "MacTimer: returning interval: " << d << "\n";
    return d;
  }

  if (elapsed < m_interval)
  {
    double sleepSecs = m_interval - elapsed;

//std::cout << "MacTimer: sleeping for " << sleepSecs << "s\n..";

    timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (long int)(sleepSecs * 1000000000.0);
    nanosleep (&ts, NULL);
  }  

//std::cout << "MacTimer: returning interval: " << m_interval << "\n";
  static double acc = 0;
  acc += m_interval;
  return acc;
}
}
