/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CrocHome.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CrocHome.h"
#include "File.h"
#include "Level.h"
#include "Engine.h"

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateCrocHome() { return new CrocHome; }
static const bool isCrocHomeRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("crochome", CreateCrocHome);
*/

const char* CrocHome::GetTypeName() const
{
  return "crochome";
}

CrocHome::CrocHome() 
{
  m_collided = false;
}

bool CrocHome::Load(File* pf)
{
  if (!Target::Load(pf))
  {
    return false;
  }

  return true;
}

void CrocHome::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
/*  
  if (!pPlayer)
  {
    m_collided = false;
    return;
  }

  if (m_collided)
  {
    return;
  }

  m_collided = true;

  if (m_pLevel->GetPlayerGoalId() == GetId())
  {
    // Player has reached her target! Go to next level.
    // (TODO congratulatory sequence at end of level)
    Engine::Instance()->LevelCompleted();
  }
*/
}

void CrocHome::HandleObjectCollision(GameObject* pObj)
{
  // AI reaching target is handled in the AI code.
  // (Or RunTo Strategy/Behaviour..)
}

}
