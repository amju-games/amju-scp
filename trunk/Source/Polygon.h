/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Polygon.h,v $
Revision 1.1.10.1  2007/07/15 21:56:58  Administrator
DEBUG mode: Each HSTri stores the name of the leaf which created it.
This is for debugging HeightServer issues, where you want to know where
a poly has come from.

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_POLYGON_H_INCLUDED)
#define SCHMICKEN_POLYGON_H_INCLUDED

#include <vector>
#include <Matrix.h>
#include "Vertex.h"

namespace Amju
{
class PoolTexture;
class File;
class HeightServer;
class PolyLoader;
class PolyDrawer;
class LeafData;
class SolidLeaf;

class Polygon
{
public:
  Polygon();

  ~Polygon();

  //void Draw();

  //bool Load(File* pf, bool hasTexCoords); 

  // Return the number of vertices for this polygon.
////  int Vertices() const { return m_theVertices.size(); }

  // Return a vertex.  TODO eliminate this
  const SceneVertex& GetVertex(int index) const;
  SceneVertex* GetVertex(int index);

  // Get absolute coords from relative coords, using transformation matrix.
  // Stores abs. coods in height server if required for this poly.
  // Stores matrix for later.
  void StoreAbsoluteCoords(Matrix m, HeightServer* pHs);

  // Used to redo absolute coords when an orientation changes. 
  // Uses matrix stored after call to StoreAbsoluteCoords() above.
  // TODO Can we get rid of this, it seems to do nothing
  void RecalculateAbsoluteCoords(const Matrix& cm);

  // Add a new vertex to back of vector. 
  // In both cases the vertex is copied.
  //void AddVertex(const SceneVertex& vertex);
  //void AddTexVertex(const TexVertex& tv);

  // Insert a vertex in a specific position.
  // The new vertex goes in position (index). So if index is zero, the new 
  // vertex is first in the vector.
  // Intended for SceneEditor, but currently unused.
  //void InsertVertex(const SceneVertex& vertex, int index);

  // Set all vertex normals to the forward-facing perpendicular to the plane.
  //void SetPerpendicularNormals();

//protected:
  //typedef std::vector<SceneVertex> VertexVector;
  SceneVertex m_theVertices[3];

};
}
#endif

