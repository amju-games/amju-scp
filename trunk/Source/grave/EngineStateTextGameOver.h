/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateTextGameOver.h,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_TEXT_GAME_OVER_H_INCLUDED)
#define ENGINE_STATE_TEXT_GAME_OVER_H_INCLUDED

#include "EngineStateText.h"

namespace Amju
{
class EngineStateTextGameOver : public EngineStateText
{
public:
  // ID of this class, which we register with the Engine
  static const char* Name;

  EngineStateTextGameOver();
  virtual bool Load();
  virtual void Red(bool);
  virtual void TimerExpired();
  virtual void Draw();
};
}

#endif

