/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolScoreboard.h,v $
Revision 1.1.2.1  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SCOREBOARD_H_INCLUDED)
#define ENGINE_STATE_GOLF_SCOREBOARD_H_INCLUDED

#include "EngineStatePoolFlyby.h"
#include "GameState.h"

namespace Amju
{
class EngineStatePoolScoreboard : public EngineStatePoolFlyby
{
public:
  static const char* Name;
  virtual void DrawOverlays();
  virtual void SetActive(bool active);
  virtual void Update();
  virtual void Red(bool);
  virtual bool Load();
 
protected:
  void CreateText(GameState::PlayerInfo* pPi, int rank);
  void DrawLine(int i);
  int GetAllBonusesInPlay();
  void UnlockNextCourse();

protected:
  PSolidComponent m_ranks[4];
  PSolidComponent m_names[4];
  PSolidComponent m_scores[4];
  // Positions of columns
  float m_rankPos, m_namePos, m_scorePos;
};
}

#endif

