/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorFactory.cpp,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EvaluatorFactory.h"
#include "EvaluatorHeight.h"
#include "EvaluatorDistanceFromGoal.h"
#include "EvaluatorDistanceFromAi.h"
#include "EvaluatorNumberAtHeight.h"
#include "EvaluatorPieceInUse.h"
#include "EvaluatorPieceSupport.h"
#include "EvaluatorAddRamp.h"
#include "EvaluatorRampFacing.h"
#include "EvaluatorNearPrevious.h"
#include "EvaluatorBlockRamp.h"

namespace Amju
{
Evaluator* CreateEvalHeight()
{
  return new EvaluatorHeight;
}

Evaluator* CreateEvalGoalDistance()
{
  return new EvaluatorDistanceFromGoal;
}

Evaluator* CreateEvalAiDistance()
{
  return new EvaluatorDistanceFromAi;
}

Evaluator* CreateEvalNumber()
{
  return new EvaluatorNumberAtHeight;
}

Evaluator* CreateEvalPieceInUse()
{
  return new EvaluatorPieceInUse;
}

Evaluator* CreateEvalPieceSupport()
{
  return new EvaluatorPieceSupport;
}

Evaluator* CreateEvalAddRamp()
{
  return new EvaluatorAddRamp;
}

Evaluator* CreateEvalRampFacing()
{
  return new EvaluatorRampFacing;
}

Evaluator* CreateEvalNearPrevious()
{
  return new EvaluatorNearPrevious;
}

Evaluator* CreateEvalBlockRamp()
{
  return new EvaluatorBlockRamp;
}

SingleEvaluatorFactory::SingleEvaluatorFactory() 
{
  RegisterFactoryFunction("height", &CreateEvalHeight);
  RegisterFactoryFunction("goal_distance", &CreateEvalGoalDistance);
  RegisterFactoryFunction("ai_distance", &CreateEvalAiDistance);
  RegisterFactoryFunction("number", &CreateEvalNumber);
  RegisterFactoryFunction("in_use", &CreateEvalPieceInUse);
  RegisterFactoryFunction("support", &CreateEvalPieceSupport);
  RegisterFactoryFunction("add_ramp", &CreateEvalAddRamp);
  RegisterFactoryFunction("ramp_facing", &CreateEvalRampFacing);
  RegisterFactoryFunction("near_prev", &CreateEvalNearPrevious);
  RegisterFactoryFunction("block_ramp", &CreateEvalBlockRamp);
}
}