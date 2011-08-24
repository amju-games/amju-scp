/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Ai.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Ai.h"
#include "AiCollectBehaviour.h"
#include "AiPlaceBehaviour.h"
#include "AiRotateBehaviour.h"
#include "AiJumpDownBehaviour.h"
#include "AiRunToBehaviour.h"
#include "StopBehaviour.h"
#include "Engine.h"
#include "Target.h"
#include "AiNotifier.h"
#include "Takeable.h"
#include "CooperationGroupManager.h"
#include "Level.h"
#include "Finder.h"
#include "TakeableCrate.h"
#include "EngineRunningVisGraph.h"
#include "VisibilityGraphWithObjects.h"
#include "StateStrings.h"

namespace Amju
{
Ai::Ai()
{
  m_pStrategizer = 0;

  SetPMCharacter(this);

  // Attempt at cleaning up behaviours using a map
  // TODO no advantage - just use an array with const int indexes.

  Behaviour* pBehaviour = new AiCollectBehaviour;
  pBehaviour->SetNpc(this);
  pBehaviour->SetTarget(0);
  m_behaviours["collect"] = pBehaviour;

  pBehaviour = new AiPlaceBehaviour;
  pBehaviour->SetNpc(this);
  pBehaviour->SetTarget(0);
  m_behaviours["place"] = pBehaviour;

  pBehaviour = new AiRotateBehaviour;
  pBehaviour->SetNpc(this);
  pBehaviour->SetTarget(0);
  m_behaviours["rotate"] = pBehaviour;

  pBehaviour = new AiJumpDownBehaviour;
  pBehaviour->SetNpc(this);
  pBehaviour->SetTarget(0);
  m_behaviours["jump"] = pBehaviour;

  pBehaviour = new StopBehaviour;
  pBehaviour->SetNpc(this);
  pBehaviour->SetTarget(0);
  m_behaviours["stop"] = pBehaviour;

  pBehaviour = new AiRunToBehaviour;
  pBehaviour->SetNpc(this);
  pBehaviour->SetTarget(0);
  m_behaviours["runto"] = pBehaviour;

#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
//  m_charName = "player";
#endif

  m_pieceId = 10001; // TODO TEMP TEST
  m_pieceYRot = 0;
  m_piecePlace = VertexBase(-50, 1, 50);

  Notifier::Instance()->Register(this);

  PieceMover::LoadExplosion();
}

const char* Ai::GetTypeName() const
{
  return "ai";
}

bool Ai::Load(File* pf)
{
  if (!Npc::Load(pf))
  {
    return false;
  }
  if (!LoadGoalList(pf))
  {
    return false;
  }

  m_pStrategizer = new Strategizer;

  if (!m_pStrategizer->Load(0))
  {
    return false;
  }

  m_pStrategizer->SetAi(this);

  File f2;
  // TODO This filename can depend on the type of AI, i.e. different
  // kinds of AI have different rules, and so make different structures.
  f2.OpenRead("findbuild.txt");

  if (!LoadFinders(&f2))
  {
    return false;
  }

  return true;
}

bool Ai::LoadFinders(File* pf)
{
  int numFinders = 0;
  if (!pf->GetInteger(&numFinders))
  {
    pf->ReportError("Expected number of finders.");
    return false;
  }
  // Load each finder.
  for (int i = 0; i < numFinders; i++)
  {
    // Get height for this finder
    int height = 0;
    if (!pf->GetInteger(&height))
    {
      pf->ReportError("Expected height for finder.");
      return false;
    }
    Finder* pFinder = new Finder;
    if (!pFinder->Load(pf))
    {
      return false;
    }
    pFinder->SetHeight(height);
    pFinder->SetAi(this);
    m_finders[height] = pFinder;
  }

  return true;
}

bool Ai::LoadGoalList(File* pf)
{
  /*
  // Decide on Goal object. This should depend on
  // - the level definition
  // - what kind of AI this is
  // etc.
  // TODO TEMP TEST
  // For now, always set it to the Croc Home we know about.
  PGameObject pGo = Engine::Instance()->GetGameObject(10201); // Croc home ID
  m_goals.push_back(pGo);
  m_goalIndex = 0;
  */

  int numGoals = 0;
  if (!pf->GetInteger(&numGoals))
  {
    pf->ReportError("Expected number of goals.");
    return false;
  }
  for (int i = 0; i < numGoals; i++)
  {
    int id = 0;
    if (!pf->GetInteger(&id))
    {
      pf->ReportError("Expected goal ID.");
      return false;
    }
    PGameObject pGo = Engine::Instance()->GetGameObject(id); 
    m_goals.push_back(pGo);
  }

  m_goalIndex = 0;

  return true;
}

PGameObject Ai::GetCurrentGoal()
{
  Assert(!m_goals.empty());
  Assert(m_goalIndex < (int)m_goals.size());
  return m_goals[m_goalIndex];
}

PGameObject Ai::GetPrevGoal()
{
  Assert(!m_goals.empty());
  Assert(m_goalIndex < (int)m_goals.size());

  if (m_goalIndex == 0)
  {
    return 0;
  }
  return m_goals[m_goalIndex - 1];
}

void Ai::PlayerLoseTest()
{
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
    
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    Ai* pAi = dynamic_cast<Ai*>(pGo.GetPtr());
    if (pAi)
    {
      State s = pGo->GetState();
      if (s != OUT_OF_PLAY)
      {
        // If any AI is still in play, the player has not lost.
        return;
      }
    }
  }

  // Player is last character to reach home - game over.
  Engine::Instance()->GameOver();
}

void Ai::ReachedHomeEffect()
{
  // TODO sadly not this easy, as the AI is set to OUT_OF_PLAY.
  // So this explosion effect is not drawn :-(
  m_explosion[0].Reset(GetPosition());
  m_explosion[1].Reset(GetPosition());
  m_explosion[2].Reset(GetPosition());
  m_explosion[3].Reset(GetPosition());

}

void Ai::CurrentGoalReached()
{
  // If there is another target to get to, set this as the current target.
  // If there are no more targets, set the AI to OUT_OF_PLAY.
  // If there are no active AIs left, then the player loses.
  m_goalIndex++;
  if (m_goalIndex >= (int)m_goals.size())
  {
    ReachedHomeEffect();

    SetState(OUT_OF_PLAY);
    // Check for remaining active AIs
    PlayerLoseTest();
    m_goalIndex = 0; // yuck, shouldn't need this.
  }
  else
  {
    // There is another goal to get to.
    SetTarget(m_goals[m_goalIndex].GetPtr());
  }
}

void Ai::Update()
{
  Npc::Update();
  UpdatePieceFootprint();

  // Detect if the AI has got itself blocked behind a wall.
  // Get number of frames AI has been blocked
  if (GetBlocked() > 10) // TODO CONFIG
  {
    //SetBlocked(false);
    //Failed();

    // Check state, it may be a rotating or stopped/frozen state,
    // If we expect to be moving, call Fail callback.
    State s = GetState();
    if (s == AI_TAKE ||
        s == AI_COLLECT ||
        s == AI_DUMP) // TODO more states ?
    {
#if defined(_DEBUG)
      std::cout << "Calling FAILED because AI is BLOCKED!\n";
#endif
      SetBlocked(false);
      Failed();
    }
    else if (s == AI_JUMP_DOWN)
    {
#if defined(_DEBUG)
      std::cout << "AI is BLOCKED in jump down state. See Ai::Update\n";
#endif
      // It would probably be good to go to a "Wander" state now.
    }
    else
    {
#if defined(_DEBUG)
      std::cout << "AI is BLOCKED but no action is being taken! See Ai::Update\n";
#endif
    }

  }
}

void Ai::Draw()
{
  Npc::Draw();
  DrawTakenPiece();
  DrawPieceFootprint();
}

void Ai::SetTarget(GameObject* pGo)
{
  Assert(pGo);

  for (BehaviourMap::iterator it = m_behaviours.begin(); 
       it != m_behaviours.end(); 
       ++it)
  {
    it->second->SetTarget(pGo); 
  }
/*
  m_behaviours["rotate"]->SetTarget(pGo);
  m_behaviours["collect"]->SetTarget(pGo);
  m_behaviours["place"]->SetTarget(pGo);
  m_behaviours["runto"]->SetTarget(pGo);
  m_behaviours["jump"]->SetTarget(pGo);
*/
}

void Ai::OnRoomEntry()
{
  Npc::OnRoomEntry();

  // This looks like a good time to join a Cooperation Group.
  // Join the group with the same name as the character - so
  // all characters which look the same work together.
  // TODO clear from any previously joined group
  const std::string& groupName = "name"; // TODO - all in same group!
  CooperationGroupManager::Instance()->JoinOrCreateGroup(this, groupName);

  m_goalIndex = 0; // reset to first goal 
  SetState(UNKNOWN);

  // TODO reset all Strategies ?

  ChooseStrategy();
}

void Ai::SetPieceId(int id)
{
  m_pieceId = id;
  PGameObject pGo = Engine::Instance()->GetGameObject(m_pieceId);
  SetTarget(pGo.GetPtr());
}

void Ai::SetPiecePos(const VertexBase& v)
{
  m_piecePlace = v;
}

void Ai::SetPieceYRot(float yrot)
{
  m_pieceYRot = yrot;
}

void Ai::SetState(State newState)
{
//#if defined(_DEBUG)
  std::cout << "Getting state change message: " << StateStrings[newState] << std::endl;
//#endif
  
  // TODO nice FSM!

  State current = GetState();
  /*
  if (current == newState)
  {
    return;
  }
  */
  if (newState == AI_TAKE)
  {
    SetBehaviour(m_behaviours["place"]);
  }
  else if (newState == AI_COLLECT) //// && current == UNKNOWN)
  {
    SetBehaviour(m_behaviours["collect"]);
  }
  else if (newState == AI_ROTATE && current == AI_TAKE)
  {
    SetBehaviour(m_behaviours["rotate"]);
  }
  else if (newState == AI_JUMP_DOWN) //// && current == AI_ROTATE)
  {
    SetBehaviour(m_behaviours["jump"]);
  }
  else if (newState == UNKNOWN)
  {
    SetBehaviour(m_behaviours["stop"]);
/*
    // Send message to change state soon
    const int id = GetId(); // used when sending messages
    Engine::Instance()->SendGameMessage(new Message(
      "decide", 1.0f, id, id, DECIDE));
*/
  }
  else if (newState == AI_DUMP)
  {
    SetBehaviour(m_behaviours["place"]);
  }
  else if (newState == AI_ROTATE && current == AI_DUMP)
  {
    newState = AI_ROTATE_FAIL;
    SetBehaviour(m_behaviours["rotate"]);
  }
/*
  else if (newState == AI_JUMP_DOWN && current == AI_ROTATE_FAIL)
  {
    newState = AI_JUMP_FAIL;
    SetBehaviour(m_behaviours["jump"]);
  }
*/
  else if (newState == AI_RUN_TO)
  {
    m_behaviours["runto"]->SetTarget(GetCurrentGoal());
    SetBehaviour(m_behaviours["runto"]);
  }
  else if (newState == DECIDE && current == UNKNOWN)
  {
    ChooseStrategy();
    return;
  }

  Npc::SetState(newState);

}

Finder* Ai::GetFinder(int iheight)
{
  Finder* pFinder = 0;

  if (m_finders.find(iheight) == m_finders.end())
  {
    // No finder for the height :-(
    // Choose highest available - i.e. get the final item.
    Finders::reverse_iterator it = m_finders.rbegin();
    // If this asserts, there are no Finders at all.
    Assert(it != m_finders.rend());
    pFinder = it->second;
  }
  else
  {
    // Finder exists for the given height.
    pFinder = m_finders[iheight];
  }

  return pFinder;
}

VertexBase Ai::GetDumpLocation()
{
  // Get the Finder Strategy, then get its Finder.
  // Hmm. Maybe the finder should be a member of this, and StrategyFindBuild
  // points to it.

  // Get a finder.
  Finder* pFinder = GetFinder(0); // zero because we don't need to scale a height. 

  PGameObject pGo = Engine::Instance()->GetGameObject(m_pieceId);
  TakeableCrate* pCrate = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
  Assert(pCrate);

  // Get vis. graph
  VisibilityGraph* pVg = EngineRunningVisGraph::GetVisibilityGraph().GetPtr();
  VisibilityGraphWithObjects* pVgwo = dynamic_cast<VisibilityGraphWithObjects*>(pVg);
  Assert(pVgwo);

  Finder::RankedMoveList rml = pFinder->GetPossibleSeedPositions(pCrate, pVgwo);

  // Iterate through the seed moves until a reachable one is found.
  // Introduce some randomness so we don't try to dump at a bad location,
  // so decide to dump the piece, but choose the same location..... 
  // and get stuck in a loop.
  while (true) // TODO counter, for safety
  {
    for (Finder::RankedMoveList::iterator it = rml.begin(); it != rml.end(); ++it)
    {
      float random = (float)rand() / (float)RAND_MAX;
      if (random > 0.9f) // TODO CONFIG
      {
        const RankedMove& rm = *it;
        // Is the location reachable from current AI position ?
        if (pFinder->IsReachable(rm)) 
        {
          VertexBase v = rm.m_or.GetVertex();
          v.y += 1.0f; // To match other target heights

#if defined(_DEBUG)
          std::cout << "Dump location: " << v << "\n";
#endif
          return v;
        }
      }
    }
  }
 
  Assert(0);
  return VertexBase(0, 0, 0);
}

void Ai::Failed()
{
//#if defined(_DEBUG)
  std::cout << "AI FAILED :-( Current State: " 
            << StateStrings[GetState()] << std::endl;
//#endif

  if (IsCarryingPiece())
  {
    // This task has gone wrong, but we have picked up the piece.
    // We must dump it somewhere before we can continue.
    // Find a nice spot to dump the piece.

    // Set the position where we want to dump the piece
    SetPiecePos(GetDumpLocation());

    SetState(AI_DUMP); 

    // We dumped this piece, so mark it as unused for the cooperation group.
    // NB This line moved from below, which looked like a mistake.
    GetCooperationGroup()->SetPieceUsed(m_pieceId, true);

    return;
  }

  // We dumped this piece, so mark it as unused for the cooperation group.
  //GetCooperationGroup()->SetPieceUsed(m_pieceId, true);

  m_pStrategizer->OnTaskFailed();

  ChooseStrategy();
}

void Ai::Succeeded()
{
  if (GetState() == AI_ROTATE_FAIL) // was AI_JUMP_FAIL, but this now happens
                                    // at the start of a task, not the end.
  {
#if defined(_DEBUG)
      std::cout << "Calling FAILED because AI has finished dumping a piece\n";
#endif

    Failed();
    return;
  }

  Assert(!IsCarryingPiece());

  // Successfully placed a piece - add it to the list of pieces which the
  // cooperation group knows about.
  GetCooperationGroup()->SetPieceUsed(m_pieceId, true);

  m_pStrategizer->OnTaskFinished();

  ChooseStrategy();
}

void Ai::ChooseStrategy()
{
  if (GetState() == OUT_OF_PLAY)
  {
    return;
  }
  m_pStrategizer->SetGoal(GetCurrentGoal());
  m_pStrategizer->ChooseStrategy();
}

const VertexBase& Ai::GetPiecePos() const
{
  return m_piecePlace;
}

float Ai::GetPieceYRot() const
{
  return m_pieceYRot;
}

int Ai::GetPieceId() const
{
  return m_pieceId;
}

void Ai::NotifyPieceTaken(Takeable* pTakeable, bool self)
{
  if (!self)
  {
    if (pTakeable->GetId() == m_pieceId &&
        GetState() == AI_COLLECT)
    {
      // We were on our way to collect a piece which has just been taken by
      // someone else! Gahhh!
      RefreshStrategies(); // TODO is this right ??

#if defined(_DEBUG)
      std::cout << "Calling FAILED because target piece has been taken.\n";
#endif

      Failed();
      return;
    }

    // TODO The piece which was taken may have formed a path to our target.
    // So we must recalculate the path.
    m_pStrategizer->RecalculatePath();

    // See if the piece belonged to something we were building.
    // Maybe we need to fix it.
    // Don't worry about that, the 'Finder' finds which piece we should
    // put where next.
  }

  RefreshStrategies();
}

void Ai::NotifyPieceDropped(Takeable* pTakeable, bool self)
{
  // A piece has been dropped. We should recalculate the path we are on
  // because the piece may be in the way.

  if (!self)
  {
    m_pStrategizer->RecalculatePath();
  }

  RefreshStrategies();
}

void Ai::NotifyPieceRotated(Takeable* pTakeable, bool self)
{
  if (!self)
  {
    m_pStrategizer->RecalculatePath();
  }

  RefreshStrategies();
}

void Ai::RefreshStrategies()
{
  m_pStrategizer->RefreshStrategies();
}

int Ai::GetIdealNumPiecesAtHeight(float diff)
{
  // TODO CONFIG for this eqn.
  int idealPieces = (int)(diff / 2.0f + 2.0f) + 1;
  return idealPieces;
}

}

