/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineCutSceneState.cpp,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineCutSceneState.h"
// TODO fix this dependency
#include "EngineRunning.h"
#include "Engine.h"
#include "Level.h"
#include "Clip.h"
#include "CameraPath.h"

namespace Amju
{
const std::string EngineCutSceneState::Name = "cutscene";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineCutSceneState::Name, new EngineCutSceneState);

EngineCutSceneState::EngineCutSceneState() :
  m_pSeq(0)
{
}

void EngineCutSceneState::SetSequence(PClipSequence pSeq)
{
  m_pSeq = pSeq;
  m_pSeq->Start();
}

void EngineCutSceneState::FinishCutScene()
{
  GetEngine()->SetLetterbox(false); 
  GetEngine()->ChangeState(EngineRunning::Name, Engine::IMMEDIATE); 
}

void EngineCutSceneState::JoyX(float f)
{
  m_pSeq->JoyX(f);
}

void EngineCutSceneState::JoyY(float f)
{
  m_pSeq->JoyY(f);
}

void EngineCutSceneState::PlusUp(bool b)
{
  m_pSeq->JoyY(b ? 1.0f : 0);
}

void EngineCutSceneState::PlusDown(bool b)
{
  m_pSeq->JoyY(b ? -1.0f : 0);
}

void EngineCutSceneState::PlusLeft(bool b)
{
  m_pSeq->JoyX(b ? -1.0f : 0);
}

void EngineCutSceneState::PlusRight(bool b)
{
  m_pSeq->JoyX(b ? 1.0f : 0);
}

void EngineCutSceneState::YellowUp(bool)
{
}

void EngineCutSceneState::Blue(bool down)
{
  m_pSeq->Blue(down);
}

void EngineCutSceneState::Green(bool down)
{
  m_pSeq->Green(down);
}

void EngineCutSceneState::Red(bool down)
{
  m_pSeq->Red(down);
}

void EngineCutSceneState::SetActive(bool active)
{
  if (active)
  {
    m_pLevel = m_pSeq->GetLevel();
    GetPlayer()->SetLevel(m_pLevel.GetPtr());

    GetEngine()->SetLetterbox(true);
  }
  else
  {
    GetEngine()->SetLetterbox(false);
  }
}

bool EngineCutSceneState::Load()
{
  EngineRunningBase::Load();
  return true;
}

void EngineCutSceneState::Clear()
{
}

PCamera EngineCutSceneState::GetCamera()
{
  SharedPtr<CameraPath> pCam = m_pSeq->GetCurrentClip()->GetCameraPath();
  return pCam.GetPtr();
}

void EngineCutSceneState::DoCameraWork()
{
  SharedPtr<CameraPath> pCam = m_pSeq->GetCurrentClip()->GetCameraPath();

  AmjuGL::PushMatrix();
  pCam->DrawRotation();
  DrawSkybox();
  AmjuGL::PopMatrix();

  pCam->Draw();
}

void EngineCutSceneState::Draw()
{
  if (m_pSeq->IsFinished())
  {
    //GetEngine()->FinishCutScene();
    return;
  }

  // Set up the level and room. Then call base class Draw().
  m_pLevel = m_pSeq->GetLevel();
  GetPlayer()->SetLevel(m_pLevel.GetPtr());
  m_pSeq->Draw();

  // Don't draw the sequence, use the base class Draw().
  // (We override DoCameraWork to use our Camera).
  EngineRunningBase::Draw();
}

void EngineCutSceneState::DrawOverlays()
{
  m_pSeq->DrawOverlays();
}

}

