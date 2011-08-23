/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateSelectLevel.h,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_ENGINE_STATE_SELECT_LEVEL_H_INCLUDED)
#define SCHMICKEN_ENGINE_STATE_SELECT_LEVEL_H_INCLUDED

#include "EngineMenuState.h"
#include "SolidComponent.h"

namespace Amju
{
// At start of game, select start level. 
// Levels which have been completed can be skipped subsequently.
class EngineStateSelectLevel : public EngineMenuState
{
public:
  static const char* Name; // ID of this state.

  EngineStateSelectLevel();

  virtual void SetActive(bool active);
  virtual void Draw();
  virtual bool Load();
  virtual void Red(bool);

protected:
  PSolidComponent  m_text;
};
}

#endif

