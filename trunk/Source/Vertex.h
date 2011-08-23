/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Vertex.h,v $
Revision 1.3.8.2  2005/09/26 07:57:11  jay
Add return value to Normalize() so we can try to find causes of
bad zero-length vectors

Revision 1.3.8.1  2005/06/04 22:28:30  jay
Add option to set no of decimal places in ToString()

Revision 1.3  2004/09/21 14:58:35  Administrator
Added ToString(), operator+() and operator-() for convenience.

Revision 1.2  2004/09/20 13:27:45  Administrator
Added Length() function

Revision 1.1  2004/09/08 15:43:21  jay
Added to repository
  
*/

#if !defined(VERTEX_H_INCLUDED)
#define VERTEX_H_INCLUDED

#include "Matrix.h"
#include <iostream>
#include <string>

namespace Amju
{
class VertexBase
{
  friend std::ostream& operator<<(std::ostream&, const VertexBase&);

  friend const VertexBase operator+(const VertexBase& v1, const VertexBase& v2);
  friend const VertexBase operator-(const VertexBase& v1, const VertexBase& v2);
  friend std::string ToString(const VertexBase& v, int decimalPlaces = 2);

public:
  VertexBase();
  VertexBase(float x, float y, float z);

//  inline float x() const { return x; }
//  inline float y() const { return y; }
//  inline float z() const { return z; }

  bool operator==(const VertexBase& rhs) const;
  bool operator!=(const VertexBase& rhs) const { return !operator==(rhs); }

  // Less-than, for STL.
  bool operator<(const VertexBase& rhs) const;

  VertexBase& operator+=(const VertexBase& rhs);
  VertexBase& operator-=(const VertexBase& rhs);
  VertexBase& operator*=(float s);

  // Returns false if length is zero; else returns true, and length 
  // will be 1.0.
  bool Normalize();

  // "Snap to Grid": set x, y and z to nearest multiple of 'snap'.
  void SnapTo(float snap);

  float Length() const;

public:
  float x, y, z;
};

class File;
class HeightServer;
class Matrix;

class SceneVertex : public VertexBase
{
public:
  SceneVertex();
  SceneVertex(float x, float y, float z);
  SceneVertex(const VertexBase& v);
  ~SceneVertex();

  SceneVertex& operator=(const SceneVertex& rhs);

  bool Load(File* pf);

#ifdef SCENE_EDITOR
  bool Save(File* pf);
#endif

  // Draw the vertex using OpenGL.
  void DrawVertex();

  // Calculate the absolute coords for this vertex, given a
  // transformation matrix.
  void CalcAbsoluteCoord(const Matrix& m);

  // Calculate the relative coord for this matrix from the absolute coord:
  // the matrix which transforms rel. to abs. is supplied.
  bool CalcRelativeCoord(const Matrix& m);

  inline float GetNormalX() const { return m_normal[0]; }
  inline float GetNormalY() const { return m_normal[1]; }
  inline float GetNormalZ() const { return m_normal[2]; }

  void SetNormal(float x, float y, float z);
  void SetRelative(float x, float y, float z);
  void SetAbsolute(float x, float y, float z);

  inline float GetAbsX() const { return m_abs[0]; }
  inline float GetAbsY() const { return m_abs[1]; }
  inline float GetAbsZ() const { return m_abs[2]; }

private:
  float m_abs[3]; // Absolute world coords.
  float m_normal[3]; // normal at this vertex.
};

float SquareDist(const VertexBase& v1, const VertexBase& v2);

// Vertex with texture (s, t) coords.
class TexVertex 
{
public:
  TexVertex();
  TexVertex(float s, float t);

  ~TexVertex();

  bool Load(File*);

#ifdef SCENE_EDITOR
  bool Save(File* pf);
#endif

  void DrawVertex();

  inline float s() { return m_st[0]; }
  inline float t() { return m_st[1]; }

  void SetS(float s) { m_st[0] = s; }
  void SetT(float t) { m_st[1] = t; }

  //virtual bool hasTexture() { return true; }

private:
  float m_st[2];  // texture coords

};
}
#endif

