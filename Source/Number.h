/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Number.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(NUMBER_H_INCLUDED)
#define NUMBER_H_INCLUDED

#include "SolidComponent.h"
#include <list>

namespace Amju
{
// Base class for numbers. Subclasses hold static arrays of digits. 
class Number
{
public:
  Number();
  virtual ~Number() {}

  virtual void Draw() = 0;

  // Work out the list of digits and initialise for drawing.
  // Call per-object before drawing the number.
  virtual void SetNumber(int n) = 0;

  int GetNumber() const;

  enum Justification { CENTRE, LEFT, RIGHT };
  void SetJustify(Justification j);

protected:
  int m_number;

  // Justification - left, right or centre
  Justification m_justify;
};
}
#endif
