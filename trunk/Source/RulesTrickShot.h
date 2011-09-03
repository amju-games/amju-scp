/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesTrickShot.h,v $
Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.5  2005/09/23 19:36:42  jay
Override BallHitsObject so hitting anything counts as no foul; use PoolChangeState

Revision 1.1.2.4  2005/09/20 13:18:56  jay
Custom behaviour

Revision 1.1.2.3  2005/09/17 21:16:13  jay
Add Scene Ed Properties

Revision 1.1.2.2  2005/09/17 16:18:11  jay
Fixes to get Trick Shots to work

Revision 1.1.2.1  2005/09/16 20:04:19  jay
Add Trick Shot rules

*/

#ifndef RULES_TRICK_SHOT_H_INCLUDED
#define RULES_TRICK_SHOT_H_INCLUDED

#include "Rules.h"
#include "PoolBehaviour.h"

namespace Amju
{
// This Rules subclass is for Trick shots - one player games where you must
// pot all the balls in a certain number of shots.
class RulesTrickShot : public Rules
{
public:
  RulesTrickShot();

  virtual Rules* Clone();
  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void OnRoomEntry();
  virtual void Reset();
  virtual void BallsHaveStopped();
  virtual void BallHitsObject(PoolBall* pBall, PoolGameObject* pObj);

protected:
  virtual void GivePlayerFoulAward();
  virtual void ThisPlayerWins();
  virtual void ThisPlayerLoses();

  void TrickShotOver();

protected:
  // No of shots allowed to complete the Trick Shot
  int m_maxNumShots;

  // List of (non-ball) objects hit this shot.
  // I.e. things like animals, blocks, etc.
  // For trick shots, a shot is only a foul if you don't hit _anything_ 
  // -- hitting a non-ball object counts ok.
  std::vector<PoolGameObject*> m_objectsHitThisShot;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif
};

class PoolBehaviourTrickShot : public PoolBehaviour
{
public:
  virtual TexturedQuad* GetHudBall() { return 0; }
  virtual ValidBalls GetValidBalls();

};
}

#endif


