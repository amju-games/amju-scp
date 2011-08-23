/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafRenderOglSorted.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEAF_RENDER_OGL_SORTED)
#define LEAF_RENDER_OGL_SORTED

#include "LeafRenderer.h"
#include "Matrix.h"
#include <list>

namespace Amju
{
// This Leaf Renderer uses Open GL display lists. The lists are sorted
// by texture, in an attempt to speed things up.
class LeafRenderOglSorted : public LeafRenderer
{
public:
  virtual void Init(LeafData* pLd);
  virtual void Draw(LeafData* pLd);
  virtual void Clear(LeafData* pLd);
  // Execute the sorted list of display lists.
  virtual void DrawList();

protected:
  class DisplayListItem
  {
  public:
    DisplayListItem(LeafData* pLd, const Matrix& m);

    bool operator<(const DisplayListItem& rhs) const;
    void Draw();

  protected:
    LeafData* m_pLd;
    Matrix m_matrix;
    int m_id;
    static int s_id;
  };

  void ClearList();

  typedef std::list<DisplayListItem> SortedDisplayList;
  SortedDisplayList m_sortedList;
};
}

#endif
