/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafRenderNoList.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEAF_RENDER_NO_LIST_H_INCLUDED)
#define LEAF_RENDER_NO_LIST_H_INCLUDED

#include "LeafRenderer.h"

namespace Amju
{
// Use OpenGL to draw leaves but without using display lists.
class LeafRenderNoList : public LeafRenderer
{
public:
  // Initialise the given Leaf Data object for rendering.
  virtual void Init(LeafData* pLd);

  // Render the given Leaf Data object.
  virtual void Draw(LeafData* pLd);

  virtual void Clear(LeafData* pLd);

  void ResetColourCounter();

private:
  int m_col;
};
}
#endif

