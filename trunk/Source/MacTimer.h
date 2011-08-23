/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacTimer.h,v $
Revision 1.1.10.1  2006/08/07 20:44:43  jay
Added fixed-interval option to MacTimer

Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if !defined(MAC_TIMER_H_INCLUDED)
#define MAC_TIMER_H_INCLUDED

namespace Amju
{
class MacTimer 
{
public:
  MacTimer();

  double GetElapsedTime();

  // Set fixed interval, as an attempt to reduce flicker.
  void SetInterval(double fixedFrameTime);

private:
  double m_interval;
  bool m_fixed;
};
}

#endif
