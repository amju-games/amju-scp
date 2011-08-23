/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyNull.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGY_NULL_H_INCLUDED)
#define SCHMICKEN_STRATEGY_NULL_H_INCLUDED

#include "Strategy.h"

namespace Amju
{
// "do nothing" strategy, for when all else fails; used for testing.
class StrategyNull : public Strategy
{
public:
  StrategyNull(Strategizer* pS);

  virtual bool IsValid() const;
  virtual void OnActivate();
};
}

#endif