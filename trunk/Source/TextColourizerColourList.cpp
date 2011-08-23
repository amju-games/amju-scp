/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextColourizerColourList.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#include "TextColourizerColourList.h"
#include "AttribDecorator.h"
#include "Attrib.h"

namespace Amju
{

TextColourizerColourList::TextColourizerColourList() : m_index(0)
{
}

/*
void GetColour(float* r, float* g, float* b)
{
  // Cycle through some colours, or choose one randomly.
  static const int NUM_COLOURS = 4;
  static float colours[NUM_COLOURS][3] = 
  {
    { 1.0f, 0, 0 },
    { 1.0f, 1.0f, 0 },
    { 0, 1.0f, 0 },
    { 0, 0.5f, 1.0f }
  };
  static int c = 0;
  *r = colours[c][0];
  *g = colours[c][1];
  *b = colours[c][2];

  ++c;
  if (c == NUM_COLOURS)
  {
    c = 0;
  }
}
*/

SolidComposite* TextColourizerColourList::ColourizeCharacter(
  SolidComponent* pChar)
{
  Assert(m_colours.size() > 0);

  m_index++;
  if (m_index == m_colours.size())
  {
    m_index = 0;
  }
  
  return TextColourizer::ColourizeCharacter(pChar);
}

PSolidComponent TextColourizerColourList::ColourizeElement(
  PSolidComponent pElem)
{
  AttribDecorator* pAd = new AttribDecorator;
  ColourAttrib* pCa = new ColourAttrib;

  Colour col = m_colours[m_index];

  pCa->Set(col.m_r, col.m_g, col.m_b, col.m_a); 
  pAd->SetAttrib(pCa);
  pAd->AddComponent(pElem.GetPtr()); 
  return pAd;
}

void TextColourizerColourList::AddColour(const Colour& c)
{
  m_colours.push_back(c);
}

}
