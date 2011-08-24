/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorBlockRamp.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorBlockRamp.h"
#include "TakeableCrate.h"
#include "BoundingBox.h"
#include "Finder.h"
#include "Ai.h"
#include "Engine.h"
#include "Level.h"

namespace Amju
{
EvaluatorBlockRamp::EvaluatorBlockRamp()
{
#if defined(_DEBUG)
  SetName("Punishment for blocking a ramp");
#endif
}

Evaluator::EvalResult EvaluatorBlockRamp::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  *pResult = 0;

  // Ramp Blocking possibilities
  // ---------------------------
  // 1. Place a piece at top or bottom of ramp
  // 2. Place a ramp in a position where its top or bottom is blocked
  // 3. Remove a piece which currently blocks a ramp top or bottom.

  // TODO We need another evaluator to test no.2.

  // Does this piece block a ramp ?
  // A ramp is blocked by a piece which occupies space up to 2 units above it,
  // or the 2-units-cube at the top or bottom.
  //
  //
  //    XXXX
  //  ----   X
  //       \   X 
  //         \   X  
  //           \   X
  //             \   XXXX
  //               -------
  //
  // We don't worry about pieces which block the diagonal part, as this
  // should be taken care of by the evaluator which scores for putting
  // piece on a flat surface, i.e. it is completely supported.
  // So we only have to check for the top and bottom areas of a ramp.
  // Create the bounding box for this move. Then check if this intersects
  // any 'ramp-blocking' bounding boxes, which we can precalculate.
  TakeableCrate* pCrate = rm.m_pCrate;
  Orientation savedOr = *(pCrate->Takeable::GetOrientation());
  State savedState = pCrate->Takeable::GetState();
  pCrate->Takeable::SetState(UNKNOWN);
  Orientation o = rm.m_or;
  BoundingBox bb = pCrate->Takeable::GetCollisionVol()->GetBoundingBox();
  
  // Check if the piece blocks a ramp before moving it - see below
  bool originalBlock = BlocksRamp(bb);

  VertexBase vAfter = o.GetVertex();
  Finder::BoundingBoxTransform(&bb, savedOr.GetVertex(), vAfter, savedOr.GetYRot(), o.GetYRot());
  // Now bb is the bounding box for this move.
  pCrate->Takeable::SetState(savedState);

  // Check bb against all ramp blocker boxes.
  if (BlocksRamp(bb))
  {
    *pResult = -1.0f;
  }
  // Ok, piece destination doesn't block a ramp.
  // Check if its original position blocked one - if so we should reward this 
  // move.
  else if (originalBlock)
  {
    *pResult = 1.0f;
  }
  else
  {
    *pResult = 0;
  }

  return EvalOk;
}

bool EvaluatorBlockRamp::BlocksRamp(const BoundingBox& bb)
{
  for (Boxes::iterator it = m_boxes.begin(); it != m_boxes.end(); ++it)
  {
    if (bb.Intersects(*it))
    {
      return true;
    }
  }
  return false;
}

void EvaluatorBlockRamp::Reset(Finder* pFinder)
{
  // Rebuild list of bounding boxes for ramp-blocking positions.
  m_boxes.clear();

  int levelId = pFinder->GetAi()->GetLevel()->GetId();
  int roomId =  pFinder->GetAi()->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the AI.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    // Consider the piece if it's a TakeableCrate and intersects the b.sphere.
    TakeableCrate* pTakeableCrate = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTakeableCrate && 
        pTakeableCrate->Takeable::GetState() == UNKNOWN &&
        pTakeableCrate->GetType() == "ramp2_2_4.comp"
        //pTakeableCrate->GetBoundingSphere()->Intersects(bs) 
       )
    {
      const Orientation& o = *(pTakeableCrate->Takeable::GetOrientation());
      // Found a ramp. Work out where the top and bottom are.
      // Create bottom box w.r.t. a ramp at origin.
      BoundingBox bottom(-1.0f, 1.0f, 0, 2.0f, 2.0f, 4.0f);
      // Rotate this b.box about ramp origin
      bottom.RotateY(o.GetYRot());
      // Translate to ramp location
      bottom.Translate(o.GetX(), o.GetY(), o.GetZ());
      m_boxes.push_back(bottom);

      BoundingBox top(-1.0f, 1.0f, 2.0f, 4.0f, -4.0f, -2.0f);
      // Rotate this b.box about ramp origin
      top.RotateY(o.GetYRot());
      // Translate to ramp location
      top.Translate(o.GetX(), o.GetY(), o.GetZ());
      m_boxes.push_back(top);
    }
  }

}

}
