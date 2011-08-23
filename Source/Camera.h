/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Camera.h,v $
Revision 1.1.10.1  2005/05/08 08:06:29  jay
Added Zoom functions, for key-controlled zooming.
Reset() iin subclasses should call the parent class version.

Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(CAMERA_H_INCLUDED)
#define CAMERA_H_INCLUDED

#include "SharedPtr.h"
#include "Orientation.h"
#include "Vertex.h"
#include "FreeMovingGameObject.h"

namespace Amju
{
class Level;

class Camera : public FreeMovingGameObject  //Shareable
{
public:
  Camera();
  virtual ~Camera() {}

  // Game Object overrides
  // ---------------------
  virtual const char* GetTypeName() const;
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& ) {}   

#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

  // 'Drawing' a Camera means orientating
  // the scene to the camera POV.
  virtual void Draw() = 0;

  // Cameras may also update each frame.
  virtual void Update() {}

  // Draw rotation only. Used for Skybox effect.
  virtual void DrawRotation() = 0;

  virtual void Reset();

  virtual const Orientation* GetOrientation() { return &m_orientation; }
  virtual void SetOrientation(const Orientation& o) { m_orientation = o; }

  VertexBase GetLookAtPos() const { return m_lookAtPos; }

  // For subclasses which get constants from a config file. Do it in this 
  // function, and assign to members - don't get the values every time
  // round the game loop!
  virtual void GetConfigValues() {}

  // Set the protagonist, who we are either looking at or through.
  virtual void SetPlayer(FreeMovingGameObject* );
  virtual FreeMovingGameObject* GetPlayer();

  // Collision detection, to prevent the camera going through walls.
  // Call this functions to keep the HeightServer current.
  virtual void RefreshHeightServer(Level* );

  // Cameras can respond to user input. 
  virtual void Red(bool) {}
  virtual void Blue(bool) {}
  virtual void Green(bool) {}
  virtual void YellowUp(bool) {}
  virtual void YellowDown(bool) {}
  virtual void YellowLeft(bool) {}
  virtual void YellowRight(bool) {}
  virtual void Z(bool) {} 
  virtual void PlusUp(bool) {}
  virtual void PlusDown(bool) {}
  virtual void PlusLeft(bool) {}
  virtual void PlusRight(bool) {}
  virtual void JoyX(float f) {}
  virtual void JoyY(float f) {}
  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  // Provide an interface for zooming in/out. Different camera types will
  // do this in different ways. 
  virtual void ZoomIn() {}
  virtual void ZoomOut() {}

protected: // ?

  // Called from MousePos() when coords change.
  virtual void OnMouseDrag(int dx, int dy) {}

protected:
  // Rotate camera position rot degrees about centre.
  void RotateCameraHoriz(float rot, const VertexBase& centre);

protected:
  Orientation m_orientation;
  VertexBase m_lookAtPos;

  // The protagonist - in Third person mode we look at the player;
  // in first person mode we look from the player position.
  FreeMovingGameObject* m_pPlayer;

protected:
  // Cameras may be controlled using the mouse, so these mouse-related variables
  // are required.

  // Window coords of mouse cursor
  int m_mouseX, m_mouseY;

  // true if mouse cursor pos has changed since last time
  bool m_newMousePos;

  // Set to true when mouse button is down.
  bool m_drag;

  // Set to true if control is pressed as the mouse is dragged.
  bool m_dragCtrl;

  // Bounding Sphere, required for collision detection
  mutable BoundingSphere m_bsphere;
};

typedef SharedPtr<Camera> PCamera;
}
#endif

