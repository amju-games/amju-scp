/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Number3d.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_NUMBER_3D_H_INCLUDED)
#define SCHMICKEN_NUMBER_3D_H_INCLUDED

#include "Number.h"

namespace Amju
{
class Number3d : public Number
{
public:
  Number3d();

  // Load the digits and config values. Call once for this class.
  static bool Init();

  virtual void Draw();

  virtual void SetNumber(int n);

protected:
  // Digits 0 - 9.
  static PSolidComponent s_digits[10];

  static float s_digitWidth; // width of digit (same for all 10)

  // The digits for this number. Each list item points to one of the elements 
  // in the m_digits array.
  typedef std::list<PSolidComponent> DigitList;
  DigitList  m_digits;
};
}

#endif

