/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolShowShotNag.cpp,v $
Revision 1.1.2.2  2006/08/30 21:12:15  Administrator
Get Pool to build in MSVC

Revision 1.1.2.1  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:05  jay
Add NagMode states

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolShowShotNag.h"
#include "EngineStatePoolSetUpShotNag.h"
#include "Engine.h"

namespace Amju
{
const char* EngineStatePoolShowShotNag::Name = "golf_show_shot_nag";

static const bool registered= Engine::Instance()->
  RegisterEngineState(EngineStatePoolShowShotNag::Name,
  new EngineStatePoolShowShotNag);

void EngineStatePoolShowShotNag::DrawOverlays()
{
  EngineStatePoolSetUpShotNag::SDrawOverlays();
}
}


