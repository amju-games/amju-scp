/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Polygon.h,v $
Revision 1.1.10.1  2007/07/15 21:56:58  Administrator
DEBUG mode: Each Plane stores the name of the leaf which created it.
This is for debugging HeightServer issues, where you want to know where
a poly has come from.

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_POLYGON_H_INCLUDED)
#define SCHMICKEN_POLYGON_H_INCLUDED

#include <vector>
#include "Matrix.h"

namespace Amju
{
class Texture;
class File;
class HeightServer;
class PolyLoader;
class PolyDrawer;
class LeafData;
class SolidLeaf;
class SceneVertex;

class Polygon
{
  friend class PolyDrawMFC;
  friend class PolyDrawOpenGL;
  friend class PolyLoader;

public:
  // The object which draws all polygons.
  static void SetDrawer(PolyDrawer* p) { s_polyDrawer = p; }
  static PolyDrawer* GetDrawer() { return s_polyDrawer; }

  // The object which loads all polygons.
  static void SetLoader(PolyLoader* p) { s_polyLoader = p; } 
  static PolyLoader* GetLoader() { return s_polyLoader; }

  // No owner - so so matrix will be available for this poly.
  Polygon();

  ~Polygon();

  void Draw();

  bool Load(File* pf, bool hasTexCoords); 

  // Return the number of vertices for this polygon.
  int Vertices() const { return m_theVertices.size(); }

  // Return a vertex.
  const SceneVertex& GetVertex(int index) const;
  SceneVertex* GetVertex(int index);

  // Get absolute coords from relative coords, using transformation matrix.
  // Stores abs. coods in height server if required for this poly.
  // Stores matrix for later.
  void StoreAbsoluteCoords(Matrix m, HeightServer* pHs);

  // Used to redo absolute coords when an orientation changes. 
  // Uses matrix stored after call to StoreAbsoluteCoords() above.
  void RecalculateAbsoluteCoords(const Matrix& cm);

  // Add a new vertex to back of vector. 
  // In both cases the vertex is copied.
  void AddVertex(const SceneVertex& vertex);
  void AddTexVertex(const TexVertex& tv);

  // Insert a vertex in a specific position.
  // The new vertex goes in position (index). So if index is zero, the new 
  // vertex is first in the vector.
  // Intended for SceneEditor, but currently unused.
  void InsertVertex(const SceneVertex& vertex, int index);

  // Set all vertex normals to the forward-facing perpendicular to the plane.
  void SetPerpendicularNormals();

protected:
  static PolyDrawer* s_polyDrawer;
  static PolyLoader* s_polyLoader;

  void Init();

  typedef std::vector<SceneVertex> VertexVector;
  VertexVector m_theVertices;

  // Vector of texture coords. This may be empty, but if not, the number of
  // elements must match the number of elements in the vector of vertices.
  typedef std::vector<TexVertex> TexVertVector;
  TexVertVector m_textureVerts;


#if defined(SCENE_EDITOR) || defined(_DEBUG)
public:
  // All Polygons point back to their owning parent. This can be set by
  // specifying a Leaf or the actual Leaf Data.
  // The pointer to parent is actually only used by SceneEditor. 
  // TODO We don't need this pointer usually, so remove it.
  // Maybe a derived class - PolyWithParent, or template ?
  Polygon(LeafData* pParent);
  Polygon(SolidLeaf* pLeaf);


  bool Save(File*);

  
  // Transformation matrix we used to get absolute coords.
  // This is used in SceneEditor to recalculate absolute coords when
  // a relative coord is changed.
  const Matrix& GetMatrix() const; 

protected:
  // Each polygon points back to its Leaf Data parent.
  // This is required only by SceneEditor.
  LeafData* m_pLeaf;
#endif

};
}
#endif

