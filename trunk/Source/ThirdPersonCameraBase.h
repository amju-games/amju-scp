/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThirdPersonCameraBase.h,v $
Revision 1.2.6.1  2005/05/08 08:06:31  jay
Added Zoom functions, for key-controlled zooming.
Reset() iin subclasses should call the parent class version.

Revision 1.2  2004/09/17 13:44:50  jay
Added enable function for mouse control

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(THIRD_PERSON_CAMERA_BASE_H_INCLUDED)
#define THIRD_PERSON_CAMERA_BASE_H_INCLUDED

#include "Camera.h"
#include "HeightServer.h"

namespace Amju
{
class Level;

// Generally useful functions for third-person cameras.
class ThirdPersonCameraBase : public Camera
{
public:
  ThirdPersonCameraBase();

  virtual void GetConfigValues();
  virtual void Draw();
  virtual void DrawRotation();
  virtual void Update();

  // Get the distance from the camera to the player.
  float GetDistance() const { return m_behind; }
 
  // Call to start the camera quick swing. 
  void QuickSwingBehind();

  // Collision detection, to prevent the camera going through walls.
  // Call this functions to keep the HeightServer current.
  virtual void RefreshHeightServer(Level* pLevel);

  // Rotate 3rd person camera about player, by some angle (proportional to 
  // mouse movement).
  virtual void RotateCameraHoriz(float rot);

  // Override Z button to reset the camera.
  virtual void Z(bool down);

  // Implement mouse control
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);
  virtual void OnMouseDrag(int x, int dy);

  // Keyboard control
  virtual void PlusUp(bool);
  virtual void PlusDown(bool);
  virtual void PlusLeft(bool);
  virtual void PlusRight(bool);

  // Set rotation vel/accel
  void SetRotateVel(float vel, float acc);

  // Enable/disable mouse control
  void SetMouseDragEnabled(bool b) { m_mouseDragEnabled = b; }
  void SetMouseZoomEnabled(bool b) { m_mouseZoomEnabled = b; }

  // Implement zooming for 3rd-person camera: move towards/away from the target.
  virtual void ZoomIn();
  virtual void ZoomOut();

  virtual void Reset();

protected:
  // Returns true if moving from before to after hits a wall.
  bool Collision(const Orientation& before, const Orientation& after);

  // Raise the camera height if the player is overlooking a drop.
  bool OverlookDrop();
  
  float m_oldPlayerX;
  float m_oldPlayerZ;
  float m_playerX;
  float m_playerZ;

  HeightServer m_heightServer;

  // If true, we are swinging behing the player quickly.
  bool m_quickSwing;

  // If true, don't rotate to face direction player is facing.
  bool m_dontFollow;

  // Camera position is this distance behind player position.
  // This should track between the min and max values.
  float m_behind;

  // Constants for camera behaviour - except they're not const because they are 
  // read from the confilg file at runtime.
  float m_behindMin; // min distance from camera to player
  float m_behindMax; // max distance from camera to player

  float m_heightMax; // max height of camera above player (usually)
  float m_heightMin; // Min height of camera above player

  // Time player must be idle before we swing behind.
  float m_idleSwingTime; 

  // Time player is idle before camera rises.
  float m_idleRaiseTime;

  // Idle: Swing behind player with this velocity (degs/sec).
  float m_swingVel;

  // Not idle: swing behind player at this vel (degs/sec).
  float m_quickSwingVel;

  // Raise camera at this velocity (units/sec).
  float m_raiseVel;

  // Lower camera at this vel (units/sec).
  float m_lowerVel;

  // Height above player we look at - this is a hacky way of getting more
  // of the scene beyond the player into the view.
  float m_lookAbove;

  // Extra height camera gains when player overlooks a drop/faces down a slope.
  float m_dropExtraHeight;

  // Absolute min y-value
  float m_minAbsHeight;
  // Absoloute max y value
  float m_maxAbsHeight;

  // Set when height has been changed by player dragging mouse.
  bool m_heightDragged; 


  // Rotate at this vel - set to non-zero if rotate key is pressed.
  float m_yRotVel;
  // Go up/down at this vel if key is pressed
  float m_upVel;
  // Acceleration for m_yRotVel
  float m_yRotAcc;
  // Acceleration for m_upVel;
  float m_upAcc;

  // If true, dragging the mouse rotates the camera look-at vertex around
  // its orientation.
  bool m_mouseDragEnabled;

  // If true, clicking on mouse button zooms camera in direction of  look-at vertex
  bool m_mouseZoomEnabled;

  // New for POOL, zoom with keys. If vel is zero, no zoom.
  // Decelerates to zero.
  float m_zoomVel;
};
}

#endif

