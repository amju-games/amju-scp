/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiPlaceBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AiPlaceBehaviour.h"
#include "Target.h"
#include "Ai.h"

namespace Amju
{
void AiPlaceBehaviour::OnActivated()
{ 
  // TODO Vel depends on level/difficulty ?
  m_vel = 10.0f;

  m_isStillActivating = true;

  Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);

  Target* pNewTarget = new Target;
  BoundingSphere bs;
  bs.SetCentre(pAi->GetPiecePos());  
  bs.SetRadius(0.1f); // needed for Reached Target test
  pNewTarget->SetSphere(bs);
  // Target held by Behaviour is ref counted, which is exactly what we want
  // for once! The target object will be deleted once we no longer need it.
  SetTarget(pNewTarget);
}

void AiPlaceBehaviour::ContinueActivating()
{
  SearchResult sr = CreatePath();

  if (sr == SearchFail)
  {
    m_isStillActivating = false;
    Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);

#if defined(_DEBUG)
      std::cout << "Calling FAILED because no path to piece destination.\n";
#endif

    pAi->Failed();
  }
  if (sr == SearchOk)
  {
    m_isStillActivating = false;
  }
}

void AiPlaceBehaviour::ReachedTarget()
{
  // Do nothing - this may be a red herring. We need to be close to the target.
}

void AiPlaceBehaviour::Update()
{
  AiBehaviourBase::Update();

  // Make a small sphere where the character sphere is.
  BoundingSphere bs;
  VertexBase v = m_pCharacter->GetPosition();
  // TODO fiddle with v ?
  v.y += 1.0f; // to match target height
  bs.SetCentre(v);
  bs.SetRadius(0.1f);

/*
#if defined(_DEBUG)
  VertexBase v1 = v;
  VertexBase v2 = m_pTarget->GetBoundingSphere()->GetCentre();

  float sqdist = SquareDist(v1, v2);
  float dist = sqrt(sqdist);
  std::cout << "Place: distance to target (" << v2.x << ", " << v2.y << ", " << v2.z<< "): " << dist;

  // TODO TEMP TEST: is bad y-value to blame ?
  v2.y = v1.y; 
  sqdist = SquareDist(v1, v2);
  dist = sqrt(sqdist);
  std::cout << " with y values equal: " << dist << "\n";

#endif
*/

  if (m_pTarget->GetBoundingSphere()->Intersects(bs))
  {
    // We have got to the spot where we want the piece to go.
    // Rotate so we are facing the required direction.
    m_pCharacter->SetState(AI_ROTATE);
  }
}
}