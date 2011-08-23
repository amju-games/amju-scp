/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiRunToBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AiRunToBehaviour.h"
#include "Ai.h"

namespace Amju
{
void AiRunToBehaviour::Update()
{
  Assert(m_pTarget.GetPtr());
  AiBehaviourBase::Update();

  if (m_isTaskCompleted)
  {
    m_pCharacter->SetForwardAccel(-20.0f); // TODO CONFIG
    m_pCharacter->SetYRotateVel(0);
    return;
  }
}

void AiRunToBehaviour::OnActivated()
{
  m_isTaskCompleted = false;

  if (!m_pTarget.GetPtr())
  {
    m_isTaskCompleted = true;
    return;
  }
  m_isStillActivating = true;
}

void AiRunToBehaviour::ReachedTarget()
{
  if (m_isTaskCompleted)
  {
    return;
  }

  m_isTaskCompleted = true; // we are done, whether or not we have succeeded.

/*
  Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);
  Assert(pAi);
  pAi->Succeeded();
*/
}

void AiRunToBehaviour::ContinueActivating()
{
  // Is there a path to the goal ?
  Assert(m_pTarget.GetPtr());
  SearchResult isPathResult = CreatePath();

  Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);
  Assert(pAi);

  if (isPathResult == SearchOk) 
  {
    m_isStillActivating = false;
    m_vel = 5.0f; // TODO CONFIG
  }
  else if (isPathResult == SearchFail)
  {
#if defined(_DEBUG)
    std::cout << "Calling FAILED because no path to goal.\n";
#endif

    pAi->Failed();
  }
  // Pending => no state change
}
}
