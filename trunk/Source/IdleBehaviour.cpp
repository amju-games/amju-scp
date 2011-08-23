/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: IdleBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "IdleBehaviour.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "Level.h"
#include "Engine.h"
#include "Chick.h"
#include "SchAssert.h"

namespace Amju
{
IdleBehaviour::IdleBehaviour()
{
  m_decideBehaviour.Init();
  m_decideBehaviour.SetUpdatePeriod(3.0f); // TODO CONFIG
}

void IdleBehaviour::Update()
{
  Assert(m_pCharacter);
  // Decelerate to a standstill.
  if (m_pCharacter->GetForwardVel() > 0)
  {
    m_pCharacter->SetForwardAccel(-1.0f); // TODO CONFIG
  }
  if (fabs(m_pCharacter->GetYRotateVel()) > 0)
  {
    m_pCharacter->SetYRotateVel(0);
  }

  // If some condition is met, start walking in a random direction.
  // TODO

  // Periodically check to see if we should change state.
  m_decideBehaviour.SetTarget(m_pTarget);
  m_decideBehaviour.SetNpc(m_pCharacter);
  m_decideBehaviour.Update();
}
}

