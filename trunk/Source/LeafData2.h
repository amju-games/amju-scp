
#ifndef LEAF_DATA_2_H_INCLUDED
#define LEAF_DATA_2_H_INCLUDED

#include <vector>
#include "../AmjuGL/AmjuGL.h"
#include "Matrix.h"
#include "TextureMethod.h"
#include "File.h"

namespace Amju
{
class HeightServer;
class PoolTexture;
class BoundingBox;
class BoundingSphere;

// Same interface as LeafData, but implemented as indexed tri list.
class LeafData
{
public:
  LeafData(const std::string& name);
  ~LeafData();
  
  void Draw();
  
  bool Load(File* pf); 

  void StoreAbsoluteCoords(const Matrix& m, HeightServer* pHs);

  // TODO DO we need this ??
  void RecalculateAbsoluteCoords(const Matrix& cm);

  // This is used for generating Terrain.
  // TODO Add a function for adding TriStrips which would be more efficient
  // For now, Terrain gen is disabled.
////  void AddPolygon(Polygon* p);

// Only required for Editor
////  bool DeletePolygon(Polygon* p);

  // Provides hook to create Display List if needed.
////  void MakeDisplayList();

  // Set Texture: used by Scene Editor.
  void SetTexture(PoolTexture* pTexture);
  // Get texture: the texture is owned by the TextureServer.
  PoolTexture* GetTexture();

  // Set the Texture Method: can be used by Scene Editor etc.
  // Allocate pTm on heap: ownership passes to this Leaf Data.
  void SetTextureMethod(TextureMethod* pTm);

  void SetTexGenMode() const;

  // Returns true if the texture method for this leaf data uses Texture Coords
  // (i.e. doesn't automatically generate the coords).
  bool HasTextureCoords() const;

  // Get Leaf name.
  std::string GetName() const { return m_name; }

  // For OpenGL rendering, create the Display List for the leaf.
  // Called by Leaf Renderer to really use display lists.
////  void CreateDisplayList();
  // Call the display list. Used by Leaf Renderer to really ues display lists.
////  void ExecuteDisplayList();

  // If Solid, the geometry should be collide-able. If not solid, objects should
  // be able to pass through the geometry.
  bool IsSolid() const { return m_isSolid; }
  bool HasSmoothNormals() const { return m_smoothNormals; }
  bool IsVisible() const { return m_isVisible; }

  void SetSmoothNormals(bool b) { m_smoothNormals = b; }
  void SetIsSolid(bool b) { m_isSolid = b; }
  void SetIsVisible(bool b) { m_isVisible = b; }

  void CreateBoundingBox(BoundingBox*, const Matrix& m);
  void CreateBoundingSphere(BoundingSphere*, const Matrix& m);

private:
  void CreateSmoothNormals();

private:
  // Leaf name, for debugging/error reporting.
  std::string m_name;

  // Method of texturing: regular, auto-generated or env-mapped etc.
  SharedPtr<TextureMethod> m_pTextureMethod;

  // If true, we generate smooth normals; if false, perpendicular normals.
  bool m_smoothNormals;

  // If true, this geometry provides solid walls/floors.
  bool m_isSolid;

  // Leaves may be invisible - in this case they just block movement.
  bool m_isVisible;

/* Use AmjuGL decl.

  // Vertex: (x, y, z) abs coord, (u, v) texture coord and normal.
  struct Vert
  {
    Vert() {} 
    Vert(float x, float y, float z, float u, float v, float nx, float ny, float nz) : m_x(x), m_y(y), m_z(z), m_u(u), m_v(v), m_nx(nx), m_ny(ny), m_nz(nz) {}

    float m_x, m_y, m_z, m_u, m_v, m_nx, m_ny, m_nz;
  };

  // For non-indexed triangle list, this is simply the 3 verts of a triangle.
  struct Tri
  {
    Vert m_verts[3];
  };
*/

  // For non-indexed tri list, this is the list of triangles.
  //// In AmjuGL typedef std::vector<AmjuGL::Tri> Tris;
  AmjuGL::Tris m_tris;  

#ifdef INDEXED_TRI_LIST
  // Verts
  typedef std::vector<Vert> Verts;
  Verts m_verts;
  
  // Tri: has 3 vertex indices
  struct Tri
  {
    int m_a, m_b, m_c;
  };

  // Tris
  typedef std::vector<Tri> Tris;
  Tris m_tris;  
#endif

  // TODO Maybe support TriStrips too, especially useful for Terrain

// This version on LeafData does not have to support Scene Ed.
#if (0) //defined(SCENE_EDITOR)
public:
  bool Save(File* pf);

  const Matrix& GetMatrix() const { return m_matrix; }
  void SetMatrix(const Matrix& m) { m_matrix = m; }

protected:
  // Transformation matrix. This is the product of the transformations which
  // map the 'relative' vertex info to the 'absolute' vertex info.
  Matrix m_matrix;
#endif

};
}

#endif


