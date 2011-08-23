/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Chick.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CHICK_H_INCLUDED)
#define CHICK_H_INCLUDED

#include "Npc.h"
#include "ChickBehaviour.h"
#include "ChickShedBehaviour.h"
#include "ChickCapturedBehaviour.h"

namespace Amju
{
class Shed;

// Chicks are friendly NPCs.
// Currently the plan is they will flock and follow the Player.
class Chick : public Npc
{
public:
  Chick();

  virtual void Draw();
  virtual void OnRoomEntry();
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(GameObject* pObj);
  virtual void SetState(State s);
  virtual void Update();
  virtual void DrawShadow() const; // don't draw shadow when captured

  // Simplified version of CharacterGameObject's implementation. This is
  // so we can lots of chicks without slowing things down so much, but means
  // that chicks can't collide with Game Objects, just the static scene.
  virtual void RefreshHeightServer();

  // Set the ID of the egg this chick hatched out of.
  void SetEgg(int eggId) { m_eggId = eggId; }

  // Call when this chick is safe, so player wins this Chick
  void Win();

  // Call when this chick is killed, so player loses this chick.
  void Lose();

  // Decide which behaviour to follow.
  void Decide();

protected:
  // Set up when hatched from egg.
  void Hatch();

  // Find closest shed.
  PGameObject FindShed();

protected:
  // Different kinds of behaviour for a Chick.
  ChickBehaviour m_normalBehaviour;
  ChickShedBehaviour m_shedBehaviour;
  ChickCapturedBehaviour m_capturedBehaviour;

  // The ID of the egg from which this Chick hatched.
  int m_eggId;
};
}

#endif
