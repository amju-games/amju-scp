/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionTimedBlocker.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(FUNCTION_TIMED_BLOCKER_H_INCLUDED)
#define FUNCTION_TIMED_BLOCKER_H_INCLUDED

#include "FunctionBlocker.h"

namespace Amju
{
// Timed blocker: blocker which returns to original orientation
// after a time delay.
class FunctionTimedBlocker : public FunctionBlocker
{
public:
  static const char* Name;

  virtual bool Load(File* pf);
  virtual void Recalc(Orientation* p);

protected:
  // Length of time to stay in stationary state before moving 
  // BACK to initial state
  float m_timeOpen; // time for which blocker is "open".

  // True if we are moving back to initial state.
  bool m_isClosing;


#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const;
  virtual bool Save(File*);
#endif
};
}

#endif


