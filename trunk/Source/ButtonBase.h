/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ButtonBase.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(BUTTON_BASE_H_INCLUDED)
#define BUTTON_BASE_H_INCLUDED

#include <vector>
#include "SolidGameObject.h"
#include "MessageList.h"
#include "ExplosionEffect.h"

namespace Amju
{
// Buttons are objects which change their appearance and send messages
// when they are triggered. They could be triggered by the player
// colliding, an object colliding, etc.
class ButtonBase : public SolidGameObject
{
public:
  ButtonBase();

  virtual const HeightServer* GetCollisionVol() const { return 0; }

  // Set up height from ground and reset shadow
  virtual void OnRoomEntry();

protected:
  virtual void RecalcCollisionVol() {}

  bool LoadMessageLists(File*);

#ifdef SCENE_EDITOR
  bool SaveMessageLists(File*);
#endif

protected:
  typedef std::vector<MessageList> MessageLists;
  MessageLists m_lists;

  // Current state: index into message list vector.
  int m_buttonState;

  // Explosion for when button is triggered.
  // NB You should override CullDraw() to draw this when the button itself
  // is culled.
  SharedPtr<ExplosionEffect> m_pExplosion;
};
}

#endif



