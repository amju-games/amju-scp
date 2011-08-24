/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: GuiText.cpp,v $
Revision 1.1.2.4  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.2  2006/01/16 13:27:55  jay
Add new GuiElements for future use

*/

#include "GuiText.h"
#include "Engine.h"
#include "TextWriter.h"

namespace Amju
{
GuiText::GuiText() : m_pFont(0)
{
}

void GuiText::DrawImpl()
{
#ifdef TEXT_DRAW_DEBUG
std::cout << "GuiText::DrawImpl\n";
#endif

  // TODO Stencil/scissor ? buffer, so text never overflows rectangle ?
  Font* pFont = m_pFont;
  if (!pFont)
  {
    pFont = Engine::Instance()->GetTextWriter()->GetDefaultFont();
  }
  Assert(pFont);
  float top = m_absTop;
  for (unsigned int i = 0; i < m_strings.size(); i++)
  {
    pFont->PrintNoBlend(m_absLeft, top, m_strings[i].c_str());
    top += 0.8f; // TODO depends on font size
  }
}

void GuiText::MousePos(int, int) 
{
}

void GuiText::MouseButton(bool, bool, bool)
{
}

void GuiText::SetText(const std::string& s)
{
  // TODO Split text so it wraps in rectangle.
  m_strings.clear();
  m_strings.push_back(s);
}

void GuiText::SetFont(Font* pFont)
{
  m_pFont = pFont;
}

}


