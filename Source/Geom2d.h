/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Geom2d.h,v $
Revision 1.1.10.2  2005/09/26 07:57:11  jay
Add return value to Normalize() so we can try to find causes of
bad zero-length vectors

Revision 1.1.10.1  2005/06/13 22:11:32  jay
Additions/fixes after using this class in Pool AI

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(GEOM_2D_H_INCLUDED)
#define GEOM_2D_H_INCLUDED

#include <string>

namespace Amju
{
// 2D Geometry classes
namespace Geom2d
{
struct Point2d
{
  Point2d() : x(0), y(0) {}
  Point2d(float x, float y) : x(x), y(y) {}
  float x, y;

  bool operator==(const Point2d& rhs) const;
  float Magnitude() const;

  Point2d operator+(const Point2d& rhs) const;
  Point2d operator-(const Point2d& rhs) const;
  bool Normalise();
  Point2d& operator*=(float);
};
 
std::string ToString(const Point2d&);

struct Vec2d : public Point2d 
{
  Vec2d(const Point2d& pt) : Point2d(pt.x, pt.y) {}
  Vec2d(float x, float y) : Point2d(x, y) {}
};

struct Line2d
{
  Line2d(const Point2d& p1, const Point2d& p2) : pt1(p1), pt2(p2) {}
  Point2d pt1, pt2;

  // Equality: two lines are equal if they share the same points. 
  bool operator==(const Line2d& rhs) const;

  float Length() const;
};

// Returns true if the two given lines intersect. In this case *pResult is set
// to the intersection point.
bool Intersects(const Line2d& line1, const Line2d& line2, Point2d* pResult);

// 2D Dot product. 
float DotProduct(const Vec2d& p1, const Vec2d& p2);
}
}
#endif

