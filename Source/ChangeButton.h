/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChangeButton.h,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if !defined(CHANGE_BUTTON_H_INCLUDED)
#define CHANGE_BUTTON_H_INCLUDED 

#include "ButtonBase.h"
#include "SolidComponent.h"
#include "MessageList.h"

namespace Amju
{
// This kind of button changes the solid drawn depending on the state.
class ChangeButton : public ButtonBase
{
public:
  ChangeButton();  
  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Draw();
  virtual void CullDraw(Frustum*); // to draw explosion
  virtual void DrawShadow() const;
  virtual void Update();
  virtual void OnRoomEntry();
  virtual void SetState(State newState);
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(GameObject*);
  virtual void SetOrientation(const Orientation&);

protected:
/*
  bool SaveMessageLists(File*);
  bool LoadMessageLists(File*);
*/
 
protected:
  std::vector<PSolidComponent> m_solids;
  bool m_isColliding;
/*
  std::vector<MessageList>  m_messageLists;
*/ 

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

};
}

#endif

