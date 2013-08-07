/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiTextButton.h,v $
Revision 1.1.10.2  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.4  2007/02/27 10:23:30  jay
Font colour for text buttons

Revision 1.3  2007/01/08 21:09:56  jay
Centre text; load font from file

Revision 1.2  2006/01/16 13:25:51  jay
Merge improvements from pool; now can set Font

Revision 1.1.10.1  2005/10/21 17:47:48  jay
Add font-related members - currently still problems with placing the text.

Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if !defined(AMJU_GUI_TEXT_BUTTON_H_INCLUDED)
#define AMJU_GUI_TEXT_BUTTON_H_INCLUDED

#include "GuiButton.h"
#include "Font.h"
#include "Colour.h"

namespace Amju
{
class PoolGuiTextButton : public PoolGuiButton 
{
public:
  PoolGuiTextButton();

  virtual bool Load(File* );
  virtual void DrawImpl();

  void SetText(const std::string&);

  // Set font - if not set, the Default Font is used.
  // NB Set Font before text so the width is set correctly.
  // TODO Refresh the width if the font is changed.
  void SetFont(PoolFont* pFont);

  void SetFontSize(float);

  void SetFontColour(const Colour& col);

protected:
  std::string m_text;
  PoolFont* m_pFont;
  float m_textWidth;
  float m_fontSize;
  Colour m_fontColour;
};
}

#endif

