/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateNull.h,v $
Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_ENGINE_STATE_NULL_H_INCLUDED)
#define SCHMICKEN_ENGINE_STATE_NULL_H_INCLUDED

#include "EngineState.h"

namespace Amju
{
// Do-nothing Engine State, required for Editor.
class EngineStateNull : public EngineState
{
public:
  virtual void Draw() {}
};
}

#endif

