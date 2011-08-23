/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ScaleSolid.cpp,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#include "SolidComponent.h"
#include "SolidComposite.h"
#include "SolidLeaf.h"
#include "Polygon.h"
#include "SchAssert.h"

namespace Amju
{
void ScaleRecursive(SolidComponent* pNode, float x, float y, float z)
{
  SolidComposite* pComp = dynamic_cast<SolidComposite*>(pNode);
  if (pComp)
  {
    for (int i = 0; i < pComp->GetNumChildren(); i++)
    {
      SolidComponent* pChild = pComp->GetChild(i).GetPtr();
      ScaleRecursive(pChild, x, y, z);
    }
    return;
  }
  // If a leaf, apply the scaling. Then recompile the display list.
  SolidLeaf* pLeaf = dynamic_cast<SolidLeaf*>(pNode);
  if (pLeaf)
  {
    // Make a matrix of the requested transform.
    Matrix s;
    s.identity();
    s.Scale(x, y, z);

    // Copied from TransformLeaf
    // Iterate through every polygon. For every vertex, perform
    // the transformations requested, by multiplying by the transformation
    // matrix.
    int numpolys = pLeaf->GetNumberOfPolygons();
    for (int i = 0; i < numpolys; i++)
    {
      Amju::Polygon* pPoly = pLeaf->GetPolygon(i);
      Assert(pPoly);
      int numvertices = pPoly->Vertices();
      for (int j = 0; j < numvertices; j++)
      {
        SceneVertex* pV = pPoly->GetVertex(j);
        VertexBase v(pV->x, pV->y, pV->z);
        // Multiply vertex by the transformation matrix.
        v = v * s;
        pV->SetRelative(v.x, v.y, v.z);
      }
    }
    
    // Recompile the display list for the leaf
    pLeaf->CreateDisplayList();
#if defined(SCENE_EDITOR)
    pLeaf->SetDirty(true);
#endif
  }
}

}
