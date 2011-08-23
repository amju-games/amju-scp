/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Font.h,v $
Revision 1.1.10.1  2005/08/26 21:10:18  jay
New Text system using Fonts

*/

#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <map>
#include <string>
#include "SharedPtr.h"
#include "TextureSequence.h"
#include "Singleton.h"

namespace Amju
{
class File;

class Font : public Shareable
{
public:
  Font(const std::string& name);

  // Load font data from a file. 
  // I.e. point size, texture file names, etc.
  bool Load(File*);
   
  // Get "point size" of font (it's a relative size rather than an
  //  absolute point size) 
  float GetSize() const;

  // Change the size 
  void SetSize(float size);

  // Get width of one character
  float GetCharacterWidth(char c);

  // Get width of a string
  float GetTextWidth(const std::string& s);

  // Draw the text on the window. Called by TextWriter, so you 
  // shouldn't need to call this directly ?
  // x and y are coords - top left is (0, 0). Bottom right
  // is approx (23, 16).
  void PrintNoBlend(float x, float y, const char* text);

protected:
  std::string m_name;
  TextureSequence m_textureSequence;
  float m_size;
  std::map<char, float> m_charWidths; 
  // The first character in the texture sequence - usually 0 for a 256-
  //  character font, or 32 if the font starts at space.
  int m_startChar;
};

class FontManager
{
public:
  // Load all available fonts
  bool Init();

  // Get a font - the name includes the point size ?
  Font* GetFont(const std::string& fontName);

protected:
  typedef std::map<std::string, SharedPtr<Font> > Fonts;
  Fonts m_fonts;
};

typedef Singleton<FontManager> TheFontManager;
}

#endif

