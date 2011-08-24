/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyle.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyle.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "PieceMover.h"
#include "Takeable.h"
#include "Player.h"

namespace Amju
{
EngineRunning* ControlStyle::ER()
{
  static EngineRunning* pEr = 0;
  if (!pEr)
  {
    pEr = dynamic_cast<EngineRunning*>(
      Engine::Instance()->GetEngineState(EngineRunning::Name).GetPtr());
  }
  return pEr;
}

void ControlStyle::DoAction(bool down)
{
  // ACTION button.
  // Currently the possible actions are as follows:
  //  - drop the takeable being carried
  //  - rotate the takeable just dropped
  //  - pick up a takeable
  //  - jump, if no takeable is near enough

  if (!down)
  {
    return;
  }

  // What is the appropriate Action ?
  PieceMover::Action a = Engine::Instance()->GetPlayer()->GetCurrentAction();

  if (a == PieceMover::ROTATE)
  {
    // Get the orientation before rotating. We send a 'taken' event for this
    // orientation, then a 'dropped' event at the new orientation.
    //Orientation before = *(pTakeable->GetOrientation());

    if (Engine::Instance()->GetPlayer()->ActionRotatePiece())
    {
      //Orientation after = *(pTakeable->GetOrientation());
      //pTakeable->SetOrientation(before);
      //Notifier::Instance()->NotifyPieceTaken(pTakeable, Engine::Instance()->GetPlayer());
      //pTakeable->SetOrientation(after);
      //Notifier::Instance()->NotifyPieceDropped(pTakeable, Engine::Instance()->GetPlayer());
    }
    return;
  }

  if (a == PieceMover::DROP)
  {
    if (Engine::Instance()->GetPlayer()->ActionDropPiece())
    {
      //Takeable* pTakeable = Engine::Instance()->GetPlayer()->GetCurrentPiece();
      //Notifier::Instance()->NotifyPieceDropped(pTakeable, Engine::Instance()->GetPlayer());

      //Assert(Engine::Instance()->GetPlayer()->GetCooperationGroup());
      //Engine::Instance()->GetPlayer()->GetCooperationGroup()->SetPieceUsed(pTakeable->GetId(), true);
    }
    return;
  }

  if (a == PieceMover::TAKE)
  {
    if (Engine::Instance()->GetPlayer()->ActionTakePiece())
    {
      //Takeable* pTakeable = Engine::Instance()->GetPlayer()->GetCurrentPiece();
      //Notifier::Instance()->NotifyPieceTaken(pTakeable, Engine::Instance()->GetPlayer());
    }
    return;
  }
}
}

