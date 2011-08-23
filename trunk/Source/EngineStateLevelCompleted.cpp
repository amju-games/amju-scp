/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateLevelCompleted.cpp,v $
Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateLevelCompleted.h"
#include "EngineStateShowLevel.h"
#include "Engine.h"
#include "LevelServer.h"
#include "GameState.h"
#include "SimpleTargetCamera.h"
#include "MoveMemorizer.h"

namespace Amju
{
const std::string EngineStateLevelCompleted::Name = "level_comp";

// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateLevelCompleted::Name, new EngineStateLevelCompleted);

EngineStateLevelCompleted::EngineStateLevelCompleted()
{
  m_time = 0;
  m_pieceIndex = -1;
  m_finished = false;
  m_pCamera = new SimpleTargetCamera;

}

void EngineStateLevelCompleted::DoCameraWork()
{
  m_pCamera->Update();
  AmjuGL::PushMatrix();
  m_pCamera->DrawRotation();
  DrawSkybox();
  AmjuGL::PopMatrix();
  m_pCamera->Draw();
}

void EngineStateLevelCompleted::DisableAis()
{
/*
  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  GameObjectMap& objs = GetEngine()->GetGameObjects();
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;

    Ai* pAi = dynamic_cast<Ai*>(pGo.GetPtr());
    if (pAi)
    {
      pAi->SetState(OUT_OF_PLAY);
    }
  }
*/
}

void EngineStateLevelCompleted::Draw()
{
  if (m_finished)
  {
    return;
  }

  float dt = GetEngine()->GetDeltaTime();
  m_time += dt;

  // divide some total time (e.g. 5 secs) by no of pieces.
  float timeForOnePiece = 5.0f / (float)MoveMemorizer::Instance()->Size();
  // TODO CONFIG

  if (m_time > timeForOnePiece) 
  {
    m_time = 0;

    // Get the next piece from the move memorizer. If no more pieces, go 
    // to next state.
    if (m_pieceIndex >= MoveMemorizer::Instance()->Size())
    {
      m_finished = true;
    }
    else
    {
      const MemMove& m = MoveMemorizer::Instance()->GetMove(m_pieceIndex);
      m_pieceIndex++;
      PGameObject pGo = GetEngine()->GetGameObject(m.m_id);
      // Put piece in new posision. 
      pGo->SetOrientation(m.m_orAfter);

      // Point camera at it.
      ((SimpleTargetCamera*)m_pCamera.GetPtr())->SetTarget(pGo.GetPtr());
      // TODO Also change camera position ?
    }
  } 

  if (m_finished) 
  {
    // Increment level number as seen by player.
//    int levelnum = GetEngine()->GetGameState()->GetFloat("display_level"); 
//    levelnum++;
//    GetEngine()->GetGameState()->SetCurrentLevel(levelnum);
 
    // Increase the "real" level ID, and reset if necessary
    int realLevel = (int)GetEngine()->GetGameState()->GetFloat("real_level"); 
    realLevel++;
    GetEngine()->GetGameState()->Set("real_level", realLevel);

    // TODO load new level file.
//    LevelServer::Instance()->Load("");

    PLevel pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
    if (!pLevel.GetPtr())
    {
      // We have run out of levels! 
      GetEngine()->GetGameState()->Set("real_level", 0); 
    }

    // Save the game state for next time
    GetEngine()->GetGameState()->Save();


    //EngineStateShowLevel* pEsl = (EngineStateShowLevel*)(
    //  GetEngine()->GetEngineState(EngineStateShowLevel::Name)).GetPtr();
    //pEsl->SetNumber(levelnum);

    GetEngine()->ChangeState(EngineStateShowLevel::Name, Engine::FADE);
  }
  else
  {
    EngineRunningVisGraph::Draw();
  }
}

void EngineStateLevelCompleted::SetActive(bool active)
{
  m_time = 1000.0f; // deliberately large to force the first call to Draw()
                    // to set the target.. a bit hacky
  m_finished = false;
  if (active)
  {
    m_pLevel  = LevelServer::Instance()->GetCurrentLevel();

    GetEngine()->SetLetterbox(true);

    m_pCamera->GetConfigValues(); // load up constants

    m_pCamera->SetOrientation(Orientation(0, 20.0f, 0, 0, 0, 0));

    // Set pieces to start positions.
    ResetTakeables();
    m_pieceIndex = 0;

    // Set all AIs to OUT_OF_PLAY. We don't want them to react to changes
    // we make to piece positions!
    DisableAis();

    GetPlayer()->SetState(OUT_OF_PLAY);
  }
  else
  {
    GetEngine()->SetLetterbox(false);
  }
}

void EngineStateLevelCompleted::DrawPlayer()
{
}

void EngineStateLevelCompleted::GameOver()
{
}

void EngineStateLevelCompleted::Update()
{
  // Call base class version
  EngineStateDemo::Update();
}

}

