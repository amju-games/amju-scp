/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolShowShot.h,v $
Revision 1.1.2.1  2006/08/14 17:50:17  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:30  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.5  2005/09/23 19:40:41  jay
Add include for convenience

Revision 1.1.8.4  2005/09/12 17:23:48  jay
Override Update() to avoid possibility of setting balls in motion, then calling
Update twice before testing for collisions

Revision 1.1.8.3  2005/08/26 21:31:56  jay
Add mouse camera control

Revision 1.1.8.2  2005/07/05 09:37:36  jay
Show valid shots for debugging

Revision 1.1.8.1  2005/06/19 21:31:30  jay
Override DrawOverlays so HUD is consistent... needs work

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if !defined(ENGINE_STATE_GOLF_SHOW_SHOT_H_INCLUDED)
#define ENGINE_STATE_GOLF_SHOW_SHOT_H_INCLUDED

#include "EngineStatePoolBase.h"
#include "Level.h"

namespace Amju
{
class EngineStatePoolShowShot : public EngineStatePoolBase
{
public:
  static const char* Name;
  virtual void SetActive(bool active);
  virtual void TimerExpired();
  virtual void Draw();
  virtual void DrawOverlays();  

  // Override so Pool Balls not updated TWICE before collisions tested
  virtual void Update();

  virtual void Green(bool);
  virtual void Blue(bool);

  virtual void OnKey(char c, bool down);

  // Move camera with mouse 
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

protected:
  // AI testing: cycle through all valid shots
  int m_validShotNum;
};
}


#endif



