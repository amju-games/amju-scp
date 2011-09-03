/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidLeaf.h,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SOLID_LEAF_H_INCLUDED)
#define SOLID_LEAF_H_INCLUDED

#include "SolidComponent.h"
#include "LeafData.h"
#include "LeafServer.h"
#include "Polygon.h"
#include "HeightServer.h"
#include "Vertex.h"
#include <string>
#include <vector>
#include <map>

namespace Amju
{
class LeafData;
class HeightServer;

class SolidLeaf : public SolidComponent
{
public:
  SolidLeaf();
  SolidLeaf(LeafData* pLeafData);
  SolidLeaf(const SolidLeaf& rhs);

  virtual ~SolidLeaf();
  virtual PSolidComponent Clone();

  virtual void Draw();
  virtual void DrawSphere();
  virtual int CullDraw(const Frustum& f);

  virtual bool Load(const std::string& filename);

  virtual int GetNumberOfPolygons() const;

  virtual void StoreHeights(const Matrix& cm);

  // Add heights to the given HeightServer if we intersect the given
  // bounding sphere.
  virtual void AddHeights(HeightServer* pResult, const BoundingSphere& bs);

  virtual bool IsLeaf() const { return true; }

  virtual void CreateBoundingSphere(const Matrix& cm);

  virtual bool LineIntersects(const Mgc::Segment3& seg, float r) const;

  virtual void RecalculateAbsoluteCoords(const Matrix& cm);

  virtual bool BoxIntersects(const BoundingBox& b) const;

  Polygon* GetPolygon(int index);

  const HeightServer& GetHeightServer() const;

  bool IsSolid() const;

  // Functions which allow us to create Leaves at run time
  // -----------------------------------------------------

  // Required to transform Leaves on the fly.
  void CreateDisplayList();

  // Allocate polygon p on heap. Ownership passes to this Leaf.
  void AddPolygon(Polygon* p);
  bool DeletePolygon(Polygon* p);

  TextureMethod* GetTextureMethod(); 
  void SetTextureMethod(TextureMethod* );

  PoolTexture* GetTexture();
  void SetTexture(PoolTexture* pTexture);

  void CreateNormals(bool smooth);
  
  LeafData* GetLeafData() { return m_pLeafData; }

protected:
  // Allow Polygons access to the leaf data.
  friend class Polygon;
  // Many leaves can point to the same LeafData. The LeafData is owned by 
  // LeafServer.
  LeafData* m_pLeafData;
  // Each leaf has a separate HeightServer.
  HeightServer m_heightServer;


#if defined(SCENE_EDITOR)
public:
  virtual bool Save(bool recursive = false);
#endif 

};
}
#endif

