/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorBlockRamp.h,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if !defined(EVALUATOR_BLOCK_RAMP_H_INCLUDED)
#define EVALUATOR_BLOCK_RAMP_H_INCLUDED

#include <vector>
#include "Evaluator.h"
#include "BoundingBox.h"

namespace Amju
{
class EvaluatorBlockRamp : public Evaluator
{
public:
  EvaluatorBlockRamp();

  virtual EvalResult Score(
    const RankedMove& rm, 
    Ai* pAi, 
    VisibilityGraphWithObjects* pVgo,
    float* pResult);

  virtual void Reset(Finder*);

protected:
  bool BlocksRamp(const BoundingBox& bb);

protected:
  // Collection of boxes in positions which block ramps.
  // This list is rebuilt in Reset().
  typedef std::vector<BoundingBox> Boxes;
  Boxes m_boxes;
};
}

#endif
