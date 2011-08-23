/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EvaluatorFactory.h,v $
Revision 1.1  2004/09/08 15:43:02  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_EVALUATOR_FACTORY_H_INCLUDED)
#define SCHMICKEN_EVALUATOR_FACTORY_H_INCLUDED

#include "Factory.h"
#include "Singleton.h"
#include "Evaluator.h"

namespace Amju
{
class SingleEvaluatorFactory : public Factory<Evaluator> 
{
public:
  SingleEvaluatorFactory();
};

typedef Singleton<SingleEvaluatorFactory> EvaluatorFactory;
}
#endif