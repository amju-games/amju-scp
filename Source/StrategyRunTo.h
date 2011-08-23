/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyRunTo.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGY_RUN_TO_H_INCLUDED)
#define SCHMICKEN_STRATEGY_RUN_TO_H_INCLUDED

#include "Strategy.h"

namespace Amju
{
// This kind of strategy tells its AI to run to the goal.
class StrategyRunTo : public Strategy
{
public:
  StrategyRunTo(Strategizer* pS);

  virtual bool IsValid() const;
  virtual void OnActivate();
  virtual void Reset();
  virtual void OnTaskFinished();
  virtual void OnTaskFailed();
  virtual void RecalculatePath();

protected:
  // This flag is true if there is a path to the target (m_pTarget),
  // or there MAY be (i.e. result is pending). 
  bool m_isPath;
};
}

#endif