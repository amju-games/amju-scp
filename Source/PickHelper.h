/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PickHelper.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(PICK_HELPER_H_INCLUDED)
#define PICK_HELPER_H_INCLUDED

#include "Colour.h"
#include "Orientation.h"
#include "GameObject.h"

namespace Amju
{
class Level;

class PickHelper
{
public:
  // Allow picking in space; i.e. allows user to pick an arbitrary unit cube rather
  // than a game object. Returns true if cursor position resolves to one cube.

  // Draw cuboids in an octree to find the volume of space pointed to by the
  // mouse cursor.
  // You supply a bounding box which is subdivided by the given number of times
  // in each axis.
  bool PickSpaceDraw();

  // Draw selected volume of space.
  void DebugDraw();

  // Get the colour under the mouse cursor.
  static Colour ReadCursorColour();

  // Returns the ID of the Game Object under the mouse cursor, or -1
  // if there is no game object under the cursor.
  static int PickDrawGameObjects(Level* pLevel);

  // Get new orientation for a Game Object which is being dragged.
  // The object is drawn in different positions. You give the position change
  // for each axis.
  static Orientation ObjectDragOrientation(PGameObject pGo, float dx, float dy, float dz);

protected:
  // Draw a cube. Returns true if the mouse cursor is on the cube.
  // If size is bigger than some limit, subdivides the volume and
  // recursively calls PickSpaceDrawSections.
  // Adds vertices to m_selectedCubes if size is below limit.
  bool PickSpaceDrawOneSection(
    float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float size);
  void PickSpaceDrawSections(float x, float y, float z, float s);

protected:
  // Unit cubes which the mouse cursor could be pointing at. 
  // (Unreachable cubes etc are pruned.)
  typedef std::vector<VertexBase> CubeVec;
  CubeVec m_selectedCubes;
};
}

#endif

