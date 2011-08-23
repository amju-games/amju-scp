/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef BUTTON_CREATOR_H_INCLUDED
#define BUTTON_CREATOR_H_INCLUDED

#include "GuiElement.h"
#include "Colour.h"

namespace Amju
{
class Font;

class ButtonCreator : public Shareable
{
public:
  ButtonCreator();
  virtual GuiElement* Create();
  void SetButtonWidthHeight(float w, float h);
  void SetButtonFilename(const std::string& filename);
  void SetText(const std::string& text);
  void SetFont(Font* pFont);
  void SetFontSize(float size);
  void SetFontColour(const Colour& c);

protected:
  float m_w;
  float m_h;
  std::string m_filename; // load button bitmap
  std::string m_text; // text to set on button
  Font* m_pFont;
  float m_fontSize;
  Colour m_fontColour;
};
}

#endif

