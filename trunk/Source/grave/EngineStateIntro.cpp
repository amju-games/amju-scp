/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateIntro.cpp,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateIntro.h"
#include "EngineRunning.h"
#include "EngineFileChooser.h"
#include "EngineStateTitle.h"
#include "EngineStateTextGameOver.h"
#include "Engine.h"
#include "CompositeFactory.h"
#include "SolidComponent.h"
#include "File.h"
#include "LeafData.h"
#include "LeafRenderer.h"
#include "LevelServer.h"

using namespace std;

namespace Amju
{
const std::string EngineStateIntro::Name = "intro";
// Register this State with the Engine.
//static const bool registered = Engine::Instance()->
//  RegisterEngineState(EngineStateIntro::Name, new EngineStateIntro);

EngineStateIntro::EngineStateIntro() : 
  m_pOverlay(0),
  m_overlayDelay(0),
  m_counter(0)
{
  m_pClipSequence = new ClipSequence;
}

EngineStateIntro::~EngineStateIntro()
{
  Clear();
}

void EngineStateIntro::FinishCutScene()
{
}

void EngineStateIntro::SetActive(bool active)
{
  if (active)
  {
    m_pSeq = m_pClipSequence;

    Assert(0);
/*
    // TODO!
    // We should actually get the level ID of the current Clip being played.
    int levelId = 0; //GetEngine()->GetGameState()->GetRealLevel(); 
    m_pLevel = LevelServer::Instance()->GetLevel(levelId);
    // TODO Should be set to room of current Clip.
    m_pLevel->SetRoomId(m_pLevel->GetStartRoomId());

    // Reset Takeables back to their start positions.
    ResetTakeables();

    ResetAis();

    
    RecalcVisGraph(m_pLevel);

    m_time = 0;
    Initialise();
    m_pSeq->Start();
    ++m_counter;
*/
  }
  else
  {
    m_pSeq->Stop();
  }

  EngineCutSceneState::SetActive(active);
}

bool EngineStateIntro::Load()
{
  string introFileName = GetEngine()->GetConfigValue("intro");
  if (introFileName.empty())
  {
    GetEngine()->ReportError("No intro file name in config file.");
    return false;
  }

  File introFile;
  if (!introFile.OpenRead(introFileName))
  {
    introFile.ReportError("Couldn't open intro file.");
    return false;
  }

  if (!m_pClipSequence->Load(&introFile))
  {
    return false;
  }
  // Load the overlay. This is a composite that gets slapped over 
  // everything else.
  string overlayName;
  if (!introFile.GetDataLine(&overlayName))
  {
    introFile.ReportError("Expected overlay name.");
    return false;
  }
  // Overlay bitmap - a leaf ?
  m_pOverlay = SolidComponent::LoadSolid(overlayName);
  if (!m_pOverlay.GetPtr())
  {
    introFile.ReportError("Failed to load Intro overlay.");
    return false;
  }

  // Get the overlay delay: the time to wait before showing the overlay.
  if (!introFile.GetFloat(&m_overlayDelay))
  {
    introFile.ReportError("Expected overlay delay.");
    return false;
  }

  EngineCutSceneState::Load();
  Initialise();

  return true;
}

void EngineStateIntro::Clear()
{
  m_pOverlay = 0;
  m_pClipSequence->Clear();
}

void EngineStateIntro::Draw()
{
  m_time += GetEngine()->GetDeltaTime();

  // EngineCutSceneState::Draw() is no good as it tells the Engine to carry on with the
  // Running state when the cut scene finishes.

  if (m_pSeq->IsFinished())
  {
    // Change state.. the new state depends on how many times we have 
    // shown the intro.
    if (m_counter == 1)
    {
      GetEngine()->ChangeState(EngineStateTitle::Name, Engine::FADE);
    }
    else if (m_counter == 2)
    {
      GetEngine()->ChangeState(EngineStateTextGameOver::Name, Engine::FADE);
      m_counter = 0;
    }

    return;
  }

  m_pLevel = m_pSeq->GetLevel();
  GetPlayer()->SetLevel(m_pLevel.GetPtr());

  m_pSeq->Draw();

  // Don't draw the sequence, use the base class Draw().
  EngineRunningBase::Draw();
}

void EngineStateIntro::DrawOverlays()
{
  // Delay drawing the overlay.
  if (m_pOverlay.GetPtr() && m_time >= m_overlayDelay)
  {
    m_pOverlay->Draw();
  }
  //EngineCutSceneState::DrawOverlays();
}

void EngineStateIntro::Red(bool down)
{
  // Red button pressed during Intro sequence.. this means start the game.
  // Sound effect. 
  string startWav = GetEngine()->GetConfigValue("start_wav");
  GetEngine()->PlayWav(startWav);

  // DON'T go to the File state. This lets the user Load a saved game.
  // - currently we DON'T support save/load game.
  //GetEngine()->ChangeState(EngineFileChooser::Name, Engine::IMMEDIATE);

  GetEngine()->StartGame(""); // no filename 
}
}


