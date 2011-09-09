/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MovingSolid.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "MovingSolid.h"
#include "Common.h"
#include "SolidComponent.h"
#include "CompositeFactory.h"
#include "FunctionFactory.h"
#include "Matrix.h"
#include "Engine.h"
#include "File.h"
#include "StringUtils.h"
#include "FunctionRotate.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
MovingSolid::MovingSolid()
{
#if defined(SCENE_EDITOR)
  // New object must be saveable for editor.
  // This code same as SolidGameObject ctor.
  std::string defaultComp = Engine::Instance()->GetConfigValue("default_comp_name");
  if (defaultComp.empty())
  {
    defaultComp = "block1.comp";
  }
  m_pSolid = SolidComponent::LoadSolid(defaultComp);

  // Create default function
  m_pFunc = FunctionFactory::Instance()->Create(FunctionRotate::Name); // any valid name will do
#endif
}

MovingSolid::~MovingSolid()
{
}

MovingSolid::MovingSolid(const MovingSolid& rhs) : SolidComponent(rhs)
{
  m_pSolid = rhs.m_pSolid->Clone();
  m_pFunc = rhs.m_pFunc;
}

PSolidComponent MovingSolid::Clone()
{
  return new MovingSolid(*this);
}

PSolidComponent MovingSolid::GetInnerSolid()
{
  return m_pSolid;
}

bool MovingSolid::Load(const std::string& filename)
{
  File msFile;
  if (!msFile.OpenRead(filename))
  {
    msFile.ReportError("Couldn't open file.");
    return false;
  }
  if (!Load(&msFile))
  {
    msFile.ReportError("Failed to load platform.");
    return false;
  }

#if defined(SCENE_EDITOR)
  m_name = filename;
#endif

  return true;
}

#if defined(SCENE_EDITOR)
bool MovingSolid::Save(bool recursive)
{
  File f;
  if (!f.OpenWrite(GetName()))
  {
    f.ReportError("Couldn't open for writing.");
    return false;
  }

  f.WriteComment("// Solid name");
  f.Write(StripPath(m_pSolid->GetName()));

  f.WriteComment("// Initial orientation");
  m_pSolid->GetOrientation()->Save(&f);

  f.WriteComment("// Function name");
  f.Write(m_pFunc->GetTypeName());

  f.WriteComment("// Function-specific data follows");
  m_pFunc->Save(&f);

  if (recursive)
  {
    m_pSolid->Save(true);
  }

  return true;
}
#endif

bool MovingSolid::Load(File* pf)
{
  Assert(pf);

  // Get the Solid name.
  string solidName;
  if (!pf->GetDataLine(&solidName))
  {
    pf->ReportError("Expected moving solid name.");
    return false;
  }

  if (solidName.empty())
  {
    pf->ReportError("Bad moving solid name.");
    return false;
  }
  // Create the solid.
  m_pSolid = SolidComponent::LoadSolid(solidName);
  if (!m_pSolid.GetPtr())
  {
    pf->ReportError("Failed to load moving solid.");
    return false;
  }

  // Load initial orientation for the solid.
  if (!m_pSolid->LoadOrientation(pf))
  {
    return false;
  }

  // Get function name
  string fnName;
  if (!pf->GetDataLine(&fnName))
  {
    pf->ReportError("Expected moving solid function name.");
    return false;
  }

  if (fnName.empty())
  {
    pf->ReportError("Empty moving solid function name.");
    return false;
  }

  m_pFunc = FunctionFactory::Instance()->Create(fnName);

  if (!m_pFunc.GetPtr())
  {
    pf->ReportError("Bad moving solid function name.");
    return false;
  }
  if (!m_pFunc->Load(pf))
  {
    return false;
  }

  return true;
}

void MovingSolid::Draw()
{
  Assert(m_pSolid.GetPtr());
  if (m_pSolid.GetPtr())
  {
    AmjuGL::PushMatrix();

    m_pSolid->Orientate(); // So recalculated orientation is used.
    m_pSolid->Draw();

    AmjuGL::PopMatrix();
  }
}

int MovingSolid::CullDraw(const Frustum& f)
{
  Assert(m_pSolid.GetPtr());
  if (m_pSolid.GetPtr())
  {
    return m_pSolid->CullDraw(f);
  }
  return 0;
}

void MovingSolid::DrawSphere()
{
  // We must recalculate the centre of the bounding sphere. So we add the
  // change in position.
  if (m_pSolid.GetPtr())
  {
    Vec3f v = m_pSolid->GetBoundingSphere()->GetCentre();
    Vec3f delta(m_orientationDelta.GetX(),
                     m_orientationDelta.GetY(),
                     m_orientationDelta.GetZ() );
    v += delta;

    m_pSolid->GetBoundingSphere()->SetCentre(v);

    m_pSolid->DrawSphere();
  }
}

void MovingSolid::Update()
{
  Assert(m_pFunc.GetPtr());
  Assert(m_pSolid.GetPtr());

  // Get new orientation, and work out position delta.
  Orientation oldO = *(m_pSolid->GetOrientation());
  m_pFunc->Recalc(m_pSolid->GetOrientation());
  m_orientationDelta = *(m_pSolid->GetOrientation()) - oldO;

  // TODO TEMP TEST make orientation accessible..?
  //m_or = *(m_pSolid->GetOrientation());

  m_pSolid->Update();
}

void MovingSolid::GetOrientationDelta(Orientation* pOrientation)
{
  *pOrientation = m_orientationDelta;
}

int MovingSolid::GetNumberOfPolygons() const
{
  Assert(m_pSolid.GetPtr());
  if (m_pSolid.GetPtr())
  {
    return m_pSolid->GetNumberOfPolygons();
  }
  return 0;
}

void MovingSolid::CreateBoundingSphere(const Matrix& cm)
{
  if (m_pSolid.GetPtr())
  {
    Matrix m = cm;
    // Get initial transformation matrix
    m_pSolid->GetOrientation()->TransformMatrix(&m);

    m_pSolid->CreateBoundingSphere(m);
  }
}

BoundingSphere* MovingSolid::GetBoundingSphere()
{
  if (m_pSolid.GetPtr())
  {
    return m_pSolid->GetBoundingSphere();
  }
  return 0;
}

void MovingSolid::RecalculateAbsoluteCoords(const Matrix& cm)
{
  Assert(m_pSolid.GetPtr());
  m_pSolid->RecalculateAbsoluteCoords(cm);
}

void MovingSolid::AddHeights(HeightServer* pResult, const BoundingSphere& bs)
{
  Assert(m_pSolid.GetPtr());
  m_pSolid->AddHeights(pResult, bs);
}

void MovingSolid::StoreHeights(const Matrix& cm)
{
  Assert(m_pSolid.GetPtr());
  m_pSolid->StoreHeights(cm);
}

bool MovingSolid::LineIntersects(const Mgc::Segment3& seg, float r) const
{
  Assert(m_pSolid.GetPtr());
  return m_pSolid->LineIntersects(seg, r);
}
}

