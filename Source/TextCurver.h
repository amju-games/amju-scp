/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextCurver.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(TEXT_CURVER_H_INCLUDED)
#define TEXT_CURVER_H_INCLUDED

namespace Amju
{
// Use by TextFactory to place text in curves.
class TextCurver
{
public:
  TextCurver();

  // Set initial orientation of text.
  // 0 => regular horizontal, 90 => text goes up, -90 => down etc.
  void SetInitialRot(float);

  // Set severity of text curvature. 0 => no curve.
  // Curve on which letters are placed curves through x * deg degrees.
  void SetCurvature(float deg); 

  // You give an x-value. This is curved to give an (x, z) coord and
  // angle at which the text should be rotated at that point.
  // If x == 0, the result is (x = 0, z = 0, rot = initial rot).
  void Curve(float x, float* pX, float* pZ, float* pRot);

protected:
  float m_initialRot;
  float m_curvature;
};
}

#endif

