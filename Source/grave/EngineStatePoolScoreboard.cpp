/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolScoreboard.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolScoreboard.h"
#include "Engine.h"
#include "GameState.h"
#include "LevelServer.h"
#include "PoolMisc.h"
#include "Trajectory.h"
#include "Controller.h"
#include "TextFactory.h"
#include "StringUtils.h"
#include "Level.h"
#include "Bonus.h"
#include "EngineStatePoolCourseSelect.h"
#include "EngineStateTitle.h"
#include "PoolCourseManager.h"

namespace Amju
{
const char* EngineStatePoolScoreboard::Name = "scoreboard";

static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolScoreboard::Name, new EngineStatePoolScoreboard);

void EngineStatePoolScoreboard::CreateText(GameState::PlayerInfo* pPi, int r)
{
  Assert(r >= 0);
  Assert(r < 4);
  m_ranks[r] = TextFactory::Instance()->Create(ToString(r + 1));
  m_names[r] = TextFactory::Instance()->Create(
    pPi->m_name,
    TextFactory::Instance()->GetCurrentColourizer(), 
    false); // false => don't centre
  m_scores[r] = TextFactory::Instance()->Create(ToString(pPi->m_score));
}

void EngineStatePoolScoreboard::DrawLine(int i)
{
  glPushMatrix();
  glTranslatef(m_rankPos, -i* 2.0f, 0);
  m_ranks[i]->Draw();
  glTranslatef(m_namePos, 0, 0);
  m_names[i]->Draw();
  glTranslatef(m_scorePos, 0, 0);
  m_scores[i]->Draw();
  glPopMatrix();
}

void EngineStatePoolScoreboard::DrawOverlays()
{
  glDisable(GL_DEPTH_TEST);
  glPushMatrix();

  glTranslatef(m_left, m_top, m_z);

  // Draw a line of text for each player, giving name, score and ranking.
  // TODO sort
  int numPlayers = Engine::Instance()->GetGameState()->GetNumberOfPlayers();
  for (int i = 0; i < numPlayers; i++)
  {
    DrawLine(i);
  }

  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
}

void EngineStatePoolScoreboard::SetActive(bool active)
{
  EngineStatePoolFlyby::SetActive(active);

  if (active)
  {
    int numPlayers = Engine::Instance()->GetGameState()->GetNumberOfPlayers();
    for (int i = 0; i < numPlayers; i++)
    {
      GameState::PlayerInfo* pPi = 
        GetEngine()->GetGameState()->GetPlayerInfo(i);
      CreateText(pPi, i);
    }
  }
}

void EngineStatePoolScoreboard::Update()
{
  EngineStatePoolFlyby::Update();
}

bool EngineStatePoolScoreboard::Load()
{
  if (!EngineStatePoolFlyby::Load())
  {
    return false;
  }
  m_z = GetEngine()->GetConfigFloat("golf_scoreboard_z");
  m_top = GetEngine()->GetConfigFloat("golf_scoreboard_y");
  m_rankPos = GetEngine()->GetConfigFloat("golf_scoreboard_rank");
  m_namePos = GetEngine()->GetConfigFloat("golf_scoreboard_name");
  m_scorePos = GetEngine()->GetConfigFloat("golf_scoreboard_score");

  return true;
}

int EngineStatePoolScoreboard::GetAllBonusesInPlay()
{
  int res = 0;
  // Get the game objects which are in the same room as the player.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;

    Bonus* pBonus = dynamic_cast<Bonus*>(pGo.GetPtr());
    if (pBonus && pBonus->GetState() != OUT_OF_PLAY)
    {
      res++;
    }
  }
  return res;
}

void EngineStatePoolScoreboard::UnlockNextCourse()
{
  int id = m_pLevel->GetId(); // TODO check this always corresponds to Course index
  // Unlock next course
  PoolCourseManager::Instance()->UnlockNext(id, 0);
}

void EngineStatePoolScoreboard::Red(bool down)
{
  if (!down) 
  {
    return;
  }

  // Is there another hole for the current level ?
  // (I.e. another room)
  // If so go to that room, go to Player Up state.

  // All holes completed for this course.
  // Get the bonuses for the current level. If they are all OUT_OF_PLAY
  // they have all been collected, and so the next course is unlocked.
  // To unlock the new course name, add course to a config file.
  // If the next course has been unlocked, go to the course select menu.
  // Otherwise go to the title state.

  int numBonuses = GetAllBonusesInPlay();
  if (numBonuses == 0)
  {
    UnlockNextCourse();
    GetEngine()->ChangeState(EngineStatePoolCourseSelect::Name, Engine::IMMEDIATE);
  }
  else
  {
    GetEngine()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
}

}

