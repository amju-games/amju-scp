/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateNumPlayers.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_NUM_PLAYERS_H_INCLUDED)
#define ENGINE_STATE_NUM_PLAYERS_H_INCLUDED

#include "EngineMenuState.h"

namespace Amju
{
class EngineStateNumPlayers : public EngineMenuState
{
public:
  static const char* Name;

  virtual bool Load();
  virtual void Red(bool down);
  virtual void SetActive(bool active);

  // Set max/min numer of players, inclusive.
  void SetMax(int);
  void SetMin(int);

protected:
  int m_min, m_max;
};
}

#endif

