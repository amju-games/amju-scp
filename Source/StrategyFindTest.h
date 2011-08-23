/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyFindTest.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGY_FIND_TEST_H_INCLUDED)
#define SCHMICKEN_STRATEGY_FIND_TEST_H_INCLUDED

#include "StrategyFindBuild.h"

namespace Amju
{
// Tests the Finder by immediately making moves, rather than moving the AI
// around. This separates the Finder from the Character testing.
class StrategyFindTest : public StrategyFindBuild
{
public:
  StrategyFindTest(Strategizer* pS);
  virtual void OnActivate();
  virtual void RecalculatePath();
};
}

#endif