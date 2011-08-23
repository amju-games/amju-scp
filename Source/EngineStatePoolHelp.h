/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolHelp.h,v $
Revision 1.1.2.2  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/07/05 09:42:34  jay
Added help state

*/

#ifndef ENGINE_STATE_POOL_HELP_H_INCLUDED
#define ENGINE_STATE_POOL_HELP_H_INCLUDED

#include "EngineStateText.h"
#include "GuiElement.h"

namespace Amju
{
class EngineStatePoolHelp : public EngineStateText
{
public:
  static const char* Name;

  virtual void SetActive(bool);
  virtual bool Load();
  virtual void Red(bool down);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);

  void SetScrollVel(float);

protected:
  PGuiElement m_pOkButton;
  PGuiElement m_pScrollUpButton;
  PGuiElement m_pScrollDownButton;
  float m_y; // position of top line of text  
  // Scroll velocity
  float m_yVel;
};
}

#endif

