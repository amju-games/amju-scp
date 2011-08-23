/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateHiScore.h,v $
Revision 1.1.2.2  2006/08/14 17:50:11  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:31  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_HI_SCORE_H_INCLUDED)
#define ENGINE_STATE_HI_SCORE_H_INCLUDED

#include "EngineStateEnterText.h"

namespace Amju
{
class EngineStateHiScore : public EngineStateEnterText
{
public:
  static const char* Name;
  
  EngineStateHiScore();
  virtual void SetActive(bool);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void OnKey(char key, bool down);
  virtual bool Load();
  // Handle the problem that the space bar cannot normally be used
  // in name.
  virtual void Red(bool b);
 
protected:
  virtual void Finished();
  virtual void Back();
  virtual void TimerExpired();

  // Set up greet text for the next player to enter his high score.
  // If there is no next high score, returns false.
  bool FindNextHiScore();
 
protected:
  std::string m_playerName;

  PSolidComponent m_pGreetText;

  // For multiplayer, we must keep track of which player is currently entering 
  // their name.
  int m_currentPlayer;
};
}

#endif


