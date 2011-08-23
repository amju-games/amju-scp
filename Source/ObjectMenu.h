/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ObjectMenu.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(OBJECT_MENU_H_INCLUDED)
#define OBJECT_MENU_H_INCLUDED

#include "Menu.h"
#include "Vertex.h"

namespace Amju
{
// This kind of menu displays any Solid objects, so has a scaling factor
// for each item. 
class ObjectMenu : public Menu
{
public:
  virtual void Draw();
  virtual bool Load(File*);

  // Set scaling factor f for menu item i.
  void SetScale(int i, float f);

  void SetPos(int i, float x, float y, float z);

protected:
  virtual void DrawItem(int i);

protected:
  // Scale for each object
  std::vector<float> m_scales;
  // Position for each object.
  std::vector<VertexBase> m_positions;
};
}

#endif

