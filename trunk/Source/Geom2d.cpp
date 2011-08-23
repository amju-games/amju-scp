/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Geom2d.cpp,v $
Revision 1.1.10.2  2005/09/26 07:57:10  jay
Add return value to Normalize() so we can try to find causes of
bad zero-length vectors

Revision 1.1.10.1  2005/06/13 22:11:32  jay
Additions/fixes after using this class in Pool AI

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Geom2d.h"
#include "Common.h"
#include "StringUtils.h"
#include "SchAssert.h"

namespace Amju
{
namespace Geom2d
{
std::string ToString(const Point2d& p) 
{
  std::string s;
  s += Amju::ToString(p.x, 2);
  s += ", ";
  s += Amju::ToString(p.y, 2);
  return s;
}

bool Point2d::operator==(const Point2d& rhs) const
{
  return (fabs(x - rhs.x) < SMALLEST && fabs(y - rhs.y) < SMALLEST);
}

bool Line2d::operator==(const Line2d& rhs) const
{
  return (pt1 == rhs.pt1 && pt2 == rhs.pt2  ||  pt1 == rhs.pt2 && pt2 == rhs.pt1); 
}

Point2d Point2d::operator+(const Point2d& rhs) const
{
  return Point2d(x + rhs.x, y + rhs.y);
}

Point2d Point2d::operator-(const Point2d& rhs) const
{
  return Point2d(x - rhs.x, y - rhs.y);
}

Point2d& Point2d::operator*=(float f)
{
  x *= f;
  y *= f;
  return *this;
}

float Line2d::Length() const
{
  return (pt1 - pt2).Magnitude();
}

float DotProduct(const Vec2d& p1, const Vec2d& p2)
{
  return (p1.x * p2.x  +  p1.y * p2.y);
}

bool Point2d::Normalize()
{
  const float len = (float) sqrt(x * x + y * y);
  if (len == 0)
  {
    return false;
  }
  x /= len;
  y /= len;
  return true;
}

float Point2d::Magnitude() const
{
  return  (float) sqrt(x * x + y * y);
}

// This 2D line intersection algorithm is from the comp.graphics.algorithms FAQ.
bool Intersects(const Line2d& line1, const Line2d& line2, Point2d* pResult)
{
  const float Ax = line1.pt1.x;
  const float Ay = line1.pt1.y;
  const float Bx = line1.pt2.x;
  const float By = line1.pt2.y;
  const float Cx = line2.pt1.x;
  const float Cy = line2.pt1.y;
  const float Dx = line2.pt2.x;
  const float Dy = line2.pt2.y;

  // Denominator
  float d = (Bx - Ax) * (Dy - Cy) - (By - Ay) * (Dx - Cx);

  if (fabs(d) < SMALLEST)
  {
    // Parallel
    return false;
  }

  // Get unknowns
  float ua = ((Ay - Cy) * (Dx - Cx) - (Ax - Cx) * (Dy - Cy) ) / d;
  float ub = ((Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay) ) / d;

  if (ua < 0 || ua > 1 || ub < 0 || ub > 1)
  {
    return false;
  }

  pResult->x = Ax + ua * (Bx - Ax);
  pResult->y = Ay + ua * (By - Ay);

  return true;
}
}
}
