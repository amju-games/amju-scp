/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolSetUpShotNag.cpp,v $
Revision 1.1.2.2  2007/06/23 11:44:11  Administrator
MSVC fix

Revision 1.1.2.1  2006/08/14 17:50:16  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:28  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2006/05/12 08:21:51  jay
Minor adjustment to nag timer

Revision 1.1.2.2  2006/03/10 19:06:29  jay
Trial may not be unlockable

Revision 1.1.2.1  2005/09/23 19:58:05  jay
Add NagMode states

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolSetUpShotNag.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "AmjuExit.h"
#include "LevelServer.h"
#include "TextWriter.h"
#include "StringUtils.h"

namespace Amju
{
extern void ReportError(const std::string&);

const char* EngineStatePoolSetUpShotNag::Name = "golf_setup_nag";

static const bool registered= Engine::Instance()->
  RegisterEngineState(EngineStatePoolSetUpShotNag::Name,
  new EngineStatePoolSetUpShotNag);

HelpText EngineStatePoolSetUpShotNag::s_helpText;

bool EngineStatePoolSetUpShotNag::Load()
{
  if (!s_helpText.Load("nag.txt")) // TODO CONFIG
  {
    ReportError("Failed to load nag text");
    return false;
  }
  return EngineStatePoolSetUpShot::Load();
}

void EngineStatePoolSetUpShotNag::DrawOverlays()
{
  SDrawOverlays();
}

void EngineStatePoolSetUpShotNag::SDrawOverlays()
{
  s_helpText.Draw(0);

#ifndef AMJU_NOT_UNLOCKABLE
  s_buynowButton->Draw();
#endif

  static float count = 10.0f; // TODO CONFIG
  count -= Engine::Instance()->GetDeltaTime();
  if (count <= 1) 
  {
    ReallyExit();
  }
  Engine::Instance()->GetTextWriter()->PrintCentre(15.0f, 
    ToString((int)count).c_str());  

/*
  // Keep changing the Level/Room to show the user all the excellent games.
// Doesn't work, character positions are not reset, eventually Asserts.
  // TODO Fix this!
  static float roomcount = 0;
  roomcount += Engine::Instance()->GetDeltaTime();
  if (roomcount > 6.0f) // TODO CONFIG
  {
    roomcount = 0;
    // Change the room
    StartDemoMode();
  }
*/
}

void EngineStatePoolSetUpShotNag::SetActive(bool active)
{
  EngineStatePoolSetUpShot::SetActive(active);
}
}


