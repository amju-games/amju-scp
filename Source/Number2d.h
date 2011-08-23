/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Number2d.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_NUMBER_2D_H_INCLUDED)
#define SCHMICKEN_NUMBER_2D_H_INCLUDED

#include "Number.h"
#include "TexturedQuad.h"

namespace Amju
{
class Number2d : public Number
{
public:
  Number2d();

  // Load the digits and config values. Call once for this class.
  static bool Init();

  virtual void Draw();
  virtual void SetNumber(int n);

  // Set overlay coords. 
  void SetPosition(float top, float left);

  // Set width and height (in overlay units) for 1 digit.
  void SetDigitSize(float width, float height);

protected:
  // Digits 0 - 9.
  static TexturedQuad* s_digits[10];

  // The digits for this number. Each list item points to one of the elements 
  // in the m_digits array.
  typedef std::list<TexturedQuad* > DigitList;
  DigitList  m_digits;

  // Position
  float m_top;
  float m_left;
  // Width/height of digits
  float m_width;
  float m_height;
};
}

#endif

