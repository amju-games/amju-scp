/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Orientation.cpp,v $
Revision 1.2.8.1  2005/05/08 08:11:21  jay
Added consts to functions where possible

Revision 1.2  2004/09/15 09:16:05  Administrator
Added ToString()

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Orientation.h"
#include "Common.h"
#include "File.h"
#include "Matrix.h"
#include "SchAssert.h"
#include "StringUtils.h"

namespace Amju
{
std::string ToString(const Orientation& o)
{
  std::string s;
  s += ToString(o.GetX());
  s += "|";
  s += ToString(o.GetY());
  s += "|";
  s += ToString(o.GetZ());
  s += "|";
  s += ToString(o.GetXRot());
  s += "|";
  s += ToString(o.GetYRot());
  s += "|";
  s += ToString(o.GetZRot());
  return s;
}

Orientation Interpolate(const Orientation& o1, const Orientation& o2, float f)
{
  Assert(f >= 0);
  Assert(f <= 1.0f);

  Orientation r;

  r.SetX(o1.GetX() + (o2.GetX() - o1.GetX()) * f);
  r.SetY(o1.GetY() + (o2.GetY() - o1.GetY()) * f);
  r.SetZ(o1.GetZ() + (o2.GetZ() - o1.GetZ()) * f);
  r.SetXRot(o1.GetXRot() + (o2.GetXRot() - o1.GetXRot()) * f);
  r.SetYRot(o1.GetYRot() + (o2.GetYRot() - o1.GetYRot()) * f);
  r.SetZRot(o1.GetZRot() + (o2.GetZRot() - o1.GetZRot()) * f);

  return r;
}

void Orientation::Draw() const
{
  AmjuGL::Translate(m_trx, m_try, m_trz);
  AmjuGL::RotateX(m_rx); //, 1.0f, 0.0f, 0.0f);
  AmjuGL::RotateY(m_ry); //, 0.0f, 1.0f, 0.0f);
  AmjuGL::RotateZ(m_rz); //, 0.0f, 0.0f, 1.0f);
}

void Orientation::DrawReverse() const
{
  AmjuGL::RotateX(m_rx); //, 1.0f, 0.0f, 0.0f);
  AmjuGL::RotateY(-m_ry); //, 0.0f, 1.0f, 0.0f);
  AmjuGL::RotateZ(m_rz); //, 0.0f, 0.0f, 1.0f);
  AmjuGL::Translate(m_trx, m_try, m_trz);
}

void Orientation::SetXRot(float degrees) 
{
  degrees = degrees - 360 * (int)(degrees / 360.0f);
  m_rx = degrees; 
}

void Orientation::SetYRot(float degrees) 
{
  degrees = degrees - 360 * (int)(degrees / 360.0f);
  m_ry = degrees; 
}

void Orientation::SetZRot(float degrees) 
{ 
  degrees = degrees - 360 * (int)(degrees / 360.0f);
  m_rz = degrees; 
}

bool Orientation::Load(File* jf)
{
  // Get translate x, y, z
  if (!jf->GetFloat(&m_trx))
  {
    jf->ReportError("Expected orientation X.");
    return false;    
  }
  if (!jf->GetFloat(&m_try))
  {
    jf->ReportError("Expected orientation Y.");
    return false;    
  }
  if (!jf->GetFloat(&m_trz))
  {
    jf->ReportError("Expected orientation Z.");
    return false;    
  }

  // Get rotate x, y, z
  if (!jf->GetFloat(&m_rx))
  {
    jf->ReportError("Expected orientation X rotation.");
    return false;    
  }
  if (!jf->GetFloat(&m_ry))
  {
    jf->ReportError("Expected orientation Y rotation.");
    return false;    
  }
  if (!jf->GetFloat(&m_rz))
  {
    jf->ReportError("Expected orientation Z rotation.");
    return false;    
  }
  
  return true;
}

#ifdef SCENE_EDITOR
bool Orientation::Save(File* jf)
{
  if (!jf->WriteComment("// Translation x, y, z"))
  {
    return false;
  }
  // Assume these next writes succeed, as the first one did.
  jf->WriteFloat(m_trx);
  jf->WriteFloat(m_try);
  jf->WriteFloat(m_trz);
  jf->WriteComment("// Rotation x, y, z");
  jf->WriteFloat(m_rx);
  jf->WriteFloat(m_ry);
  jf->WriteFloat(m_rz);

  return true;
}
#endif

float degToRad(float deg) { return deg / (float)(180.0 * pi); }

void Orientation::TransformMatrix(Matrix* m) const
{
    Matrix tr; // transformation matrix.
    // NB This is a problem if release Matrix ctor doesn't reset all elements!
    
    tr.Translate(Vec3f(m_trx, m_try, m_trz));
    (*m) = tr * (*m);
    
    tr.RotateX(degToRad(m_rx));
    (*m) = tr * (*m);
    
    tr.RotateY(degToRad(m_ry));
    (*m) = tr * (*m);
    
    tr.RotateZ(degToRad(m_rz));
    (*m) = tr * (*m);
}

Orientation& Orientation::operator+=(const Orientation& rhs)
{
  m_trx += rhs.m_trx;
  m_try += rhs.m_try;
  m_trz += rhs.m_trz;

  m_rx += rhs.m_rx;
  m_ry += rhs.m_ry;
  m_rz += rhs.m_rz;

  return *this;
}

Orientation& Orientation::operator-=(const Orientation& rhs)
{
  m_trx -= rhs.m_trx;
  m_try -= rhs.m_try;
  m_trz -= rhs.m_trz;

  m_rx -= rhs.m_rx;
  m_ry -= rhs.m_ry;
  m_rz -= rhs.m_rz;

  return *this;
}

bool Orientation::operator==(const Orientation& rhs) const
{
  return (
    (fabs(m_trx - rhs.m_trx) < SMALLEST) &&
    (fabs(m_try - rhs.m_try) < SMALLEST) &&
    (fabs(m_trz - rhs.m_trz) < SMALLEST) &&
    (fabs(m_rx - rhs.m_rx) < SMALLEST) &&
    (fabs(m_ry - rhs.m_ry) < SMALLEST) &&
    (fabs(m_rz - rhs.m_rz) < SMALLEST) );
}

bool Orientation::operator!=(const Orientation& rhs) const
{
  return !operator==(rhs);
}

Orientation operator+(const Orientation& lhs, const Orientation& rhs)
{
  Orientation o(lhs);
  o += rhs;

  return o;
}

Orientation operator-(const Orientation& lhs, const Orientation& rhs)
{
  Orientation o(lhs);
  o -= rhs;

  return o;
}

Vec3f Orientation::GetVertex() const
{
  return Vec3f(m_trx, m_try, m_trz);
}

void Orientation::SetVertex(const Vec3f& v)
{
  m_trx = v.x;
  m_try = v.y;
  m_trz = v.z;
}

bool Orientation::operator<(const Orientation& rhs) const
{
  if (m_trx < rhs.m_trx) 
    return true;

  if (m_trx > rhs.m_trx) 
    return false;

  if (m_try < rhs.m_try) 
    return true;

  if (m_try > rhs.m_try) 
    return false;

  if (m_trz < rhs.m_trz) 
    return true;

  if (m_trz > rhs.m_trz) 
    return false;

  if (m_rx < rhs.m_rx) 
    return true;

  if (m_rx > rhs.m_rx) 
    return false;

  if (m_ry < rhs.m_ry) 
    return true;

  if (m_ry > rhs.m_ry) 
    return false;

  return (m_rz < rhs.m_rz) ;
}
}
