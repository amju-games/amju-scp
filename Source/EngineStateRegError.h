/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateRegError.h,v $
Revision 1.1.10.1  2007/03/25 17:50:17  jay
No letterbox for Sorry state

Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_REG_ERROR_H_INCLUDED)
#define ENGINE_STATE_REG_ERROR_H_INCLUDED

#include "EngineStateText.h"
#include "GuiElement.h"

namespace Amju
{
class EngineStateRegError : public EngineStateText
{
public:
  static const char* Name;
  virtual void SetActive(bool active);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Red(bool);
  virtual bool Load();
  virtual void MousPos(int, int);
  virtual void MouseButton(bool, bool, bool);
  void SetRegCode(const std::string& s) { m_regCode = s; }

protected:
  // The code entered
  std::string m_regCode;

  // Back button
  PGuiElement m_pBackButton;
  // Back text
  //PSolidComponent m_pBackText;

};
}

#endif




