/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SimpleCamera.h,v $
Revision 1.1.8.1  2005/04/03 16:26:30  jay
Added DrawRotation - used for Pool birds eye camera.

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SIMPLE_CAMERA_H_INCLUDED)
#define SIMPLE_CAMERA_H_INCLUDED

#include "Camera.h"

namespace Amju
{
// Basic camera which you simply point in one of the 6 possible directions.
class SimpleCamera : public Camera
{
public:
  SimpleCamera();

  enum Direction { UP, DOWN, NORTH, SOUTH, EAST, WEST };

  void SetDirection(Direction);
  Direction GetDirection() const;

  virtual void SetOrientation(const Orientation& o);

  virtual void Draw();
  virtual void DrawRotation();

  // Allow ctrl-drag to zoom in/out.
  // Maybe drag to pan camera ?
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

protected:
  Direction m_direction;
  bool m_drag; // if true, mouse controls panning
  bool m_zoom; // if true, mouse controls zoom
  int m_oldx, m_oldy;
};
}

#endif


