/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SolidComposite.cpp,v $
Revision 1.1.10.1  2005/06/13 22:09:40  jay
Turn off spheres with compile switch

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream> // debug
#include <string>
#include "SolidComposite.h"
#include "CompositeFactory.h"
#include "Common.h"
#include "File.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Engine.h"
#include "Frustum.h"
#include "StringUtils.h"
#include "BoundingBox.h"
#include "SchAssert.h"

namespace Amju
{
SolidComposite::SolidComposite()
{
}

SolidComposite::~SolidComposite()
{
  //std::cout << "composite dtor, name: " << m_name.c_str() << "\n";
}

SolidComposite::SolidComposite(const SolidComposite& rhs) : SolidComponent(rhs)
{
  for (CompVector::const_iterator it = rhs.m_children.begin();
       it != rhs.m_children.end();
       ++it)
  {
    PSolidComponent pComp = (*it);
    PSolidComponent pClone = pComp->Clone();
    m_children.push_back(pClone);
  }
}

PSolidComponent SolidComposite::Clone()
{
  return new SolidComposite(*this);
}

void SolidComposite::Draw()
{
  for (CompVector::iterator it = m_children.begin();
       it != m_children.end();
       ++it)
  {
    SolidComponent* pComp = (*it).GetPtr();

    AmjuGL::PushMatrix();

    if (pComp)
    {
      pComp->Orientate();
      pComp->Draw();
    }

    AmjuGL::PopMatrix();
  }
}

void SolidComposite::Update()
{
  for (CompVector::iterator it = m_children.begin();
       it != m_children.end();
       ++it)
  {
    SolidComponent* pComp = (*it).GetPtr();
    pComp->Update();
  }
}

void SolidComposite::DrawSphere()
{
#ifdef DRAW_SPHERES
  if (Engine::Instance()->GetConfigValue("recursespheres") == "y")
  {
    for (CompVector::iterator it = m_children.begin();
         it != m_children.end();
         ++it)
    {
      SolidComponent* pComp = (*it).GetPtr();
      pComp->DrawSphere();
    }
  }
  m_bsphere.Draw();
#endif
}

int SolidComposite::CullDraw(const Frustum& f)
{
  if (!f.Contains(m_bsphere))
  {
    return 0;
  }

  // Counts actual no. of polygons drawn.
  int totalPolys = 0;
  for (CompVector::iterator it = m_children.begin();
       it != m_children.end();
       ++it)
  {
    SolidComponent* pComp = (*it).GetPtr();
    AmjuGL::PushMatrix();
    if (pComp)
    {
      pComp->Orientate();
      totalPolys += pComp->CullDraw(f);
    }
    AmjuGL::PopMatrix();
  }
  return totalPolys;
}

bool SolidComposite::Load(const std::string& filename)
{
  File jf;
  if (!jf.OpenRead(filename))
  {
    jf.ReportError("Couldn't open file.");
    return false;
  }

  m_name = filename;

  // Get the number of components
  int components = 0;
  if (!jf.GetInteger(&components))
  {
    jf.ReportError("Expected number of components.");
    return false;
  }
  
  // Load each component.
  for (int i = 0; i < components; i++)
  {
    // Get name of component. We don't know whether it's a leaf or composite.
    std::string strComponent;
    if (!jf.GetDataLine(&strComponent))
    {
      jf.ReportError("Expected component name.");
      return false;
    }
    
    PSolidComponent pChild = SolidComponent::LoadSolid(strComponent);
    if (!pChild.GetPtr())
    {
      std::string error = "Failed to load composite: " + strComponent;
      jf.ReportError(error);
      return false;
    }

    if (!pChild->LoadOrientation(&jf))
    {
      jf.ReportError("Failed to load orientation.");
      return false;
    }

    m_children.push_back(pChild);
  }

  return true;
}

void SolidComposite::CreateBoundingSphere(const Matrix& cm)
{
  // Get b sphere of each child. These will use absolute coords, so just
  // find a bigger sphere that encloses all the children.
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    PSolidComponent pc = m_children[i];
    Matrix m = cm;
    pc->GetOrientation()->TransformMatrix(&m);

    pc->CreateBoundingSphere(m);

    if (i == 0)
    {
      m_bsphere = *(pc->GetBoundingSphere());
    }
    else
    {
      m_bsphere  += *(pc->GetBoundingSphere());
    } 
  }
}

void SolidComposite::StoreHeights(const Matrix& cm)
{
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    SolidComponent* pc = m_children[i].GetPtr();
    Assert(pc);
    Matrix m = cm;
    pc->GetOrientation()->TransformMatrix(&m);
    pc->StoreHeights(m);
  }
}

void SolidComposite::AddHeights(HeightServer* pResult, const BoundingSphere& bs)
{
  // If this composite doesn't intersect the sphere, we are done - no children 
  // intersect it.
  if (!GetBoundingSphere()->Intersects(bs))
  {
    return;
  }

  // Add the HeightServer belonging to each child if the child intersects the
  // given bounding sphere.
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    SolidComponent* pc = m_children[i].GetPtr();
    Assert(pc);
    pc->AddHeights(pResult, bs);
  }
}

void SolidComposite::AddComponent(PSolidComponent pComp)
{
  m_children.push_back(pComp);
}

bool SolidComposite::DeleteComponent(SolidComponent* pComp)
{
  for (CompVector::iterator it = m_children.begin();
       it != m_children.end();
       ++it)
  {
    if ((*it).GetPtr() == pComp)
    { 
      m_children.erase(it);
      // Don't delete pComp - it's ref counted.
      return true;
    }
  }
  return false;
}

void SolidComposite::RecalculateAbsoluteCoords(const Matrix& cm)
{
  // Call this same method on each child, modifying the matrix.
  for (CompVector::iterator it = m_children.begin();
       it != m_children.end();
       ++it)
  {
    SolidComponent* pComp = (*it).GetPtr();
    // Copy matrix. Transform copy using orientation of current component.
    // Recalculate child absolute coords using this matrix.
    Matrix m = cm;
    pComp->GetOrientation()->TransformMatrix(&m);

    pComp->RecalculateAbsoluteCoords(m);
  }  
}

#if defined(SCENE_EDITOR)
bool SolidComposite::Save(bool recursive)
{
  if (IsDirty())
  {
    File jf;
    // Write name, but strip off the path if any.
    if (!jf.OpenWrite(GetName()))
    {
      return false;
    }
    // Write number of components
    jf.WriteComment("// Number of components");
    int numComponents = m_children.size();
    jf.WriteInteger(numComponents);
    jf.WriteComment(File::SEPARATOR);

    // For each component, write the name and orientation.
    for (int i = 0; i < numComponents; i++)
    {
      SolidComponent* pComp = m_children[i].GetPtr();
      char buf[200];
      sprintf(buf, "// Component %d follows.", i);
      jf.WriteComment(buf);
      // Strip off any path info in name of composite.
      jf.Write(StripPath(pComp->GetName()));
      pComp->SaveOrientation(&jf);
      jf.WriteComment(File::SEPARATOR);
    }
  }

  if (recursive)
  {
    int numComponents = m_children.size();
    for (int i = 0; i < numComponents; i++)
    {
      SolidComponent* pComp = m_children[i].GetPtr();
      pComp->Save(true);
    }
  }

  return true;
}
#endif

int SolidComposite::GetNumChildren() 
{ 
  return m_children.size(); 
}

PSolidComponent SolidComposite::GetChild(int i)
{
  return m_children[i];
}

int SolidComposite::GetNumberOfPolygons() const
{
  // To get the number of polys, add up the number of polys in each child.
  int total = 0;
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    SolidComponent* pc = m_children[i].GetPtr();
    total += pc->GetNumberOfPolygons();
  }
  return total;
}

bool SolidComposite::LineIntersects(const Mgc::Segment3& seg, float r) const
{
  if (!m_bsphere.Intersects(seg, r))
  {
    return false;
  }

  // Line seg intersects bounding sphere, so we must test against each child.
  int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    SolidComponent* pc = m_children[i].GetPtr();
    if (pc->LineIntersects(seg, r))
    {
      return true;
    }
  }
  return false;
}

bool SolidComposite::BoxIntersects(const BoundingBox& b) const
{
  if (!b.Intersects(m_bsphere))
  {
    return false;
  }

  const int size = m_children.size();
  for (int i = 0; i < size; i++)
  {
    const SolidComponent* pc = m_children[i].GetPtr();
    if (pc->BoxIntersects(b))
    {
      return true;
    }
  }
  return false;
}
}
