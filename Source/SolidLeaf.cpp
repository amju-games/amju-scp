/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidLeaf.cpp,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream> // debug
#include "SolidLeaf.h"
#include "Common.h"
#include "Polygon.h"
#include "File.h"
#include "Vertex.h"
#include "HeightServer.h"
#include "Frustum.h"
#include "CompositeFactory.h"
#include "BoundingBox.h"
#include "SchAssert.h"

namespace Amju
{
SolidLeaf::SolidLeaf() : m_pLeafData(0)
{
}

SolidLeaf::SolidLeaf(LeafData* pLeafData) : m_pLeafData(pLeafData)
{
}

SolidLeaf::SolidLeaf(const SolidLeaf& rhs) :
  SolidComponent(rhs), 
  m_pLeafData(rhs.m_pLeafData)
{
}

SolidLeaf::~SolidLeaf()
{
  //std::cout << "leaf dtor, name: " << m_name.c_str() << "\n";
}

PSolidComponent SolidLeaf::Clone()
{
  return new SolidLeaf(*this);
}

void SolidLeaf::Draw()
{
  if (m_pLeafData)
  {
    m_pLeafData->Draw();
  }
}

void SolidLeaf::DrawSphere()
{
  m_bsphere.Draw();
}

int SolidLeaf::CullDraw(const Frustum& f)
{
  if (!f.Contains(m_bsphere))
  {
    return 0;
  }
  Draw();
  return GetNumberOfPolygons();  
}

bool SolidLeaf::Load(const std::string& filename)
{
  m_name = filename;

  LeafServer* pLs = LeafServer::Instance();

  // A leaf is stored only once, and is managed by the LeafServer. One 
  // LeafData object can be pointed to by many SolidLeaf objects.
  m_pLeafData = pLs->Get(filename);

  if (!m_pLeafData)
  {
    return false;
  }

  return true;
}

void SolidLeaf::CreateDisplayList()
{
  Assert(m_pLeafData);
  m_pLeafData->CreateDisplayList();
}

void SolidLeaf::StoreHeights(const Matrix& m)
{
  if (!m_pLeafData)
  {
    return;
  }

  // Do nothing if this geometry isn't solid (collide-able)
  if (!m_pLeafData->IsSolid())
  {
    return;
  }

#if defined(SCENE_EDITOR)
  m_pLeafData->SetMatrix(m);
#endif

  // Clear H.S. so we can repeatedly call to rebuild Platform H.S.
  m_heightServer.Clear(); 
  m_pLeafData->StoreAbsoluteCoords(m, &m_heightServer); 
}

void SolidLeaf::AddHeights(HeightServer* pResult, const BoundingSphere& bs)
{
  Assert(m_pLeafData);

  // Do nothing if this geometry isn't solid (collide-able)
  if (!m_pLeafData->IsSolid())
  {
    return;
  }

  // Add the HeightServer belonging to this Leaf if the Leaf intersects the
  // given bounding sphere.
  if (GetBoundingSphere()->Intersects(bs))
  {
    // Add the contents of this leaf's HeightServer to the Result HeightServer.
    pResult->AddHeightServer(m_heightServer);
  }
}

#if defined(SCENE_EDITOR)
bool SolidLeaf::Save(bool)
{
  if (IsDirty())
  {
    File jf;  
    if (!jf.OpenWrite(GetName()))
    {
      return false;
    }
    if (m_pLeafData)
    {
      bool ret = m_pLeafData->Save(&jf);
    }

    // Binary file
    std::string bfilename = GetName();
    bfilename += "b";
    File bfile(true);
    bfile.OpenWrite(bfilename, File::CURRENT_VERSION, true /*binary*/);
    if (m_pLeafData)
    {
      bool ret = m_pLeafData->Save(&bfile);
    }
  }

  return true;
}
#endif

int SolidLeaf::GetNumberOfPolygons() const
{
  if (!m_pLeafData)
     return 0;

  return m_pLeafData->m_polygons.size();
}

Polygon* SolidLeaf::GetPolygon(int index)
{
  if (index >= GetNumberOfPolygons())
    return 0;

  return m_pLeafData->m_polygons[index];
}

void SolidLeaf::AddPolygon(Polygon* p)
{
  if (!m_pLeafData)
  {
    // Make sure this kind of leaf is known to the LeafServer.
    LeafServer* pLs = LeafServer::Instance();
    // TODO pass in type - it is currently fixed.

    m_pLeafData = new LeafData(GetName()); 
/*
#if defined(_DEBUG)
    m_pLeafData = new LeafData(GetName()); 
#else
    // TODO make sure this is ok! 
    // It should be, because we only add polys to leaves we create on the fly.
    // So the LeafServer, which loads leaves, shouldn't have to worry about
    // this one.
    m_pLeafData = new LeafData("test"); 
#endif
*/

  }
  Assert(m_pLeafData);
  m_pLeafData->m_polygons.push_back(p);
}

bool SolidLeaf::DeletePolygon(Polygon* p)
{
  if (!m_pLeafData)
  {
    return false; 
  }
  return m_pLeafData->DeletePolygon(p);
}

void SolidLeaf::SetTexture(Texture* pTexture)
{
  m_pLeafData->SetTexture(pTexture);
}

Texture* SolidLeaf::GetTexture() 
{ 
  if (!m_pLeafData)
  {
    return 0;
  }
  return m_pLeafData->GetTexture(); 
}

TextureMethod* SolidLeaf::GetTextureMethod()
{
  if (!m_pLeafData)
  {
    return 0;
  }
  return m_pLeafData->m_pTextureMethod.GetPtr(); 
}

void SolidLeaf::SetTextureMethod(TextureMethod* pTm)
{
  Assert(m_pLeafData);
  m_pLeafData->SetTextureMethod(pTm);
}

void SolidLeaf::RecalculateAbsoluteCoords(const Matrix& cm)
{
  if (m_pLeafData)
  {
    m_pLeafData->RecalculateAbsoluteCoords(cm);
  }
}

void SolidLeaf::CreateBoundingSphere(const Matrix& cm)
{
  RecalculateAbsoluteCoords(cm);
  m_bsphere.SetFromLeaf(*this);
}

bool SolidLeaf::LineIntersects(const Mgc::Segment3& seg, float r) const
{
  if (!m_pLeafData->IsSolid())
  {
    return false;
  }

  if (!m_bsphere.Intersects(seg, r))
  {
    return false;
  }
  // Line seg intersects bounding sphere, so we must test against heightserver.
  bool b = m_heightServer.Intersects(seg, r);
  return b;
}

bool SolidLeaf::BoxIntersects(const BoundingBox& b) const
{
  if (!m_pLeafData->IsSolid())
  {
    return false;
  }

  return m_heightServer.GetBoundingBox().Intersects(b);
}

const HeightServer& SolidLeaf::GetHeightServer() const
{
  return m_heightServer;
}

bool SolidLeaf::IsSolid() const
{
  return m_pLeafData->IsSolid();
}

void SolidLeaf::CreateNormals(bool smooth)
{
  m_pLeafData->SetSmoothNormals(smooth);
  m_pLeafData->CreateNormals();
}
}
