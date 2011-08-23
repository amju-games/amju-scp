/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextColourizerColourList.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_TEXT_COLOURIZER_COLOUR_LIST_H_INCLUDED)
#define SCHMICKEN_TEXT_COLOURIZER_COLOUR_LIST_H_INCLUDED

#include <vector>
#include "TextColourizer.h"
#include "Colour.h"

namespace Amju
{
// Colour text by giving each consecutive character a colour from a
// list.
class TextColourizerColourList : public TextColourizer
{
public:
  TextColourizerColourList();
  virtual SolidComposite* ColourizeCharacter(SolidComponent* pChar);

  virtual PSolidComponent ColourizeElement(PSolidComponent pCharacter);

  // Add to list of colours which are cycled through to decorate characters.
  void AddColour(const Colour& c);

protected:
  std::vector<Colour> m_colours;
  int m_index; // current colour in vector
};
}

#endif

