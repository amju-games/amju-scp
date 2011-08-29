/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Font.cpp,v $
Revision 1.1.2.2  2007/07/15 21:54:59  Administrator
MSVC warnings

Revision 1.1.2.1  2005/08/26 21:10:18  jay
New Text system using Fonts

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "Font.h"
#include "Engine.h"
#include "File.h"
#include "TextWriter.h"
#include "SchAssert.h"

namespace Amju
{
Font::Font(const std::string& name) : m_name(name)
{
}

bool Font::Load(File* pf)
{
  // Get the Point Size. This is not really a point size because it depends 
  // on the dimensions of the window!
  if (!pf->GetFloat(&m_size))
  {
    pf->ReportError("Expected font point size.");
    return false;
  }
  // Get the bitmap and alpha filenames
  std::string bm, alpha;
  if (!pf->GetDataLine(&bm))
  {
    pf->ReportError("Expected font bitmap name.");
    return false;
  }
  if (!pf->GetDataLine(&alpha))
  {
    pf->ReportError("Expected font alpha name.");
    return false;
  }
  // Get no of characters in X and Y directions.
  int xchars = 0;
  int ychars = 0;
  if (!pf->GetInteger(&xchars))
  {
    pf->ReportError("Expected font x chars");
    return false;
  }
  if (!pf->GetInteger(&ychars))
  {
    pf->ReportError("Expected font y chars");
    return false;
  }

  bool b = m_textureSequence.Load(
    bm, alpha, xchars, ychars, TextWriter::CHAR_SIZE, TextWriter::CHAR_SIZE);
  if (!b)
  {
    pf->ReportError("Failed to load texture sequence for font");
    return false;
  }

  // Get start character - usually something like 0 or 32.
  if (!pf->GetInteger(&m_startChar))
  {
    pf->ReportError("Expected font start character");
    return false;
  }

  // Get filename for character widths
  std::string filename;
  if (!pf->GetDataLine(&filename)) 
  {
    pf->ReportError("Expected font widths filename");
    return false;
  }

  // load char widths
  File f(false); // false => don't expect version info
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open font widths file");
    return false;
  }
  // Widths are in *.INI format:
  // [<section>]
  // <char>=<width>
  // <char>=<width>
  // ... etc.
  std::string line;
  f.GetDataLine(&line); // get [<section>]
  // Now get lines of format <char code>=<width>
  // Width is font bitmap width/num chars
  const float MAX_WIDTH = (float)m_textureSequence.GetTextureWidth() / 
    (float)xchars;

  // We expect 256 character widths.
  // NB We must not loop until the file ends - this will not work with
  // GLUE files.
  for (int i = 0; i < 256; i++)
  {
    f.GetDataLine(&line);

    std::string::size_type eq = line.find('=');
    std::string strCode = line.substr(0, eq);
    std::string strWidth = line.substr(eq + 1);
    int code = atoi(strCode.c_str());
    float width = (float)atoi(strWidth.c_str());
    width /= MAX_WIDTH;
    width *= TextWriter::HORIZONTAL_SPACING;

    m_charWidths[(char)code] = width;
  }

//#ifdef FONT_DEBUG
std::cout << "FONT: Successfully loaded " << m_name.c_str() << "\n";
//#endif

  return true;
}

float Font::GetSize() const
{
  return m_size;
}

void Font::SetSize(float s)
{
  m_size = s;
}

float Font::GetCharacterWidth(char c)
{
  float f = m_charWidths[c];
  return f;
}

float Font::GetTextWidth(const std::string& s)
{
  float f = 0;
  int chars = s.size();
  for (int i = 0; i < chars; i++)
  {
    f += GetCharacterWidth(s[i]);
  }
  return f;
}

void Font::PrintNoBlend(float x, float y, const char* text)
{
    if (!text)
    {
        return;
    }

    AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
    AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_READ);

    AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
    AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

    AmjuGL::PushMatrix();
    AmjuGL::Translate(
      TextWriter::CHAR_SIZE * x - TextWriter::X_OFFSET, 
      TextWriter::Y_OFFSET - TextWriter::CHAR_SIZE * y, 
      TextWriter::Z_OFFSET);

    m_textureSequence.Bind();
    //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
    //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);

    int i = 0;
    while (unsigned char c = text[i])
    {
        i++;

        AmjuGL::PushMatrix();
        AmjuGL::Scale(m_size, m_size, 1.0f);
        m_textureSequence.Draw(c - (char)m_startChar);
        AmjuGL::PopMatrix();

        float f = GetCharacterWidth(c) * m_size;
        AmjuGL::Translate(f, 0.0f, 0.0f);
    }

    AmjuGL::PopMatrix();

    AmjuGL::PopAttrib();
    AmjuGL::PopAttrib();
}

bool FontManager::Init()
{
  // Get font info filename. This file contains info for all fonts.
  std::string fontInfo = Engine::Instance()->GetConfigValue("fonts");
  File f;
  if (!f.OpenRead(fontInfo))
  {
    return false;
  }
  int numFonts = 0;
  if (!f.GetInteger(&numFonts))
  {
    f.ReportError("Expected number of fonts.");
    return false;
  }
  for (int i = 0; i < numFonts; i++)
  {
    std::string fontName;
    if (!f.GetDataLine(&fontName))
    {
      f.ReportError("Expected font name.");
      return false;
    }

    SharedPtr<Font> pFont = new Font(fontName);
    if (!pFont->Load(&f))
    {
      f.ReportError("Failed to load font info.");
      return false;
    }
    
    m_fonts[fontName] = pFont;
  }
  if (m_fonts.empty())
  {
std::cout << "FONT MANAGER: no fonts found!\n";
    return false;
  }

  // Loaded all fonts. The first font is the default.
  SharedPtr<Font> pDefault = m_fonts.begin()->second;
  Engine::Instance()->GetTextWriter()->SetDefaultFont(pDefault.GetPtr());

  return true;
}

Font* FontManager::GetFont(const std::string& fontName)
{
  return m_fonts[fontName].GetPtr();
}
}

