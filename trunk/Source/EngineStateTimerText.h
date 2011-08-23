/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTimerText.h,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_PLAYER_UP_H_INCLUDED)
#define ENGINE_STATE_PLAYER_UP_H_INCLUDED

#include "EngineStateText.h"

namespace Amju
{
class EngineStateTimerText : public EngineStateText
{
public:
  static const char* Name;

  EngineStateTimerText();

  // Set up text based on current Game State.
  virtual void SetActive(bool active);
  virtual void TimerExpired();
  virtual bool Load();
};
}

#endif
