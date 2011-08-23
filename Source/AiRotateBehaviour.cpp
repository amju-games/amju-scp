/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiRotateBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AiRotateBehaviour.h"
#include "Ai.h"
#include "AiNotifier.h"
#include "Takeable.h"
#include "AngleCompare.h"

namespace Amju
{
AiRotateBehaviour::AiRotateBehaviour()
{
  m_droppedPiece = false;
}

void AiRotateBehaviour::OnActivated()
{
  m_droppedPiece = false;
}

void AiRotateBehaviour::Update()
{
  Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);

  // Disable test to see if AI is blocked, it won't work here.
  pAi->SetBlocked(false);

  if (m_droppedPiece)
  {
    if (!pAi->IsFalling())
    {
      pAi->Succeeded();
    }
    return;
  }

  // We are in the right position, but may be facing the wrong way.
  // Stop moving
  pAi->SetForwardAccel(0);
  pAi->SetForwardVel(0);

  // Get the direction we are required to face.
  float requiredYrot = pAi->GetPieceYRot();
  float yrot = pAi->GetOrientation()->GetYRot();

  // TODO negative angles etc
  float diff = AngleCompare::DegDiff(yrot, requiredYrot);

  if (fabs(diff) < 10.0f)
  {
    // Got to required angle
    pAi->SetYRotateVel(0);
    //pAi->SetState(AI_DROP);
    // TODO Drop behaviour
    // Try to drop the piece
    pAi->ActionDropPiece();
    if (m_pTarget->GetState() == UNKNOWN) // no longer TAKEN
    {
      Takeable* pTakeable = dynamic_cast<Takeable*>(m_pTarget.GetPtr());
      Notifier::Instance()->NotifyPieceDropped(pTakeable, pAi);

      // We've dropped the piece - and so are now on top of it. The final
      // step is to get down to floor level.
      //pAi->SetState(AI_JUMP_DOWN);

      // Jumping down is part of the next task, because we want to jump in 
      // a sensible direction.
      m_droppedPiece = true;
    }
    else
    {
#if defined(_DEBUG)
      std::cout << "Calling FAILED because Drop action failed.\n";
#endif

      pAi->Failed();
    }
  }
  else
  {
    // Rotate until we get to the required angle.
    if (diff > 0)
    {
      pAi->SetYRotateVel(90.0f); // TODO CONFIG
    }
    else
    {
      pAi->SetYRotateVel(-90.0f); // TODO CONFIG
    }
  }
}
}