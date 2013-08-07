/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextWriter.h,v $
Revision 1.1.10.1  2005/08/26 21:10:19  jay
New Text system using Fonts

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(DRAW_TEXT_H_INCLUDED)
#define DRAW_TEXT_H_INCLUDED

#include <deque>
#include <string>
#include "Font.h"

namespace Amju
{
// TextWriter superimposes text over the image. 
// Short strings can be placed within a 20 * 30 'grid'.
// Text can be added to a list of scrolling lines. In this case you only have to 
// add the string once, rather than Draw it every frame.
class TextWriter
{
public:
  TextWriter();
  virtual ~TextWriter();

  // Set "drop shadow" effect for text
  void SetDropShadow(bool);

  // Print text. You must call this every frame.
  // If pFont is 0, the default Font is used.
  void Print(float x, float y, const char *text, PoolFont* pFont = 0);

  // As above, but centre text along x-axis.
  // If pFont is 0, the default Font is used.
  void PrintCentre(float y, const char* text, PoolFont* pFont = 0);

  // No Blending is set, so you can override the normal blending calls.
  void PrintNoBlend(float x, float y, const char *text, PoolFont* pFont = 0);

  // Add a string to the scrolling text. 
  void ScrollPrint(const std::string& text);

  // Call to display the scrolling text. This should be called every frame.
  void DrawScrollingItems();

  // Clears the scrolling text.
  void ClearScrollingItems();

  // Scrolling text: max no of lines displayed in window.
  void SetMaxLines(int m) { m_maxLines = m; }
  int GetMaxLines() { return m_maxLines; }

  // Convenience functions: Get character/string widths for the default font
  float GetCharacterWidth(char c);
  float GetTextWidth(const std::string& s);

  static const float CHAR_SIZE;
  static const float X_OFFSET;
  static const float Y_OFFSET;
  static const float Z_OFFSET;
  static const float HORIZONTAL_SPACING;

  // The default font - probably don't need to access it ?
  PoolFont* GetDefaultFont();

private:
  friend class PoolFontManager;
  // Set by Font Manager after fonts have been loaded.
  void SetDefaultFont(PoolFont*);

protected:
  // Container of strings which scroll up the screen. 
  // We use a queue of strings: new strings get pushed onto the end, and the 
  // oldest string gets popped off.
  // Using a deque allows both upwards and downwards scrolling.
  typedef std::deque<std::string> ScrollItems;
  ScrollItems m_scrollItems;
  int m_maxLines;

  // The default font; this is used whenever a custom font is not specified.
  RCPtr<PoolFont> m_pFont;

  bool m_dropShadow;
};
}
#endif

