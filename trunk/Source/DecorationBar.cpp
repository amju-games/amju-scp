/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: DecorationBar.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "DecorationBar.h"
#include "File.h"

namespace Amju
{
bool DecorationBar::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open decoration bar file.");
    return false;
  }
  
  // Get the number of quad bitmaps.
  int numQuads = 0;
  if (!f.GetInteger(&numQuads))
  {
    f.ReportError("Expected number of quads.");
    return false;
  }

  // Get each quad.
  for (int i = 0; i < numQuads; i++)
  {
    std::string bitmapFile;
    std::string alphaFile;
    if (!f.GetDataLine(&bitmapFile))
    {
      f.ReportError("Expected bitmap filename.");
      return false;
    }
    // Every quad must have an alpha bitmap.
    if (!f.GetDataLine(&alphaFile))
    {
      f.ReportError("Expected alpha filename.");
      return false;
    }

    TexturedQuad tq;
    tq.Load(bitmapFile, alphaFile);
    m_quads.push_back(tq);
  }

  // Get quad width and height (same for all quads)
  if (!f.GetFloat(&m_w))
  {
    f.ReportError("Expected quad width.");
    return false;
  }
  if (!f.GetFloat(&m_h))
  {
    f.ReportError("Expected quad height.");
    return false;
  }
  // Get increments.
  if (!f.GetFloat(&m_vInc))
  {
    f.ReportError("Expected vertical increment.");
    return false;
  }
  if (!f.GetFloat(&m_hInc))
  {
    f.ReportError("Expected horizontal increment.");
    return false;
  }
  return true;
}

void DecorationBar::Draw(float top, float left, float bottom, float right)
{
  float i = 0; // quad index

  float t = top;
  while (t < bottom)
  {
    float l = left;
    while (l < right)
    {
      m_quads[i].Draw(t, l, t + m_h, l + m_w);
      i++;
      if (i == m_quads.size())
      {
        i = 0;
      }
      l += m_hInc;
    }
    t += m_vInc;
  }
}

}
