/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiTextButton.cpp,v $
Revision 1.1.10.2  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.9  2007/02/27 10:23:30  jay
Font colour for text buttons

Revision 1.8  2007/02/02 14:19:34  jay
Fail to Load if font info is missing: Glue Files do not allow for optional lines

Revision 1.7  2007/01/08 21:09:55  jay
Centre text; load font from file

Revision 1.6  2006/12/17 20:08:13  jay
Fix for when not selectable

Revision 1.5  2006/12/15 12:41:41  jay
Load localised string

Revision 1.4  2006/12/12 19:30:38  jay
Adjust text placement

Revision 1.3  2006/12/03 21:24:15  jay
Tweak to text position, probably will not work for different text or
 button sizes

Revision 1.2  2006/01/16 13:25:51  jay
Merge improvements from pool; now can set Font

Revision 1.1.10.1  2005/10/21 17:47:48  jay
Add font-related members - currently still problems with placing the text.

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "GuiTextButton.h"
#include "File.h"
#include "Engine.h"
#include "TextWriter.h"

//#define GUI_TEXT_BUTTON_DEBUG

namespace Amju
{
GuiTextButton::GuiTextButton()
{
  m_height = 1.0f;
  m_pFont = 0;
  m_textWidth = 0;
  m_fontSize = 1.0f;
}

bool GuiTextButton::Load(File* pf)
{
  if (!GuiButton::Load(pf)) 
  {
    return false;
  } 

  std::string text; 
  if (!pf->GetDataLine(&text)) // TODO Localisation
  {
    pf->ReportError("Expected button text.");
    return false;
  }

  // Get Font face
  std::string fontFace;
  if (!pf->GetDataLine(&fontFace))
  {
    // IS compulsory: glue files do not allow for optional extra bits.
    pf->ReportError("No font face");
    return false;
  }
  m_pFont = TheFontManager::Instance()->GetFont(fontFace);
  if (!m_pFont)
  {
    pf->ReportError("Bad font face: " + fontFace);
    return false;
  }
  // Get font size
  if (!pf->GetFloat(&m_fontSize))
  {
    pf->ReportError("Expected font size");
    return false;
  }
  SetText(text);

  return true;
}

void GuiTextButton::DrawImpl()
{
  GuiButton::DrawImpl();
  
  if (!IsSelected())
  {
    //c = 0.5f; // TODO CONFIG
  }
  //float a = 1.0f;
  //if (!IsEnabled())
  //{
  //  a = 0.5f; // TODO CONFIG
  //}

  PoolFont* pFont = m_pFont;
  if (!pFont)
  {
    pFont = Engine::Instance()->GetTextWriter()->GetDefaultFont();
  }
  const float origSize = pFont->GetSize(); 
  const float size = pFont->GetSize() * m_fontSize;
  float scale = 1.0f;
  if (IsPlayerSelectable() && (IsMouseOver() || IsSelected()))
  {
    scale *= 1.1f;
  }
  float e = 0;
  if (IsPlayerSelectable() && IsSelected())
  {
    e = 0.1f;
  }
  pFont->SetSize(size * scale);
  // Centre horizontally
  float left = m_absLeft + m_width * 0.5f - m_textWidth * 1.85f * m_fontSize;

  // Centre vertically TODO CONFIG
  // For m_fontSize = 0.5, pos is m_absTop (- a tiny bit)
  // For m_fontSize = 1.0, pos is m_absTop +  m_height * 0.5 - 0.2f 
  // To centre the text vertically, add m_height * 0.5
  // To compensate for text size, sub m_fontSize * 0.5
  static const float TEXT_H_FONTSIZE_MULT = Engine::Instance()->GetConfigFloat(
    "text-button-h-fontsize-mult");
  static const float TEXT_H_OFFSET = Engine::Instance()->GetConfigFloat(
    "text-button-h-offset");
  float top = m_absTop - TEXT_H_FONTSIZE_MULT / m_fontSize + 
    m_height * 0.5f + 
    TEXT_H_OFFSET;

#ifdef GUI_TEXT_BUTTON_DEBUG
std::cout << "Text button: " << m_text.c_str() << " text width: " << m_textWidth
  << " left: " << left << "\n";
#endif

  Engine::Instance()->PushColour(m_fontColour);
  pFont->PrintNoBlend(left + e, top + e, m_text.c_str());
  Engine::Instance()->PopColour();

  pFont->SetSize(origSize);
}

void GuiTextButton::SetText(const std::string& s)
{
  m_text = s;
  // Get the width of the text, and set the button width to fit.
  PoolFont* pFont = m_pFont;
  if (!pFont)
  {
    pFont = Engine::Instance()->GetTextWriter()->GetDefaultFont();
  }
  float w = pFont->GetTextWidth(s);
  m_textWidth = w; 
  // NB DON'T mult text width by font size here; the width is for 
  // a font of size 1.0.
}

void GuiTextButton::SetFont(PoolFont* pFont)
{
  m_pFont = pFont;
}

void GuiTextButton::SetFontSize(float fs)
{
  m_fontSize = fs;
  // Text width is independent of size (assumes size 1.0) so no need
  // to recalculate.
}

void GuiTextButton::SetFontColour(const Colour& col)
{
  m_fontColour = col;
}
}

