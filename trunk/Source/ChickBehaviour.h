/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickBehaviour.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CHICK_BEHAVIOUR_H_INCLUDED)
#define CHICK_BEHAVIOUR_H_INCLUDED

#include "Behaviour.h"

namespace Amju
{
class Chick;

// This is the default behaviour for Chicks. Try to follow the player, and
// keep a minimum distance between other chicks.
class ChickBehaviour : public Behaviour
{
public:
  ChickBehaviour();

  static void Init();

  virtual void Update();

protected:
  void KeepMinDistance(Chick* pChick);

protected:
  // Timer, which must reach m_waitBetweenUpdates before we do an update.
  float m_updateTime;
  // This is copied from s_waitBetweenUpdates, but may then be tuned depending
  // on how many chicks there are, etc.
  float m_waitBetweenUpdates;

  static float s_far;
  static float s_near;
  static float s_farRotVel;
  static float s_farVel;
  static float s_nearVel;
  static float s_minDist;
  // Time to wait between updates. 
  static float s_waitBetweenUpdates;

};
}

#endif

