/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LaunchPad.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_LAUNCH_PAD_H_INCLUDED)
#define SCHMICKEN_LAUNCH_PAD_H_INCLUDED

#include "Target.h"

namespace Amju
{
// Launchpad is a bounding sphere (Target) with a fun property:
// any Game Object (including the player) which enters the bounding sphere
// gets thrown up in the air! Wheee!
class LaunchPad : public Target
{
public:
  LaunchPad();
  virtual const char* GetTypeName() const;
  virtual bool Load(File* pf);
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(GameObject* pObj);

protected:
  // Upward velocity for launched things.
  float m_upVel;

  // Time to remain in OUT_OF_PLAY state after launchpad triggered.
  // This should be _longer_ than the time taken for a launched object
  // to fall back to ground level, (which depends on m_upVel and gravity).
  float m_restoreTime;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif

