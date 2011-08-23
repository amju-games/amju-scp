/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolShotInPlayNag.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:05  jay
Add NagMode states

*/

#include "EngineStatePoolShotInPlayNag.h"
#include "EngineStatePoolSetUpShotNag.h"
#include "Engine.h"

namespace Amju
{
const char* EngineStatePoolShotInPlayNag::Name = "golf_inplay_nag";

static const bool registered= Engine::Instance()->
  RegisterEngineState(EngineStatePoolShotInPlayNag::Name,
  new EngineStatePoolShotInPlayNag);

void EngineStatePoolShotInPlayNag::DrawOverlays()
{
  EngineStatePoolSetUpShotNag::SDrawOverlays();
}
}


