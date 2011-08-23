/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: EsLogo.h,v $
Revision 1.1.2.2  2006/03/08 19:06:15  jay
Added Logo state - shows Amju logo and BOONTY if defined

Revision 1.1  2006/02/16 13:29:07  jay
State to show Amju Games logo on startup

*/

#ifndef ES_LOGO_H_INCLUDED
#define ES_LOGO_H_INCLUDED

#include <vector>
#include "EngineState.h"
#include "TexturedQuad.h"

namespace Amju
{
class EsLogo : public EngineState
{
public:
  static const char* Name;
  EsLogo();
  virtual void SetActive(bool);
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Update();
  virtual void TimerExpired();

protected:
  TexturedQuad m_bg;
  std::vector<TexturedQuad> m_logos;
};
}

#endif

