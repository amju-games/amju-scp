/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Objective.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Objective.h"
#include "File.h"
#include "Engine.h"
#include "GameState.h"

namespace Amju
{
bool Objective::Load(File* pf)
{
  // The objective is represented by a list of <game object ID, state ID> 
  // pairs. A pair evaluates to true if the specifed game object has the
  // state specfied. If all pairs are true, the level is complete.

  // First thing is a function. This tells us whether all the pairs must
  // be satisfied, or any of them, or what.
  std::string funcName;
  pf->GetDataLine(&funcName);
  if (funcName == "and")
  {
    m_op = AND;
  }
  else if (funcName == "or")
  {
    m_op = OR;
  }
  else
  {
    pf->ReportError("Expected operator, e.g. \"and\".");
    return false;
  }

  int numPairs = 0;
  if (!pf->GetInteger(&numPairs))
  {
    pf->ReportError("Expected number of objective pairs.");
    return false;
  }
  for (int i = 0; i < numPairs; i++)
  {
    int gameObjId = 0;
    int stateId = 0;
    if (!pf->GetInteger(&gameObjId))
    {
      pf->ReportError("Expected game object ID in objective.");
      return false;
    }
    if (!pf->GetInteger(&stateId))
    {
      pf->ReportError("Expected state ID in objective.");
      return false;
    }
    
    ObjectivePair p(gameObjId, (State)stateId);
    this->push_back(p);
  }

  return true;
}

bool Objective::Save(File* pf)
{
  pf->WriteComment("// Objective: operator");
  if (m_op == AND)
  {
    pf->Write("and");
  }
  else if (m_op == OR)
  {
    pf->Write("or");
  }
  pf->WriteComment("// Objective: no of pairs, then (game id, state) pairs");
  pf->WriteInteger(size());
  for (int i = 0; i < size(); i++)
  {
    ObjectivePair& p = (*this)[i];
    pf->WriteInteger(p.first);
    pf->WriteInteger(p.second);
  }
  return true;
}

bool Objective::IsSatisfied() const
{
  GameState* pGs = Engine::Instance()->GetGameState();
  // Iterate over the objective pairs, and test each one.
  for (Objective::const_iterator it = this->begin();
       it != this->end(); 
       ++it)
  {
    const ObjectivePair& p = *it;
    const GameObjectId& gameObjId = p.first;
    const State& requiredState = p.second;
    State s = Engine::Instance()->GetGameObject(gameObjId)->GetState();

    if (m_op == AND && s != requiredState)
    {
      return false;
    }
    if (m_op == OR && s == requiredState)
    {
      return true;
    }
  }
  return true;
}
}

