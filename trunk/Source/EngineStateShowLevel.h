/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateShowLevel.h,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_SHOW_LEVEL)
#define ENGINE_STATE_SHOW_LEVEL

#include "EngineStateText.h"
#include "SolidComponent.h"

namespace Amju
{
// Shows level number and the number of chicks which must be saved to 
// clear the level.
class EngineStateShowLevel : public EngineStateText
{
public:
  static const char* Name; // ID of this state.

  EngineStateShowLevel();

  virtual void Draw();
  virtual bool Load();

  virtual void Red(bool);

  virtual void SetActive(bool active);

private:
  PSolidComponent  m_leveltext;
  PSolidComponent  m_leveltext2;

};
}
#endif
