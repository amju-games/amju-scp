/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Stupid.h,v $
Revision 1.1.10.2  2006/04/13 18:17:40  jay
Override RefreshHeightServer to ignore NonHeadAreas; update ball pos in
Update(), not Draw()

Revision 1.1.10.1  2005/09/30 18:09:57  jay
Override Update() so we correctly control Stupids. The version in
CharacterGameObject is hacked for POOL, but if we change it we change
the behaviour of the player characters :-(

Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if !defined(STUPID_H_INCLUDED)
#define STUPID_H_INCLUDED

#include "Npc.h"
#include "ExplosionEffect.h"

namespace Amju
{
// This kind of Npc blindly walks in a straight line until it hits something,
// whereupon it changes direction.
class Stupid : public Npc
{
public:
  Stupid();

  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Update();
  virtual void OnRoomEntry();
  virtual void HandleObjectCollision(GameObject* pObj);
  virtual void SetState(State newState);
  virtual void CullDraw(Frustum*);
  virtual void Draw();
  virtual void RefreshHeightServer();  

  float GetTurnDirection() const;
  void SetTurnDirection(float d);

  float GetInitialDirection() const;
  void SetInitialDirection(float d);

  void SetAppearanceDelay(float delay);

protected:
  float m_initialDir;
  float m_turnDir;
  float m_appearanceDelay;

  ExplosionEffect m_explosion;

  // ID of object which appears when this stupid is hit.
  int m_hiddenId;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif


};
}

#endif

