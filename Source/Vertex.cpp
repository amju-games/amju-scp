/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Vertex.cpp,v $
Revision 1.4.8.4  2005/09/26 07:57:11  jay
Add return value to Normalise() so we can try to find causes of
bad zero-length vectors

Revision 1.4.8.3  2005/07/06 20:20:10  jay
Test for 0 length in Normalise - possible Windows camera bug

Revision 1.4.8.2  2005/06/04 22:28:30  jay
Add option to set no of decimal places in ToString()

Revision 1.4.8.1  2005/05/09 05:54:54  jay
Use new version of ToString(float) to limit no of digits

Revision 1.4  2004/09/21 14:58:35  Administrator
Added ToString(), operator+() and operator-() for convenience.

Revision 1.3  2004/09/20 13:27:46  Administrator
Added Length() function

Revision 1.2  2004/09/17 12:50:19  Administrator
Removed public domain comment

Revision 1.1  2004/09/08 15:43:21  jay
Added to repository
  
*/

#include "Vertex.h"
#include "Common.h"
#include "File.h"
#include "Matrix.h"
#include "Geometry.h"
#include "StringUtils.h"
#include "SchAssert.h"

namespace Amju
{

float SquareDist(const Vec3f& v1, const Vec3f& v2)
{
  float xdiff = v1.x - v2.x;
  float ydiff = v1.y - v2.y;
  float zdiff = v1.z - v2.z;
  return (xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
}

std::string ToString(const Vec3f& v, int digits /* = 2 */)
{
  std::string s;
  s += ToString(v.x, digits);
  s += "|";
  s += ToString(v.y, digits);
  s += "|";
  s += ToString(v.z, digits);
  return s;
}

std::ostream& operator<<(std::ostream& os, const Vec3f& v)
{
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

/*
Vec3f::Vec3f() : x(0), y(0), z(0)
{
}

Vec3f::Vec3f(float x1, float y1, float z1) : x(x1), y(y1), z(z1)
{
}
*/

bool operator==(const Vec3f& lhs, const Vec3f& rhs) 
{
  return (fabs(lhs.x - rhs.x) < SMALLEST &&
          fabs(lhs.y - rhs.y) < SMALLEST &&
          fabs(lhs.z - rhs.z) < SMALLEST);
}

/*
bool Vec3f::operator<(const Vec3f& rhs) const
{
    if (fabs(x - rhs.x) < SMALLEST)
    {
        if (fabs(y - rhs.y) < SMALLEST)
        {
            if (fabs(z - rhs.z) < SMALLEST)
            {
                return false;
            }
            else
            {
                return z < rhs.z;
            }
        }
        else
        {
            return y < rhs.y;
        }
    }
    else
    {
        return x < rhs.x;
    }
}

const Vec3f operator+(const Vec3f& v1, const Vec3f& v2)
{
  Vec3f v(v1);
  v += v2;
  return v;
}

const Vec3f operator-(const Vec3f& v1, const Vec3f& v2)
{
  Vec3f v(v1);
  v -= v2;
  return v;
}

Vec3f& Vec3f::operator+=(const Vec3f& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vec3f& Vec3f::operator-=(const Vec3f& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vec3f& Vec3f::operator*=(float s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

bool Vec3f::Normalise()
{
  float len = (float) sqrt(x * x + y * y + z * z);

  if (len ==  0.0f)
  {
    // TODO TEMP TEST
    // Cause of Windows camera bug ?
    std::cout << "Normalise: can't normalize zero length vector!\n";
    //Assert(0);
    return false;
  }
  else
  {
    x/=len;
    y/=len;
    z/=len;
  }
  return true;
}

float Vec3f::Length() const
{
  float dsq = x * x + y * y + z * z;
  float d = sqrt(dsq);
  return d;
}
*/

SceneVertex::SceneVertex()
{
  m_abs[0] = 0;
  m_abs[1] = 0;
  m_abs[2] = 0;
  m_normal[0] = 0;
  m_normal[1] = 0;
  m_normal[2] = 0;
}

SceneVertex::SceneVertex(float x, float y, float z) : Vec3f(x, y, z)
{
  m_abs[0] = 0;
  m_abs[1] = 0;
  m_abs[2] = 0;
  m_normal[0] = 0;
  m_normal[1] = 0;
  m_normal[2] = 0;
}

SceneVertex::SceneVertex(const Vec3f& v) : Vec3f(v)
{
  m_abs[0] = 0;
  m_abs[1] = 0;
  m_abs[2] = 0;
  m_normal[0] = 0;
  m_normal[1] = 0;
  m_normal[2] = 0;
}

SceneVertex::~SceneVertex()
{
}

SceneVertex& SceneVertex::operator=(const SceneVertex& rhs)
{
  if (&rhs == this)
  {
    return *this;
  }
  
  m_abs[0] = rhs.m_abs[0];
  m_abs[1] = rhs.m_abs[1];
  m_abs[2] = rhs.m_abs[2];

  m_normal[0] = rhs.m_normal[0];
  m_normal[1] = rhs.m_normal[1];
  m_normal[2] = rhs.m_normal[2];

  x = rhs.x;
  y = rhs.y;
  z = rhs.z;

  return *this;
}

bool SceneVertex::Load(File* pf)
{
  if (!pf->GetFloat(&x)) 
  {
    pf->ReportError("Expected vertex X.");
    return false;
  }
  
  if (!pf->GetFloat(&y)) 
  {
    pf->ReportError("Expected vertex Y.");
    return false;
  }

  if (!pf->GetFloat(&z)) 
  {
    pf->ReportError("Expected vertex Z.");
    return false;
  }
 
  return true; 
}

#ifdef SCENE_EDITOR
bool SceneVertex::Save(File* pf)
{
  pf->WriteComment("// x, y, z");
  pf->WriteFloat(x);
  pf->WriteFloat(y);
  pf->WriteFloat(z);

  return true;
}
#endif

//void SceneVertex::DrawVertex()
//{
//  glNormal3fv(m_normal);  
//  glVertex3f(x, y, z);
//}

void SceneVertex::SetNormal(float x, float y, float z)
{
  m_normal[0] = x;
  m_normal[1] = y;
  m_normal[2] = z;
  
  // Make sure length of normal vector is 1.
  Geometry::Normalise(m_normal);
}

void SceneVertex::SetRelative(float x1, float y1, float z1)
{
  x = x1;
  y = y1;
  z = z1;
}

void SceneVertex::SetAbsolute(float x, float y, float z)
{
  m_abs[0] = x;
  m_abs[1] = y;
  m_abs[2] = z;
}

// Calculate the absolute values for the vertex from the relative ones.
void SceneVertex::CalcAbsoluteCoord(const Matrix& m)
{
  Vec3f v(x, y, z);
  Vec3f absolute = v * m;

  SetAbsolute(absolute.x, absolute.y, absolute.z);
}

// Get relative values from absolute ones.
bool SceneVertex::CalcRelativeCoord(const Matrix& cm)
{
  // Try to invert the transformation matrix. 
  Matrix inv;
  if (cm.Inverse(&inv))
  {
    Vec3f absolute(GetAbsX(), GetAbsY(), GetAbsZ());
    Vec3f relative = absolute * inv;
    SetRelative(relative.x, relative.y, relative.z);
    return true;
  }

  return false;
}

// ----------------------------------------------------------------------------

TexVertex::TexVertex()
{
  m_st[0] = 0.0;
  m_st[1] = 0.0;
}

TexVertex::TexVertex(float s, float t)
{
  m_st[0] = s;
  m_st[1] = t;
}

TexVertex::~TexVertex()
{
}

bool TexVertex::Load(File* pf)
{
  if (!pf->GetFloat(&m_st[0]))
  {
    pf->ReportError("Expected vertex S.");
    return false;
  }

  if (!pf->GetFloat(&m_st[1]))
  {
    pf->ReportError("Expected vertex T.");
    return false;
  }
  
  return true;
}
#ifdef SCENE_EDITOR
bool TexVertex::Save(File* pf)
{
  pf->WriteComment("// Texture (s, t)");
  pf->WriteFloat(s());
  pf->WriteFloat(t());

  return true;
}
#endif

//void TexVertex::DrawVertex()
//{
//  glTexCoord2fv(m_st); 
//}
}

