/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionCtrlPoints.cpp,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionCtrlPoints.h"
#include "File.h"
#include "Vertex.h"
#include "Engine.h"
#include "FunctionFactory.h"
#include "AngleCompare.h"
#include "SchAssert.h"

namespace Amju
{
/*
// Register class with Factory.
Function* CreateFunctionCtrlPoints() { return new FunctionCtrlPoints; }
static const bool isRegistered = FunctionFactory::Instance()->
  RegisterFactoryFunction(FunctionCtrlPoints::Name, CreateFunctionCtrlPoints);
*/

const char* FunctionCtrlPoints::Name = "points";

void FunctionCtrlPoints::Clear()
{
  m_points.clear();
}

void FunctionCtrlPoints::AddPoint(float time, const Orientation& o)
{
  m_points.push_back(std::make_pair(time, o));
}

#if defined(SCENE_EDITOR)
const char* FunctionCtrlPoints::GetTypeName() const
{
  return FunctionCtrlPoints::Name;
}

bool FunctionCtrlPoints::Save(File* pf)
{
  // Save the number of control points
  int numPoints = m_points.size();
  pf->WriteComment("// Number of control points");
  pf->WriteInteger(numPoints);

  // Save each control point.
  // Each control point consists of a time (float) and an orientation.
  for (int i = 0; i < numPoints; i++)
  {
    pf->WriteComment("");
    char buf[100];
    sprintf(buf, "// Point %d - time", i);
    pf->WriteComment(buf);
    TimePoint& tp = m_points[i];
    pf->WriteFloat(tp.first);

    pf->WriteComment("// Orientation");
    tp.second.Save(pf);
  }
  return true;
}
#endif

bool FunctionCtrlPoints::Load(File* pf)
{
  Clear();

  // Get the number of control points.
  int numCtrlPoints;
  if (!pf->GetInteger(&numCtrlPoints))
  {
    pf->ReportError("Expected number of control points.");
    return false;
  }
  // This kind of path requires at least 4 control points.
  if (numCtrlPoints < 4)
  {
    pf->ReportError("Required at least four control points.");
    return false;
  }
  // Load each control point.
  for (int i = 0; i < numCtrlPoints; i++)
  {
    // Each control point consists of a time (float) and an orientation.
    float t = 0;
    if (!pf->GetFloat(&t))
    {
      pf->ReportError("Expected control point time value.");
      return false;
    }
    if (i == 0)
    {
      // Time value for the first control point should be zero.
      if (t != 0)
      {
        pf->ReportError("Warning: first control point time should be 0.");
        // don't fail 
        t = 0;
      }
    }

    Orientation o;
    if (!o.Load(pf))
    {
      pf->ReportError("Failed to load orientation for control point.");
      return false;
    }
    m_points.push_back(TimePoint(t, o));
  }
  return true;
}

void FunctionCtrlPoints::Interp(TimePoint* pPoints, Orientation* pResult)
{
  Interpolate(m_time, pPoints, pResult);
}

void FunctionCtrlPoints::Recalc(Orientation* p)
{
  Assert(p);
  m_time += Engine::Instance()->GetDeltaTime();

  // We require at least 4 control points.
  if (m_points.size() < 4)
  {
    return;
  }

  // Iterate over control points until we find the current and next points.
  // TODO optimise this!
  
  // We need four control points to interpolate the spline curve.
  // point[1] is the last point before the current time. 
  // point[2] is the next point after the current time.
  // points[0] and [3] are the previous and next elements in the vector.
  TimePoint fourPoints[4];

  for (unsigned int i = 2; i < m_points.size() - 1; i++)
  {
    TimePoint& tp = m_points[i];
    float time = tp.first;
    if (time > m_time)
    {
      // We have found the next control point.
      fourPoints[0] = m_points[i-2];
      fourPoints[1] = m_points[i-1];
      fourPoints[2] = m_points[i];
      fourPoints[3] = m_points[i+1];

      Interp(fourPoints, p);
      return;
    }
  }
  // Didn't find the next control point. 
  m_time = 0;
}

int FunctionCtrlPoints::GetNumPoints() const
{
  return m_points.size();
}

void FunctionCtrlPoints::GetPoint(int index, float* pTimeResult, Orientation* pOrientationResult)
{
  Assert(index >= 0);
  Assert((unsigned int)index < m_points.size());

  *pTimeResult = m_points[index].first;
  *pOrientationResult = m_points[index].second;
}

void FunctionCtrlPoints::FaceDirection()
{
  int n = GetNumPoints() - 1;
  if (n < 1)
  {
    return;
  }
  for (int i = 0; i < n; i++)
  {
    Orientation& thisOr = m_points[i].second;
    Orientation& nextOr = m_points[i + 1].second;
    float yRot = AngleCompare::TurnToFace(thisOr.GetX(), thisOr.GetZ(), nextOr.GetX(), nextOr.GetZ());
    thisOr.SetYRot(RadToDeg(yRot));
  }
}
}

