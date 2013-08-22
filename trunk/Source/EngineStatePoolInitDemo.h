#ifndef ENGINE_STATE_POOL_INIT_DEMO_H_INCLUDED
#define ENGINE_STATE_POOL_INIT_DEMO_H_INCLUDED

#include "EngineState.h"

namespace Amju
{
class EngineStatePoolInitDemo : public EngineState
{
public:
  static const char* Name;
  virtual void SetActive(bool active);
  virtual void Draw() {}
};
}

#endif
