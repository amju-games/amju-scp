/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiCollectBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AiCollectBehaviour.h"
#include "Target.h"
#include "CharacterGameObject.h"
#include "PieceMover.h"
#include "Ai.h"
#include "Takeable.h"
#include "AiNotifier.h"

namespace Amju
{
void AiCollectBehaviour::Update()
{
  // There's nothing wrong with the base implementation of Update(),
  // except it won't realise when we have reached the goal!
  if (m_isStillActivating)
  {
    // Not ready yet, must continue to activate.
    // Likely case for this is when we must complete pathfinding.
    ContinueActivating();
    return;
  }

  AiBehaviourBase::Update();

  if (m_isTaskCompleted)
  {
    m_pCharacter->SetForwardAccel(-20.0f); // TODO CONFIG
    m_pCharacter->SetYRotateVel(0);
    return;
  }

  if (m_pTarget->GetBoundingSphere()->Intersects(
    *(m_pCharacter->GetBoundingSphere())))
  {
    ReachedTarget();
    // Don't go on to the next test, as the target's state will have been reset!
    return;
  }

  if (m_pTarget->GetState() != UNKNOWN) 
  {
#if defined(_DEBUG)
      std::cout << "Calling FAILED because piece state has changed.\n";
#endif

    // If someone else gets to the piece first, fail.
    Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);
    pAi->Failed();
  }
}

void AiCollectBehaviour::OnActivated()
{
  // Work out path to piece.
  //Assert(m_pTarget.GetPtr());
  // May be called with no target at Startup :-(
  if (!m_pTarget.GetPtr())
  {
    return;
  }

  m_isTaskCompleted = false;

  // Vel should depend on level, i.e. get faster as game progresses.
  m_vel = 10.0f;
  
  // Split activation over frames, as this may take some time.
  m_isStillActivating = true;

  // The problem with calling CreatePath() is that the Piece we want to aim
  // for will count as obstructing the way to the centre!
  // We can get around this by temporarily disabling the piece.
  // Make a new target, at the x-z position of the piece, with a suitable 
  // height. We make this the m_pTarget temporarily in ContinueActivating().
  if (!m_pInnerTarget.GetPtr())
  {
    m_pInnerTarget = new Target;
  }
  BoundingSphere bs;
  const Orientation* pO = m_pTarget->GetOrientation();
  bs.SetCentre(VertexBase(pO->GetX(), pO->GetY() + 1.0f, pO->GetZ()));
  bs.SetRadius(1.0f); // TODO needed ?
  m_pInnerTarget->SetSphere(bs);
}

void AiCollectBehaviour::ContinueActivating()
{
  Assert(m_pInnerTarget.GetPtr());

  PieceMover* pPm = dynamic_cast<PieceMover*>(m_pCharacter);
  Assert(pPm);

  PGameObject pTemp = m_pTarget; // The piece we want
  State tempState = pTemp->GetState();
  // Temporarily disable the piece, so it won't obstruct the path to its
  // own centre.
  pTemp->SetState(OUT_OF_PLAY);

  // Target held by Behaviour is ref counted, which is exactly what we want
  // for once! The target object will be deleted once we no longer need it.
  SetTarget(m_pInnerTarget.GetPtr());
 
  SearchResult sr = CreatePath();

  // State of piece should be UNKNOWN already.. if it isn't, Update() will fail.
  pTemp->SetState(tempState);
  SetTarget(pTemp);
  
  if (sr == SearchFail)
  {
    m_isStillActivating = false;

#if defined(_DEBUG)
      std::cout << "Calling FAILED because can't find a path to piece.\n";
#endif

    ///pAi->Failed();
    // Try jumping down from current position.
  }

  if (sr == SearchOk)
  {
    m_isStillActivating = false;
  }
}

void AiCollectBehaviour::ReachedTarget()
{
  // This probably won't be called by the base implementation of Update()
  // because the very piece we want is blocking the way to its centre!

  if (m_isTaskCompleted)
  {
    return;
  }

  m_isTaskCompleted = true; // we are done, whether or not we have succeeded.

  PieceMover* pPm = dynamic_cast<PieceMover*>(m_pCharacter);

  // Pick up piece. Then we are finished!
  Assert(pPm);

  Takeable* pTakeable = dynamic_cast<Takeable*>(m_pTarget.GetPtr());
  Assert(pTakeable);
  if (!pPm->ActionTakePiece(pTakeable))
  {
#if defined(_DEBUG)
      std::cout << "Calling FAILED because Take action failed.\n";
#endif

///    pPm->Failed();
    return;
  }
  
  if (m_pTarget->GetState() != TAKEN)
  {
#if defined(_DEBUG)
      std::cout << "Calling FAILED because Take action failed (2)\n";
#endif

///    pPm->Failed();
    return;
  }

  Notifier::Instance()->NotifyPieceTaken(pTakeable, pPm);

  // Picked up piece, so now take it to the destination.
  //m_pCharacter->SetState(AI_TAKE);
}

}
