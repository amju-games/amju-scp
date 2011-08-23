/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolOutOfBounds.h,v $
Revision 1.1.2.1  2006/08/14 17:50:14  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:27  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_OUT_OF_BOUNDS_H_INCLUDED)
#define ENGINE_STATE_GOLF_OUT_OF_BOUNDS_H_INCLUDED

#include "EngineStatePoolText.h"

namespace Amju
{
// Called by EngineStatePoolShotInPlay if the ball goes OB.
// NOT called by the Demo and Test states which subclass it because
// the next state will be different.
class EngineStatePoolOutOfBounds : public EngineStatePoolText
{
public:
  static const char* Name;

//  virtual void Draw();
  virtual void SetActive(bool active);
  virtual bool Load();

protected:
  virtual void TimerExpired();
};
}

#endif


