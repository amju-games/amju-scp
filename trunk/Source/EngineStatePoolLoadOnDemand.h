#ifndef ENGINE_STATE_POOL_LOAD_ON_DEMAND_H_INCLUDED
#define ENGINE_STATE_POOL_LOAD_ON_DEMAND_H_INCLUDED

#include "EngineStateText.h"

namespace Amju
{
// Load one level (or room in level??) - as required 

class EngineStatePoolLoadOnDemand : public EngineStateText
{
public:
  static const std::string Name; 

  EngineStatePoolLoadOnDemand();

  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Draw();
  virtual void DrawOverlays();
  virtual void Update();
  
  static void SetLevelToLoad(int levelNum);
  static void SetPrevState(const std::string& stateName);

protected:
};
}


#endif
