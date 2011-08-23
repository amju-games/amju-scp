/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiComposite.h,v $
Revision 1.1.10.2  2007/03/09 21:48:55  jay
Copy latest GUI library from trunk

Revision 1.2  2007/01/14 21:33:47  jay
Add more overrides

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_COMPOSITE_H_INCLUDED)
#define AMJU_GUI_COMPOSITE_H_INCLUDED

#include<vector>
#include "GuiElement.h"

namespace Amju
{
class GuiComposite : public GuiElement
{
public:
  GuiComposite();

  virtual void DrawImpl();
  virtual bool Load(File*);
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void OnKey(char c);
  virtual void OnPlusUp(bool down);
  virtual void OnPlusDown(bool down);
  virtual void OnPlusLeft(bool down);
  virtual void OnPlusRight(bool down);

  virtual bool IsSelected() const;
  
  virtual void SetRelPos(float top, float left);
  virtual void SetAbsPos(float top, float left);

  void AddChild(GuiElement*);
  int GetNumberOfChildren() const;
  SharedPtr<GuiElement> GetChild(int i);

protected:
  virtual bool CanDrag();

protected:
  typedef std::vector<SharedPtr<GuiElement> > Elements;
  Elements m_children;

};
}

#endif

