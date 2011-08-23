/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Button.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BUTTON_H_INCLUDED)
#define SCHMICKEN_BUTTON_H_INCLUDED

#include "ButtonBase.h"
//#include "SolidGameObject.h"
#include "Message.h"
#include "MessageList.h"
#include "Orientation.h"

namespace Amju
{
// Button: a Game Object which responds to being hit by an object or the player.
// When it is hit ('set'), it sends messages. These can included a timed message
// to itself to unset, so we can have timer buttons.
// When set, the position of the button changes from its unset position to 
// its set position at some velocity.
class Button : public ButtonBase
{
public:
  Button();

  virtual bool Load(File*);
  virtual const char* GetTypeName() const;
    
  virtual void DrawShadow() const;
  virtual void Update();
  virtual void OnRoomEntry();
  virtual void SetState(State newState);
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  // TODO handle object collision ?

  void Set(bool);

protected:
  // Position of button when set
  Orientation m_setPosition;
  // Position of button when unset
  Orientation m_unsetPosition;
  // TODO velocity when switching positions ?

  // Messages to fire when button is set.
  MessageList m_setMessages;

  // Messages to fire when button is unset
  // Currently no messages.
  //MessageList m_setMessages;

  // true when moving to/from set to unset position
  bool m_isMoving;
  // When moving, varies from 0 (not moved yet) to 1.0 (finished moving)
  float m_interp;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

  MessageList* GetSetMessages() { return &m_setMessages; }

#endif
};
}
#endif

