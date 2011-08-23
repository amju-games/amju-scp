/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateThanks.h,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_THANKS_H_INCLUDED)
#define ENGINE_STATE_THANKS_H_INCLUDED

#include "EngineStateText.h"
#include "GuiElement.h"

namespace Amju
{
class EngineStateThanks : public EngineStateText
{
public:
  static const char* Name;

  virtual void Draw();
  virtual void Red(bool);
  virtual bool Load();
  virtual void MouseButton(bool, bool, bool);
  virtual void SetActive(bool);

protected:
  PSolidComponent m_pHearts;

  PGuiElement m_pOkButton;
};
}

#endif


