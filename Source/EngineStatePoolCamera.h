/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolCamera.h,v $
Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2  2004/09/21 15:02:07  Administrator
Override DrawOverlays() for special text

Revision 1.1  2004/09/17 13:57:50  jay
Added camera mode

*/

#ifndef ENGINE_STATE_GOLF_CAMERA_H_INCLUDED
#define ENGINE_STATE_GOLF_CAMERA_H_INCLUDED

#include "EngineStatePoolSetUpShot.h"

namespace Amju
{
// Like the Set-Up-Shot state, but clicking and dragging the mouse on screen
// moves the camera.
class EngineStatePoolCamera : public EngineStatePoolSetUpShot
{
public:
  static const char* Name;
 
  virtual void SetActive(bool);
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool, bool, bool);
  virtual void DrawOverlays();  
};
}

#endif

