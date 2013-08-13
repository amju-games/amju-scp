/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextWriter.cpp,v $
Revision 1.1.10.2  2005/09/11 19:01:09  jay
Alter centre

Revision 1.1.10.1  2005/08/26 21:10:19  jay
New Text system using Fonts

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

// The font-drawing code originally came from a "NeHe" tutorial I think.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TextWriter.h"
#include "TextureServer.h"
#include "Engine.h" // for Push/PopColour
#include "File.h"
#include "Common.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
// Number of characters in font.
const float TextWriter::HORIZONTAL_SPACING = 0.26f;
const float TextWriter::CHAR_SIZE = 0.25f;
// Offsets to get grid centred on screen.
const float TextWriter::X_OFFSET = 3.1f;
const float TextWriter::Y_OFFSET = 1.8f;
const float TextWriter::Z_OFFSET = -5.0f;

const float DROP_SHADOW_OFFSET = 0.01f;

TextWriter::TextWriter()
{
  m_maxLines = 16;
  SetDropShadow(true);
}

void TextWriter::SetDropShadow(bool b)
{
  m_dropShadow = b;
}

PoolFont* TextWriter::GetDefaultFont()
{
  return m_pFont.GetPtr();
}

void TextWriter::SetDefaultFont(PoolFont* pFont)
{
  m_pFont = pFont;
}

float TextWriter::GetTextWidth(const std::string& s)
{
  return GetDefaultFont()->GetTextWidth(s);
}

float TextWriter::GetCharacterWidth(char c)
{
  return GetDefaultFont()->GetCharacterWidth(c);
}

TextWriter::~TextWriter() 
{ 
}

void TextWriter::PrintNoBlend(float x, float y, const char *text, PoolFont* pFont)
{
    if (pFont == 0)
    {
        pFont = GetDefaultFont();
    }
    pFont->PrintNoBlend(x, y, text);
}

void TextWriter::Print(float x, float y, const char *text, PoolFont* pFont)
{
    AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);

    ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    AmjuGL::Enable(AmjuGL::AMJU_BLEND);

#ifdef USE_DROP_SHADOW
    if (m_dropShadow)
    {
      // To get a Drop Shadow effect, set the current colour to black. 
      // Then restore the colour, and redraw with an offset.
      Engine::Instance()->PushColour(0, 0, 0, 1.0f);
      // Draw black text
      PrintNoBlend(x, y, text, pFont);
      Engine::Instance()->PopColour();
      // Redraw text in current colour with an offset.
      AmjuGL::PushMatrix();
      AmjuGL::Translate(DROP_SHADOW_OFFSET, DROP_SHADOW_OFFSET, 0);
      PrintNoBlend(x, y, text, pFont);
      AmjuGL::PopMatrix();
    }
    else
    {
      PrintNoBlend(x, y, text, pFont);
    }
#else
    Engine::Instance()->PushColour(0, 0, 0, 1.0f);
#endif

    AmjuGL::PopAttrib(); // Blend
}

void TextWriter::PrintCentre(float y, const char* text, PoolFont* pFont)
{
    // Get width of string.
    if (pFont == 0)
    {
        pFont = GetDefaultFont();
    }
    float w = GetTextWidth(text) * pFont->GetSize();
    Print(12.0f - w * 2.0f, y, text, pFont);
}

void TextWriter::ScrollPrint(const string& text)
{
    // Add this latest string to the end of our container. 
    // Discard the oldest string(s).
    // (To change direction, swap "back" and "front".)
    m_scrollItems.push_back(text);

    while (m_scrollItems.size() > (unsigned int)m_maxLines)
    {
        m_scrollItems.pop_front();
    }
}

void TextWriter::DrawScrollingItems()
{
    // We maintain a container of items. The number of items is fixed, so we
    // just print them all.
    int y = 0;
    for (ScrollItems::iterator it =  m_scrollItems.begin();
         it != m_scrollItems.end();
         ++it)
    {
        Print(0, (float)y++, it->c_str());
    }
}

void TextWriter::ClearScrollingItems()
{
  m_scrollItems.clear();
}
}

