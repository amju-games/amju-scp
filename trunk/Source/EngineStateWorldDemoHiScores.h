/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateWorldDemoHiScores.h,v $
Revision 1.1.2.2  2006/08/14 17:50:19  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_WORLD_DEMO_HI_SCORES_H_INCLUDED)
#define ENGINE_STATE_WORLD_DEMO_HI_SCORES_H_INCLUDED

#include <vector>
#include "EngineStateText.h"

namespace Amju
{
class EngineStateWorldDemoHiScores : public EngineStateText
{
public:
  static const char* Name;
  virtual void DrawOverlays();
  virtual bool Load();
  virtual void TimerExpired();
  virtual void MouseButton(bool, bool, bool);
  virtual void SetActive(bool);

protected:
  std::vector<PSolidComponent> m_strs;
  // Top position of text; goes up to scroll.
  float m_hiScoreTop;
};
}

#endif
