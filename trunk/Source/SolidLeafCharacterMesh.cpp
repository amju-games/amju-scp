/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidLeafCharacterMesh.cpp,v $
Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#include "SolidLeafCharacterMesh.h"
#include "Character.h"
#include "Frustum.h"
#include "File.h"
#include "CharacterManager.h"

namespace Amju
{
SolidLeafCharacterMesh::~SolidLeafCharacterMesh()
{
}

PSolidComponent SolidLeafCharacterMesh::Clone()
{
  return new SolidLeafCharacterMesh(*this);
}

void SolidLeafCharacterMesh::Draw()
{
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);

  m_pMesh->Draw();
}

void SolidLeafCharacterMesh::DrawSphere()
{
  m_pSimpleSolid->DrawSphere();
}

int SolidLeafCharacterMesh::CullDraw(const Frustum& f)
{
  if (!f.Contains(*(m_pSimpleSolid->GetBoundingSphere())))
  {
    return 0;
  }
  Draw();
  return GetNumberOfPolygons();  
}

bool SolidLeafCharacterMesh::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }
  
  // Get the mesh name. Get a new mesh of this type from the character manager.
  if (!f.GetDataLine(&m_meshName))
  {
    f.ReportError("Expected mesh name.");
    return false;
  }
  m_pMesh = CharacterManager::Instance()->GetCharacter(m_meshName);
  if (!m_pMesh.GetPtr())
  {
    f.ReportError("Bad mesh name.");
    return false;
  }

  std::string simpleSolidName;
  if (!f.GetDataLine(&simpleSolidName))
  {
    f.ReportError("Expected simple solid name.");
    return false;
  }
  // Get simple volume for collision detection.
  m_pSimpleSolid = SolidComponent::LoadSolid(simpleSolidName);
  if (!m_pSimpleSolid.GetPtr())
  {
    f.ReportError("Bad simple solid name.");
    return false;
  }

  SetName(filename);

  return true;
}

int SolidLeafCharacterMesh::GetNumberOfPolygons() const
{
#if defined(_DEBUG)
  return m_pMesh->GetNumberOfPolygons();
#else
  return 0;
#endif
}

void SolidLeafCharacterMesh::StoreHeights(const Matrix& cm)
{
  m_pSimpleSolid->StoreHeights(cm);
}

void SolidLeafCharacterMesh::AddHeights(HeightServer* pResult, const BoundingSphere& bs)
{
  m_pSimpleSolid->AddHeights(pResult, bs);
}

bool SolidLeafCharacterMesh::IsLeaf() const
{
  return true;
}

void SolidLeafCharacterMesh::CreateBoundingSphere(const Matrix& cm)
{
  m_pSimpleSolid->CreateBoundingSphere(cm);
}

bool SolidLeafCharacterMesh::LineIntersects(const Mgc::Segment3& seg, float r) const
{
  return m_pSimpleSolid->LineIntersects(seg, r);
}

void SolidLeafCharacterMesh::RecalculateAbsoluteCoords(const Matrix& cm)
{
  m_pSimpleSolid->RecalculateAbsoluteCoords(cm);
}

bool SolidLeafCharacterMesh::BoxIntersects(const BoundingBox& b) const
{
  return m_pSimpleSolid->BoxIntersects(b);
}

#if defined(SCENE_EDITOR)
bool SolidLeafCharacterMesh::Save(bool recursive)
{
  if (IsDirty())
  {
    File jf;  
    if (!jf.OpenWrite(GetName()))
    {
      return false;
    }
    jf.Write(m_meshName);
    jf.Write(m_pSimpleSolid->GetName());
    if (recursive)
    {
      if (!m_pSimpleSolid->Save(true))
      {
        return false;
      }
    }
  }  
  return true;
}
#endif

}

