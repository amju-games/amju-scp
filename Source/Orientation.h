/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Orientation.h,v $
Revision 1.2.8.1  2005/05/08 08:11:21  jay
Added consts to functions where possible

Revision 1.2  2004/09/15 09:16:05  Administrator
Added ToString()

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(ORIENTATION_H_INCLUDED)
#define ORIENTATION_H_INCLUDED

#include <string>
#include "Vertex.h"

namespace Amju
{
class File;
class Matrix;

class Orientation
{
  // Create an Orientation which is between o1 and o2.
  // 0 <= f <= 1.0.
  // If f == 0, result is o1; if f == 1, result is o2.
  friend Orientation Interpolate(const Orientation& o1, const Orientation& o2, float f);

public:
  Orientation() : 
    m_trx(0.0), m_try(0.0), m_trz(0.0), m_rx(0.0), m_ry(0.0), m_rz(0.0) {}

  Orientation(float x, float y, float z, float rx, float ry, float rz) :
    m_trx(x), m_try(y), m_trz(z), m_rx(rx), m_ry(ry), m_rz(rz) {}

  void Draw() const;

  void DrawReverse() const;

  bool Load(File* jf);

#ifdef SCENE_EDITOR
  bool Save(File* jf);
#endif

  // Transform the given matrix, so that the matrix can be used to effect
  // this orientation.
  void TransformMatrix(Matrix* m) const;

  void SetX(float x) { m_trx = x; }
  void SetY(float y) { m_try = y; }
  void SetZ(float z) { m_trz = z; }

  void SetXRot(float degrees);
  void SetYRot(float degrees);
  void SetZRot(float degrees);

  float GetX() const { return m_trx; }
  float GetY() const { return m_try; }
  float GetZ() const { return m_trz; }

  // Get the vertex.
  VertexBase GetVertex() const;
  void SetVertex(const VertexBase&);

  float GetXRot() const { return m_rx; }
  float GetYRot() const { return m_ry; }
  float GetZRot() const { return m_rz; }

  // 'Snap' translation values to nearest multiples of snapTr, and
  // rotation values to nearest multiple of snapRot.
  void SnapTo(float snapTr, float snapRot);

  // Add translation part of rhs to translation part of this object,
  // and add rotation part of rhs to rotation part of this.
  Orientation& operator+=(const Orientation& rhs);
  Orientation& operator-=(const Orientation& rhs);

  friend Orientation operator+(const Orientation& lhs, const Orientation& rhs);
  friend Orientation operator-(const Orientation& lhs, const Orientation& rhs);

  bool operator==(const Orientation& rhs) const;
  bool operator!=(const Orientation& rhs) const;
  bool operator<(const Orientation& rhs) const;

private:
  // Translation
  float m_trx;
  float m_try;
  float m_trz;

  // Rotation
  float m_rx;
  float m_ry;
  float m_rz;
};

std::string ToString(const Orientation& o);
}
#endif

