/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateObjectMenu.h,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_OBJECT_MENU_H_INCLUDED)
#define ENGINE_STATE_OBJECT_MENU_H_INCLUDED

#include <vector>
#include "EngineMenuState.h"
#include "ObjectMenuItem.h"

namespace Amju
{
class EngineStateObjectMenu : public EngineMenuState
{
public:
  static const char* Name;
 
  virtual void SetActive(bool active);
  virtual void Red(bool);
  virtual bool Load();
  virtual void Draw(); 
  virtual void DrawOverlays();

  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  void AddItem(ObjectMenuItem*);

protected:
  typedef std::vector<SharedPtr<ObjectMenuItem> > ObjectItemVec;
  ObjectItemVec m_objectItems;

  // Second menu, for text items cancel and select.
  //PMenu m_pMenu2;
};
}

#endif



