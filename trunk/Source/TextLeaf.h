/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextLeaf.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXT_LEAF_H_INCLUDED)
#define TEXT_LEAF_H_INCLUDED

#include "SolidComponent.h"
#include <string>

namespace Amju
{
// A Composite, showing text.
class TextLeaf : public SolidComponent
{
public:
  TextLeaf();
  TextLeaf(const TextLeaf&);

  virtual PSolidComponent Clone();
  virtual void Draw();
  virtual int CullDraw(const Frustum& f) { Draw(); return 0; }
  virtual int GetNumberOfPolygons() const { return 0; }
  virtual bool Load(const std::string& filename);
  // Load from an opened file object - used to build menus.
  virtual bool Load(File* pf);

  float GetX() const { return m_x; }
  float GetY() const { return m_y; }
  void SetX(float x) { m_x = x; }
  void SetY(float y) { m_y = y; }

  void SetText(const std::string& s) { m_text = s; }

protected:
  // 'Print at' coords
  float m_x; // 0 <= x < 30
  float m_y; // 0 <= y < 20
  std::string m_text;
};
}
#endif

