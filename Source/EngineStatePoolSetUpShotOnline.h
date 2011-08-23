
#ifndef ENGINE_STATE_POOL_SET_UP_SHOT_ONLINE_H_INCLUDED
#define ENGINE_STATE_POOL_SET_UP_SHOT_ONLINE_H_INCLUDED

#include "EngineStatePoolSetUpShot.h"

namespace Amju
{
class EngineStatePoolSetUpShotOnline : public EngineStatePoolSetUpShot
{
public:
  virtual void SetActive(bool active);

};
}

#endif

