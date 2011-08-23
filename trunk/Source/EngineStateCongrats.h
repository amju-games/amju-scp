/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateCongrats.h,v $
Revision 1.1.2.2  2006/08/14 17:50:08  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_CONGRATS_H_INCLUDED)
#define ENGINE_STATE_CONGRATS_H_INCLUDED 

#include "EngineStateText.h"
#include "GuiElement.h"

namespace Amju
{
class EngineStateCongrats : public EngineStateText
{
public:
  static const char* Name;

  virtual bool Load();
  virtual void Draw();
  virtual void MouseButton(bool, bool, bool);

protected:
  PGuiElement m_pOkButton;
};
}

#endif


