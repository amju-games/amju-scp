/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: RulesTrickShot.cpp,v $
Revision 1.1.2.5  2007/07/15 22:03:45  Administrator
Fixed text for "did not hit anything" but this seems to not be used:
instead the Rules (base class) text is used.

Revision 1.1.2.4  2007/06/10 21:33:27  Administrator
Check for extra states BALL_IN_POCKET and BALL_OUT_OF_BOUNDS

Revision 1.1.2.3  2006/08/30 21:12:21  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:40  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.7  2005/09/29 19:13:47  jay
ValidBalls must check IsPoolBall so we don't count subclassed objects

Revision 1.1.2.6  2005/09/25 20:14:23  Administrator
Added msg when no more shots

Revision 1.1.2.5  2005/09/23 19:36:42  jay
Override BallHitsObject so hitting anything counts as no foul; use PoolChangeState

Revision 1.1.2.4  2005/09/20 13:18:56  jay
Custom behaviour

Revision 1.1.2.3  2005/09/17 21:16:13  jay
Add Scene Ed Properties

Revision 1.1.2.2  2005/09/17 16:18:10  jay
Fixes to get Trick Shots to work

Revision 1.1.2.1  2005/09/16 20:04:19  jay
Add Trick Shot rules

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "RulesTrickShot.h"
#include "Engine.h"
#include "File.h"
#include "EngineStateTitle.h"
#include "EngineStatePoolTrickShotOver.h"
#include "PoolMisc.h"
#include "StringUtils.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "PoolGameState.h"
#include "PoolPlayerStats.h"
#include "LevelServer.h"
#include "PoolChangeState.h"

namespace Amju
{
PoolGameObject* CreateRulesTrickShot()
{
  return new RulesTrickShot;
}

PoolBehaviour::ValidBalls PoolBehaviourTrickShot::GetValidBalls()
{
  ValidBalls vb;

  // Loop through all balls in this room
  PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
 
  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PoolGameObject* pGo = it->second.GetPtr();
    if (IsPoolBall(pGo))
    {
      PoolBall* pBall = dynamic_cast<PoolBall*>(pGo);
      Assert(pBall);
      if (pBall && InPlay(pBall) && !IsCueBall(pBall))
      {
        vb.push_back(pBall);
      }
    }
  }
  return vb;
}

RulesTrickShot::RulesTrickShot()
{
  m_pBehaviour = new PoolBehaviourTrickShot;
  m_helpfilename = "help-trickshot.txt";
}

Rules* RulesTrickShot::Clone()
{
  return new RulesTrickShot(*this);
}

const char* RulesTrickShot::GetTypeName() const
{
  return "rules-trick-shot";
}

bool RulesTrickShot::Load(File* pf)
{
  if (!Rules::Load(pf))
  {
    pf->ReportError("Failed to load trick shot rules help");
    return false;
  }
  if (!pf->GetInteger(&m_maxNumShots))
  {
    pf->ReportError("Expected Trick Shot max no of shots");
    return false;
  }
  return true;
}

void RulesTrickShot::OnRoomEntry()
{
  Rules::OnRoomEntry();

  int numBalls = m_pBehaviour->GetValidBalls().size();
  m_text = "Pot ";
  if (numBalls == 1)
  {
    m_text += "the ball";
  }
  else if (numBalls == 2)
  {
    m_text += "both balls";
  }
  else
  {
    m_text += "all the balls";
  }
  m_text += " in ";
  m_text += ToString(m_maxNumShots);
  m_text += " shot";
  if (m_maxNumShots > 1)
  {
    m_text += "s";
  }
  m_shotsRemaining = m_maxNumShots;

  // HACK: Force player 0 to always be the active player. We never want
  // the AI player to start.
  Engine::Instance()->GetGameState()->SetCurrentPlayer(0);

  m_objectsHitThisShot.clear();
}

void RulesTrickShot::Reset()
{
  m_objectsHitThisShot.clear();
  Rules::Reset();
}

void RulesTrickShot::GivePlayerFoulAward()
{
}

void RulesTrickShot::TrickShotOver()
{
#ifdef _DEBUG
std::cout << "TRICK SHOT RULES: it's over!\n";
#endif

  m_bigText = "";

  if (IsDemoMode())
  {
    Engine::Instance()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
  else
  {
    Engine::Instance()->ChangeState(EngineStatePoolTrickShotOver::Name, 
      Engine::IMMEDIATE);
  }
}

void RulesTrickShot::ThisPlayerWins()
{
  // Award player a bonus of type 2.
  PlayerStats* p = Engine::Instance()->GetGameState()->
    GetCurrentPlayerInfo()->GetPlayerStats();

  PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
  if (pp)
  {
    int levelId = m_pLevel->GetId();
    int roomId = m_pLevel->GetRoomId();
    bool solved = pp->GetRoomSolved(levelId, roomId);
    if (!solved)
    {
      // Add bonus of the given type
      pp->AddBonus(2);
      pp->SetRoomSolved(levelId, roomId, true);
    }
  }
  UpdateStats();

  ((EngineStatePoolTrickShotOver*)(Engine::Instance()->
    GetEngineState(EngineStatePoolTrickShotOver::Name).GetPtr()))->SetSuccess(true);

  TrickShotOver();
}

void RulesTrickShot::ThisPlayerLoses()
{
  ((EngineStatePoolTrickShotOver*)(Engine::Instance()->
    GetEngineState(EngineStatePoolTrickShotOver::Name).GetPtr()))->SetSuccess(false);

  Engine::Instance()->PlayWav("golf-churchbell1.wav");

  TrickShotOver();
}

void RulesTrickShot::BallsHaveStopped()
{
  // Selectively copied from  Rules::BallsHaveStopped();

  if (m_firstBallId == -1 && m_objectsHitThisShot.empty() && m_text.empty())
  {
    m_text = "Foul! ";
    m_text += GetName();
    m_text += " didn't hit any balls.";

    ThisPlayerFouls();
  }

  if (IsShotFoul())
  {
    ThisPlayerLoses();
    return;
  }

  int numBalls = GetNumBallsRemainingForPlayer();
  DecShotCounter();
  if (m_shotsRemaining == 0)
  {
    if (numBalls == 0)
    {
      ThisPlayerWins();
    }
    else
    {
      m_text = "No more shots left.";
      ThisPlayerLoses();
    }
    return;
  }

  if (numBalls == 0)
  {
    // All balls potted 
    ThisPlayerWins();
    return;
  }

  if (m_maxNumShots > 0 && m_shotsRemaining < m_maxNumShots)
  {
    m_text = ToString(m_shotsRemaining);
    m_text += " shot";
    if (m_shotsRemaining > 1)
    {
      m_text += "s";
    }
    m_text += " left";
  }

  ChangeStateToSetUpShot();
}

#if defined(SCENE_EDITOR)
bool RulesTrickShot::Save(File* pf)
{
  if (!pf->WriteComment("// Num shots"))
  {
    return false;
  }
  pf->WriteInteger(m_maxNumShots);
  return true;
}

static const char* NUM_SHOTS = "Num shots";
GameObject::PropertyMap RulesTrickShot::GetProperties() const
{
  PropertyMap pm;
  pm[NUM_SHOTS] = m_maxNumShots;
  return pm;
}

void RulesTrickShot::SetProperties(const PropertyMap& pm)
{
  m_maxNumShots = const_cast<GameObject::PropertyMap&>(pm)[NUM_SHOTS].GetInt();
}
#endif

void RulesTrickShot::BallHitsObject(PoolBall* pBall, PoolGameObject* pObj) 
{
  if (IsCueBall(pBall)) // ??
  { 
    m_objectsHitThisShot.push_back(pObj);
  }
}
}

