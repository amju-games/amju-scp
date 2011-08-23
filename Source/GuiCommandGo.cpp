/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiCommandGo.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:16  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:20  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiCommandGo.h"
#include "Engine.h"
#include "EngineStatePoolShowShot.h"

namespace Amju
{
bool GuiCommandGo::Do()
{
  Engine::Instance()->ChangeState(EngineStatePoolShowShot::Name, Engine::IMMEDIATE);
  return false; // no undo
}

}


