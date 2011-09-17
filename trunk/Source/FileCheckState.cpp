/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileCheckState.cpp,v $
Revision 1.2.6.14  2007/12/10 22:56:38  jay
Update version number

Revision 1.2.6.13  2007/08/11 10:08:06  jay
New version: Bass sound player updated to work when buil on Win Vista

Revision 1.2.6.12  2007/08/04 10:37:42  Administrator
Update version v.2.0.0

Revision 1.2.6.11  2007/07/31 06:21:43  jay
New version number

Revision 1.2.6.10  2007/06/11 20:29:20  jay
Update version string

Revision 1.2.6.9  2007/05/28 12:55:05  Administrator
New version string

Revision 1.2.6.8  2007/03/27 16:57:14  jay
Show version number TODO move to GameSpecific file

Revision 1.2.6.7  2007/03/23 09:20:07  jay
Update copyright info

Revision 1.2.6.6  2006/07/25 08:01:22  jay
Use IsFullScreen(), not a global variable

Revision 1.2.6.5  2006/05/12 08:09:25  jay
Add message for Mac full screen resolution change keys

Revision 1.2.6.4  2006/03/13 09:31:03  jay
Copyright text

Revision 1.2.6.3  2006/03/12 00:34:41  Administrator
New game logo

Revision 1.2.6.2  2006/03/08 19:21:31  jay
Full screen splash

Revision 1.2.6.1  2005/07/05 21:53:58  jay
Update text

Revision 1.2  2004/09/20 13:28:51  Administrator
Max progress bar value made configurable

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include <string>
#include "Common.h"
#include "FileCheckState.h"
#include "Engine.h"
#include "EngineStateTitle.h" // TODO TEMP TEST
#include "EngineRunning.h"
#include "TexturedQuad.h"
#include "PowerGuage.h"
#include "TextWriter.h"

#ifdef MACOSX
//extern bool g_isFullScreen;
bool IsFullScreen();
#endif

namespace Amju
{
const std::string FileCheckState::Name = "filecheck";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(FileCheckState::Name, new FileCheckState);


// Callback function - called when a state or level worker is loaded.
// NB This was written to be thread safe but this is overkill as we must
// load in the main thread :-( 
void fileCheckStateProgress()
{
  static FileCheckState* pFileCheckState = (FileCheckState*)
    Engine::Instance()->GetEngineState(FileCheckState::Name).GetPtr();  

  pFileCheckState->IncProgressCount();
}

FileCheckState::FileCheckState()
{
  m_frames = 0;
  m_success = false;
  Engine::Instance()->SetProgressCallback(&fileCheckStateProgress);
  ResetProgressCount();
  // Set temp value until config file has been read in.
  SetMaxProgressCount(100); 
}

void FileCheckState::SetActive(bool active)
{
  if (!active)
  {
    return;
  }

  Engine::Instance()->SetClearColour(1.0f, 1.0f, 1.0f);

  // Start a thread which loads the Engine States.
  // The Engine will call the progress bar callback above.
  //LoadThread* pLoadThread = new LoadThread;
  //pLoadThread->Start();

}

void FileCheckState::DeathBlow()
{
  GetEngine()->ReportError("Sorry, cannot continue due to file errors.");
  GetEngine()->ReportError("Please quit and check files.");
}

void FileCheckState::Draw()
{
}

void FileCheckState::DrawOverlays()
{
    static TexturedQuad tq;
    static bool first = true;
    if (first)
    {
        first = false;

        // Total number of engine states, sections in Engine::Load and the total
        // no of level workers.
        static const int maxProgress = 
          (int)(Engine::Instance()->GetConfigFloat("max_progress"));
        SetMaxProgressCount(maxProgress);  

        Engine::Instance()->SetClearColour(1.0f, 1.0f, 1.0f);
        tq.Load("supercoolpool.png", "");

        m_pGuage = new PowerGuage;
        File ftimer;
        if (!ftimer.OpenRead(Engine::Instance()->GetConfigValue("loadtimer")))
        {
          GetEngine()->ReportError("Bad timer file.");
          //return false;
        }
        if (!m_pGuage->Load(&ftimer))
        {
          GetEngine()->ReportError("Failed to load timer.");
          //return false;
        }

    }      

    m_frames++;

    Engine* e = GetEngine();

    e->GetTextWriter()->Print(0, 0, "Amju Super Cool Pool");
    // Extra large splash screen
    tq.Draw(5, 0, 10, 25);

    e->GetTextWriter()->Print(0, 0, "Amju Super Cool Pool v.3-0.1");
    e->GetTextWriter()->Print(0, 1, "© Copyright 2005-2011 Amju Games");
    e->GetTextWriter()->Print(0, 2, "Please visit www.amju.com for updates and more games!");        

#ifdef SHOW_FULLSCREEN_TEXT
    // Tell user that + and - keys change full screen resolution
    if (IsFullScreen())
    {
      e->GetTextWriter()->Print(0, 4, "You can use the + and - keys to change screen resolution.");
    }
#endif

    DrawProgressBar();

    // Once Load() returns true, it has finished loading.
    // false has meant that the load failed, but for a progress bar
    // we can also let it mean that there is still more to load. 
    if (Engine::Instance()->Load())
    {
        if (e->GetConfigValue("load_test") == "y")
        {
            // For profiling, exit immeditately.
            //exit(0); 
            e->ReportError("Loaded ok.");
        }
        else
        {
            e->ClearText();
            e->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
            e->SetClearColour(0, 0, 0);
        }
    }

    //DrawProgressBar();
}

void FileCheckState::ClearScrollText()
{
  // We want file error messages to persist, so don't clear scrolling text.
}
}

