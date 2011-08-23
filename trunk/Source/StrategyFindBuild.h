/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: StrategyFindBuild.h,v $
Revision 1.1  2004/09/08 15:43:03  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_STRATEGY_FIND_BUILD_H_INCLUDED)
#define SCHMICKEN_STRATEGY_FIND_BUILD_H_INCLUDED

#include <map>
#include <set>
#include <vector>
#include "Strategy.h"
#include "Finder.h"

namespace Amju
{
class File;
class TakeableCrate;

class StrategyFindBuild : public Strategy
{
public:
  StrategyFindBuild(Strategizer* pS);

  virtual bool IsValid() const;
  virtual void OnActivate();
  virtual void OnTaskFinished();
  virtual void OnTaskFailed();
  virtual bool Load(File*); 
  virtual void Reset();
  virtual void RecalculatePath();

protected:
  void SelectFinder();

protected:
  // Set to false if we can't find a piece.
  bool m_isValid;

  Finder* m_pFinder;

};
}

#endif
