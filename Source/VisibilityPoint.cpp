/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibilityPoint.cpp,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "VisibilityPoint.h"
#include "VisibilityGraph.h"
#include "File.h"
#include "Frustum.h"
#include "Polygon.h"

namespace Amju
{
void VisibilityPoint::AddVisibilityPoint(VisibilityGraph* pVg) const
{
  pVg->AddPoint(m_bsphere.GetCentre());
}

PSolidComponent VisibilityPoint::Clone()
{
  return new VisibilityPoint(*this);
}

void VisibilityPoint::Draw()
{
#if defined(_DEBUG)
  // GL has already tranformed the coords, so we just want to show a sphere
  // located at the local origin.
  BoundingSphere bsphere(VertexBase(0, 0, 0), 0.5f);
  bsphere.Draw();
#endif
}

int VisibilityPoint::CullDraw(const Frustum& f)
{
#if defined(_DEBUG) || defined(SCENE_EDITOR)
  Draw();
#endif
  return 0;
}

bool VisibilityPoint::Load(const std::string& filename)
{
#if defined(SCENE_EDITOR)
  m_name = filename;
#endif

  // There's nothing to load! VisibilityPoint 'files' don't really exist!

  // This sphere gets it centre transformed because it's a SolidComposite node.
  m_bsphere.SetCentre(VertexBase(0, 0, 0));
  m_bsphere.SetRadius(0); 
  return true;
}

#ifdef SCENE_EDITOR
bool VisibilityPoint::Save(bool)
{
  // Nothing to save!
  return true;
}
#endif

void VisibilityPoint::StoreHeights(const Matrix& cm)
{
  RecalculateAbsoluteCoords(cm);
}

void VisibilityPoint::RecalculateAbsoluteCoords(const Matrix& cm)
{
  SceneVertex sv;
  sv.CalcAbsoluteCoord(cm);
  m_bsphere.SetCentre(VertexBase(sv.GetAbsX(), sv.GetAbsY(), sv.GetAbsZ()));
  m_bsphere.SetRadius(0);
}

void VisibilityPoint::CreateBoundingSphere(const Matrix& cm)
{
  RecalculateAbsoluteCoords(cm);
}
}
