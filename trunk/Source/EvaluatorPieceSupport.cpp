/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorPieceSupport.cpp,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorPieceSupport.h"
#include "TakeableCrate.h"
#include "Finder.h"
#include "VisibilityGraphWithObjects.h"
#include "CubeMap.h"

namespace Amju
{
EvaluatorPieceSupport::EvaluatorPieceSupport()
{
#if defined(_DEBUG)
  SetName("Punishment for moving a piece to an unsupported position");
#endif
}

Evaluator::EvalResult EvaluatorPieceSupport::Score(
  const RankedMove& rm, 
  Ai* pAi, 
  VisibilityGraphWithObjects* pVgo,
  float* pResult)
{
  // Make sure that piece moved to new location would be fully supported.
  // Happily this excludes pieces from sitting on top of ramps.
 
  // Score is -1 for a piece not properly supported;
  // 0 for a properly supported piece.
  // I.e. this is a 'punishment' score.

  TakeableCrate* pCrate = rm.m_pCrate;
  Orientation savedOr = *(pCrate->Takeable::GetOrientation());
  State savedState = pCrate->Takeable::GetState();
  pCrate->Takeable::SetState(UNKNOWN);

  // Change the orientation to BELOW the piece.
  // Try the new orientation. Get the new bounding box and check if it
  // intersects the cube map. If it does, then something is underneath
  // where we want the piece to go.
  Orientation o = rm.m_or;
  //o.SetY(o.GetY() - 2.0f);
  //pCrate->SetOrientation(o);
  
  BoundingBox bb = pCrate->Takeable::GetCollisionVol()->GetBoundingBox();
  VertexBase vAfter = o.GetVertex();
//  vAfter.y -= 1.0f;  // make the destination height adjustment
  Finder::BoundingBoxTransform(&bb, savedOr.GetVertex(), vAfter, savedOr.GetYRot(), o.GetYRot());

  // Check each cube in the top layer of bb.
  bb.m_ymax -= 2.0f;
  bb.m_ymin = bb.m_ymax - 1.0f;
  *pResult = -1.0f; 
  if (pVgo->GetCubeMap().IsBoxFullyOccupied(bb))
  {
    *pResult = 0;
  }

  pCrate->Takeable::SetState(savedState);
  //pCrate->SetOrientation(savedOr);

  return EvalOk;
}
}
