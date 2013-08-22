#ifndef ENGINE_STATE_POOL_INIT_GAME_H_INCLUDED
#define ENGINE_STATE_POOL_INIT_GAME_H_INCLUDED

#include "EngineState.h"

namespace Amju
{
class EngineStatePoolInitGame : public EngineState
{
public:
  static const char* Name;
  virtual void SetActive(bool active);
  virtual void Draw() {}

  static void SetRoom(int room);
};
}

#endif
