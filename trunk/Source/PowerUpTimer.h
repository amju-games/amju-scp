/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerUpTimer.h,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if !defined(POWER_UP_TIMER_H_INCLUDED)
#define POWER_UP_TIMER_H_INCLUDED

#include "Guage.h"

namespace Amju
{
// Shows time remaining before a Power Up goes away.
// (Could also show other time-based things).
// Draw() shows the timer as long as the time set in SetTime()
// has not expired. I.e. if you call SetTime(10) then the 
// timer will be shown for the next 10 secs.
// SetMaxTime is required so we know how much of the guage to fill.
// Maybe this should be a Mixin class, so different kinds of guages can 
// be timers.
class PowerUpTimer : public Guage
{
public:
  virtual bool Load(File*);
  virtual void Draw();

  void SetTime(float secs);
  void SetMaxTime(float secs);

protected:
  float m_timeRemaining;
  float m_maxTime;
};
}

#endif
