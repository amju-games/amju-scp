/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextColourizer.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_TEXT_COLOURIZER_H_INCLUDED)
#define SCHMICKEN_TEXT_COLOURIZER_H_INCLUDED

#include "RCPtr.h"
#include "SolidComponent.h"
#include "SolidComposite.h"

namespace Amju
{
class TextColourizer : public RefCounted
{
public:
  virtual ~TextColourizer() {}

  // Override to reset any counter etc, then call base class implementation.
  virtual SolidComposite* ColourizeCharacter(SolidComponent* pChar);

  // Implement in subclasses to decorate one piece of a character.
  virtual PSolidComponent ColourizeElement(PSolidComponent pCharacter) = 0;
};
}

#endif

