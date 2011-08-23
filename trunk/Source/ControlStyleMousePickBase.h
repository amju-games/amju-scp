/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMousePickBase.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CONTROL_STYLE_MOUSE_PICK_BASE_H_INCLUDED)
#define CONTROL_STYLE_MOUSE_PICK_BASE_H_INCLUDED

#include <map>
#include <vector>
#include "ControlStyleMouseBase.h"
#include "Colour.h"
#include "Vertex.h"
#include "Behaviour.h"

namespace Amju
{
class ControlStyleMousePickBase: public ControlStyleMouseBase
{
public:
  ControlStyleMousePickBase();
  virtual void Reset();
  virtual void Update();

  virtual void MousePos(int x, int y);
  virtual void MouseButton(bool down, bool ctrl, bool shift);

  virtual void Draw();

protected:
  // Draw objects in false colours so we can determine the object under 
  // the cursor.
  void PickDraw();
  void PickDrawGameObjects();
  void PickDrawScene();
  Colour GetObjectPickColour(int objId);
  bool GetObjectPickId(const Colour& c, int* pResult);
  // Get the Game Object under the cursor. Returns true if successful.
  bool GetSelectedObject();

protected:
  // ID of Game Object selected by mouse cursor, or -1 meaning no object selected.
  int m_selectedObjectId;
  // Hold previous piece ID so if it is reselected we don't reset the 
  // destination.
  int m_previousSelectedObjectId;

  // Coord of last unit cube picked by calling PickSpaceDraw().
  VertexBase m_lastGoodCoord;

  typedef std::map<Colour, int> ColourMap;
  ColourMap m_colourMap;

  // If true, do picking in Draw(). I.e. the mouse has been clicked.
  bool m_doPicking;

  // Player Behaviours
  // -----------------
  // Points at the current behaviour.
  //Behaviour* m_pBehaviour;
  // Available behaviours.
  //BehaviourMap m_behaviours;
};
}

#endif


