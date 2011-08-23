/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ButtonCreator.h"
#include "File.h"
#include "GuiTextButton.h"

namespace Amju
{
ButtonCreator::ButtonCreator()
{
  m_w = 1.0f;
  m_h = 1.0f;
  m_pFont = 0;
  m_fontSize = 1.0f;
}

void ButtonCreator::SetText(const std::string& text)
{
  m_text = text;
}

void ButtonCreator::SetFont(Font* pFont)
{
  m_pFont = pFont;
}

void ButtonCreator::SetFontSize(float size)
{
  m_fontSize = size;
}

void ButtonCreator::SetFontColour(const Colour& c)
{
  m_fontColour = c;
}

void ButtonCreator::SetButtonWidthHeight(float w, float h)
{
  m_w = w;
  m_h = h;
}

void ButtonCreator::SetButtonFilename(const std::string& filename)
{ 
  m_filename = filename;
}

GuiElement* ButtonCreator::Create()
{ 
  GuiTextButton* p = new GuiTextButton;
  File f;
  if (!f.OpenRead(m_filename))
  { 
    f.ReportError("Failed to open button file for menu");
    return 0;
  }
  if (!p->Load(&f))
  { 
    f.ReportError("Failed to load GuiTextButton for menu");
    return 0;
  }
  p->SetSize(m_w, m_h);
  p->SetFont(m_pFont);
  p->SetText(m_text);
  p->SetFontSize(m_fontSize);
  p->SetFontColour(m_fontColour);
  return p;
}

}

