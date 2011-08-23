/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolBallInHole.h,v $
Revision 1.1.2.1  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.4.1.2.2  2005/09/16 20:04:50  jay
Remove unused code

Revision 1.1.4.1.2.1  2005/05/24 20:37:33  jay
Show player stats

Revision 1.1.4.1  2004/11/09 16:05:00  jay
Bonus points now depend on how many strokes it took to get the ball in the hole.

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_BALL_IN_HOLE_H_INCLUDED)
#define ENGINE_STATE_GOLF_BALL_IN_HOLE_H_INCLUDED

#include "EngineStatePoolText.h"

namespace Amju
{
class PlayerStats;

class EngineStatePoolBallInHole : public EngineStatePoolText
{
public:
  static const char* Name;

  EngineStatePoolBallInHole();

  virtual void Draw();
  virtual void DrawOverlays();
  virtual void SetActive(bool active);
  virtual void Update();
  virtual bool Load();
  virtual void MouseButton(bool, bool, bool);
  virtual void OnNewGame();

protected:
  virtual void TimerExpired();
  void DrawStats(PlayerStats*, float row);

protected:
  // True if timer has expired
  bool m_expired;
/*
  enum { NUM_EXPLOSIONS = 5 };

  ExplosionEffect m_explosion[NUM_EXPLOSIONS];

  float explosionTimer;
  int m_currentExplosion;

  PSolidComponent m_pBonusText;
  float m_maxBonusTime;
  // Number of bonuses given to player since this
  // state was made active
  int m_numBonuses;

  // Store set of all holes which have ben completed THIS GAME.
  // This stops you from playing the same hole over and over getting a 
  // big score. You only get the points the first time you finish the
  // hole.
  // Set of <level, room> pairs.
  // v.1.2: for multi players, we must also hold the player ID along with
  // the level, room pair.
  // So we now have a <player ID, level/room pair> pair.
  //typedef std::set< std::pair<int, int> > HoleSet;
  typedef std::set< std::pair<int, std::pair<int, int> > > HoleSet;
  HoleSet m_holesFinished;

  // Points to award to the winning player, in hundreds.
  int m_bonusPointsHundreds;

  PGuiElement m_pOkButton;
*/
};
}

#endif

