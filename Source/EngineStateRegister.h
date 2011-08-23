/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateRegister.h,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_REGISTER_H_INCLUDED)
#define ENGINE_STATE_REGISTER_H_INCLUDED

#include "EngineStateEnterText.h"

namespace Amju
{
class EngineStateRegister : public EngineStateEnterText
{
public:
  static const char* Name;

  EngineStateRegister();

  virtual bool Load();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

//protected:
  virtual void Finished();
  virtual void Back();

protected:
  // Buttons
  PGuiElement m_pOkButton;
  PGuiElement m_pCancelButton;
  PGuiElement m_pBuyButton;

  // Text next to buttons
  PSolidComponent m_pOkText;
  PSolidComponent m_pCancelText;
  PSolidComponent m_pBuyText;

};
}

#endif

