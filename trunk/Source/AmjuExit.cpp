/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: AmjuExit.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:12  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:24  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.4  2006/05/09 18:16:56  jay
Mac OS X: Don't just exit, post a Quit event, so we can clean up full
screen, etc.

Revision 1.1.2.3  2006/03/24 23:17:58  jay
Turn off nagging if not unlockable

Revision 1.1.2.2  2005/11/11 13:40:01  jay
Turn off music when we exit (Win) as music can skip during exit.

Revision 1.1.2.1  2005/09/23 19:56:35  jay
Add exit functions, which go to Nag Mode if unregistered.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "AmjuExit.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShotNag.h"
#include "PoolMisc.h"
#include "Registration.h"
#include "SoundPlayer.h"

#ifdef MACOSX
#include <Carbon/carbon.h>
#endif

using namespace Amju;

void OnExitClicked()
{
#ifdef AMJU_NOT_UNLOCKABLE
  ReallyExit();
#else
  if (Registration::IsRegistered())
  {
    ReallyExit();
  }
  else
  {
    StartDemoMode();
    SetNagMode(true);
    Engine::Instance()->ChangeState(EngineStatePoolSetUpShotNag::Name,
      Engine::IMMEDIATE);
  }
#endif
}

#ifdef MACOSX
void MacPostQuitEvent()
{
  EventRef windowEvent;
  OSStatus err = MacCreateEvent (
    nil, 
    kEventClassCommand, 
    kEventProcessCommand, 
    0, kEventAttributeNone, 
    &windowEvent);

  if (err == noErr) 
  {
    HICommand command; // set HI command parameter...
    command.commandID = kHICommandQuit; 
    command.attributes = 0;
    command.menu.menuRef = 0; 
    command.menu.menuItemIndex = 0;
    err = SetEventParameter(
      windowEvent, 
      kEventParamDirectObject, 
      kEventParamHICommand, 
      sizeof(command), &command);

    if (err == noErr)
    {
std::cout << "Posting quit event\n";

      err = PostEventToQueue (
        GetCurrentEventQueue(), 
        windowEvent, 
        kEventPriorityHigh);
      ReleaseEvent (windowEvent);
    }
  }
}
#endif

void ReallyExit()
{
  // Turn off music - in full screen it skips while we exit.
  Engine::Instance()->GetSoundPlayer()->SetSongMaxVolume(0);

#ifdef WIN32
  PostQuitMessage(0); 
#endif
#ifdef MACOSX
  MacPostQuitEvent();
#endif 
}


