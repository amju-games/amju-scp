/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePick.h,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_ENGINE_STATE_PICK_H_INCLUDED)
#define SCHMICKEN_ENGINE_STATE_PICK_H_INCLUDED

#include "EngineState.h"

namespace Amju
{
// When in this state, no objects are updated. Objects are drawn in 
// distinct colours so the object under the cursor can be found.
class EngineStatePick : public EngineState
{
public:
  static const std::string Name; // ID of this state.

  virtual void SetActive(bool active);
  virtual void Draw();
  virtual void Update();

};
}

#endif

