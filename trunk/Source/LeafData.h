/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafData.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEAF_DATA_H_INCLUDED)
#define LEAF_DATA_H_INCLUDED

#include "SolidComponent.h"
#include "Polygon.h"
#include "File.h"
#include "Texture.h"
#include "TextureMethod.h"
#include <ObjMesh.h>
#include <string>
#include <vector>

namespace Amju
{
class HeightServer;
class LeafRenderer;

typedef std::vector<Polygon> PolyVector;

class LeafData
{
  friend class SolidLeaf; 
  friend class LeafRenderOpenGL;
  friend class LeafRenderNoList;
  friend class LeafRenderMFC;

public:
  LeafData(const std::string& name);
   ~LeafData();

  static void SetRenderer(LeafRenderer* pLr) { s_pLeafRenderer = pLr; }
  static LeafRenderer* GetRenderer() { return s_pLeafRenderer; }

  // Draw isn't virtual, it delegates to the LeafRenderer.
  // For LeafRenderOpenGL, this executes the display list created in 
  // MakeDisplayList().
  void Draw();

  bool Load(File* pf);
  bool Load(const std::string& filename);

  void StoreAbsoluteCoords(Matrix m, HeightServer* pHs);
  void RecalculateAbsoluteCoords(const Matrix& cm);

  void AddPolygon(Polygon* p);
  bool DeletePolygon(Polygon* p);

  // Provides hook to create Display List if needed. 
  void MakeDisplayList();

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

  PolyVector* GetPolygons() { return &m_polygons; }

  // For OpenGL rendering, create the Display List for the leaf.
  // Called by Leaf Renderer to really use display lists.
  void CreateDisplayList();
  // Call the display list. Used by Leaf Renderer to really ues display lists.
  void ExecuteDisplayList();

  // If Solid, the geometry should be collide-able. If not solid, objects should
  // be able to pass through the geometry.
  bool IsSolid() const { return m_isSolid; }
  bool HasSmoothNormals() const { return m_smoothNormals; }
  bool IsVisible() const { return m_isVisible; }

  void SetSmoothNormals(bool b) { m_smoothNormals = b; }
  void SetIsSolid(bool b) { m_isSolid = b; }
  void SetIsVisible(bool b) { m_isVisible = b; }

protected:

  // Clear all allocated data, for reloading, freeing level once finished, etc.
  void Clear();

  // Create perpendicular normals at each vertex; smooth them if smoothing is on.
  void CreateNormals();
  void CreateSmoothNormals();

  void CreatePolygonDisplayList();

  bool LoadTexture(File* pf);

  bool LoadNormalsFlag(File* pf);

  bool LoadVersion6(File* pf); // bkwd-compatible load
  bool LoadPolygons(File* pf);

  bool LoadSolidityFlag(File* pf);
  bool LoadVisibleFlag(File* pf);

protected:
  // Leaf name, for debugging/error reporting.
  std::string m_name;

  // Polygons in the leaf. 
  PolyVector m_polygons;

  // OpenGL Display list identifier.
  unsigned int m_displayList;

  // Method of texturing: regular, auto-generated or env-mapped etc.
  SharedPtr<TextureMethod> m_pTextureMethod;

  // If true, we generate smooth normals; if false, perpendicular normals.
  bool m_smoothNormals;

  // If true, this geometry provides solid walls/floors.
  bool m_isSolid; 

  // Leaves may be invisible - in this case they just block movement.
  bool m_isVisible;

  // The object which renders all leaves.
  static LeafRenderer* s_pLeafRenderer;

  ObjMesh m_obj;


#if defined(SCENE_EDITOR)
public:
  bool Save(File* pf);

  const Matrix& GetMatrix() const { return m_matrix; }
  void SetMatrix(const Matrix& m) { m_matrix = m; }
  
protected:
  bool SaveTexture(File* jf);
  bool SaveNormalsFlag(File* jf);
  bool SaveSolidityFlag(File* pf);
  bool SaveVisibilityFlag(File*);

protected:
  // Transformation matrix. This is the product of the transformations which
  // map the 'relative' vertex info to the 'absolute' vertex info. 
  Matrix m_matrix;


#endif
};
}
#endif

