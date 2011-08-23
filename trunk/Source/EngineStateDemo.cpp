/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateDemo.cpp,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateDemo.h"
#include "Engine.h"
#include "ThirdPersonCamera2.h"
#include "LevelServer.h"
#include "CharacterGameObject.h"
#include "EngineStateTitle.h"
#include "Npc.h"
#include "CooperationGroupManager.h"
#include "TextWriter.h"
#include "SchAssert.h"

namespace Amju
{
const std::string EngineStateDemo::Name = "demo";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateDemo::Name, new EngineStateDemo);

EngineStateDemo::EngineStateDemo() 
{
  //m_showControls = true;
  m_pCamera = new ThirdPersonCamera2;
  m_time = 0;
}

void EngineStateDemo::SetCameraTarget()
{
  // Get list of all NPCs in the level we are demoing.
  // Then pick one at random.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  std::vector<PGameObject> npcs;

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    //GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    // Take the piece if it's a Takeable and intersects the player.
    Npc* pNpc = dynamic_cast<Npc*>(pGo.GetPtr());
    if (pNpc)
    {
      npcs.push_back(pNpc);
    }
  }

  // No NPCs exist in the demo level/room.
  Assert(!npcs.empty());
  if (npcs.empty())
  {
    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
    return;
  }

  // Choose a random NPC.
  float fr = (float)rand() / (float)RAND_MAX;
  Assert(fr >= 0);
  Assert(fr < 1.0f);
  int r = npcs.size() * (int)fr;

  PGameObject pGo = npcs[r];

  CharacterGameObject* pPlayer = dynamic_cast<CharacterGameObject*>(pGo.GetPtr());
  Assert(pPlayer);

  m_pCamera->SetPlayer(pPlayer);
  m_pCamera->SetOrientation(*(pPlayer->GetOrientation()));
  m_pCamera->Reset();
}

void EngineStateDemo::DrawPlayer()
{
}

void EngineStateDemo::SetActive(bool active)
{
  //static int demoLevel = 100;

  if (active)
  {
    GetEngine()->SetLetterbox(true);

    m_pCamera->GetConfigValues(); // load up constants
    // TODO Replace the usual values with demo-specific constants as necessary

    //demoLevel++;

    m_pLevel = LevelServer::Instance()->GetCurrentLevel();
    int levelId = m_pLevel->GetId(); 

    Assert(m_pLevel.GetPtr());
    // TODO Should be set to room of current Clip.
    m_pLevel->SetRoomId(m_pLevel->GetStartRoomId());

    // Reset Takeables back to their start positions.
    ResetTakeables();

    // As for EngineRunning::NewLevel, we must rebuild the whole graph here.
    // But during play we want to only rebuild the minimum necessary.
    RecalcVisGraph(m_pLevel.GetPtr());

    ResetAis();

    // Point TP camera at a character
    SetCameraTarget();

    // Disable player, so his ghostly collision vol doesn't get in the way
    GetPlayer()->SetState(OUT_OF_PLAY);

    CooperationGroupManager::Instance()->ClearAllGroups();

    m_time = 0;
  }
  else
  {
    GetEngine()->SetLetterbox(false);
  }
}

void EngineStateDemo::Draw()
{
  EngineRunningVisGraph::Draw();

  m_time += GetEngine()->GetDeltaTime();
  // If time expires, go back to title state.
  static float timeLimit = atof(GetEngine()->GetConfigValue("demo_time_limit").c_str());
  if (m_time > timeLimit) // TODO CONFIG
  {
    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::FADE);
    m_time = 0;
  }

  // TODO
  // If character timer expires, switch focus to another character ?
}

void EngineStateDemo::Red(bool down)
{
  // Go to start game state.
  // Red button pressed during Intro sequence.. this means start the game.
  // Sound effect. 
  std::string startWav = GetEngine()->GetConfigValue("start_wav");
  GetEngine()->PlayWav(startWav);

  // DON'T go to the File state. This lets the user Load a saved game.
  // - currently we DON'T support save/load game.
  //GetEngine()->ChangeState(EngineFileChooser::Name, Engine::IMMEDIATE);

  GetEngine()->StartGame(""); // no filename 
}

PCamera EngineStateDemo::GetCamera()
{
  return m_pCamera;
}

void EngineStateDemo::DoCameraWork()
{
  // TP camera - refresh the HeightServer used for collision detection.
  ((ThirdPersonCamera2*)m_pCamera.GetPtr())->RefreshHeightServer(m_pLevel.GetPtr());

  m_pCamera->Update();

  // Skybox: this should be rotated as the camera changes its orientation,
  // but the distance should always be fixed.
  // NB The scene moves w.r.t. the skybox. If the skybox is not big enough,
  // it will 'clip' the scene far from the player!
  // (not if we turn off depth buffer writing)
  AmjuGL::PushMatrix();
  // TODO fix this for first-person camera.
  m_pCamera->DrawRotation();

  DrawSkybox();

  // Undo the skybox rotation.
  AmjuGL::PopMatrix();

  // "Draw" the camera, i.e. transform the view.
  m_pCamera->Draw();
}

void EngineStateDemo::GameOver()
{
  // This can get called if all the AIs reach their final targets while 
  // we are in Demo mode.
  // In this case reset all Game Objects to their start positions, and
  // go back to Title mode.
  //ResetTakeables();??

  // TODO
//  OnNewLevel();

  GetEngine()->ChangeState(EngineStateTitle::Name, Engine::FADE);
}

void EngineStateDemo::DrawOverlays()
{
  GetEngine()->Fps();
}

}

