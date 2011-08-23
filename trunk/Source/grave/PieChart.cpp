/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PieChart.cpp,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PieChart.h"
#include "TextureServer.h"
#include "TexturedQuad.h"
#include "Engine.h"

namespace Amju
{
void PieChart2d::Draw()
{
  float top = 0;
  float left = 0;
  float bottom = 1.0f;
  float right = 1.0f;

  // Draw slices. 
  static const int numSlices = (int)(360.0f / m_sliceAngleDegs);
  for (int i = 0; i < numSlices; i++)
  {
    m_slice.Draw(top, left, bottom, right);
  }
}

void PieChart::Set(PieChartKey key, float value)
{
  m_values[key] = value;
  // TODO work out total of all values.
  // Get fraction of total for each key, then set the number of slices 
  // for the key.
  m_total = 0;
  for (ValueMap::iterator it = m_values.begin(); it != m_values.end(); ++it)
  {
    m_total += it->second;
  }
  
}

void PieChart::SetColour(PieChartKey key, Colour c)
{
  m_colours[key] = c;
}

void PieChart::SetSliceAngle(float degrees)
{
  m_sliceAngleDegs = degrees;
}

void PieChart::Clear()
{
  m_values.clear();
  m_colours.clear();
}

void PieChart2d::SetSlice(Texture* pTexture)
{
//  m_slice.Set
}

void PieChart3d::Draw()
{
  if (!m_pSlice.GetPtr())
  {
    return;
  }
  
  if (m_values.empty())
  {
    return;
  }

  float total = 0;

  AmjuGL::PushMatrix();
  ValueMap::iterator valueIt = m_values.begin();
  ColourMap::iterator colourIt = m_colours.begin();

  // Draw slices. 
  const int numSlices = (int)(360.0f / m_sliceAngleDegs);
  float sliceValue = m_total / (float)numSlices;

  Colour c = colourIt->second;
  float v = valueIt->second;

  for (int i = 0; i < numSlices; i++)
  {
    Engine::Instance()->PushColour(c.m_r, c.m_g, c.m_b, c.m_a);
    m_pSlice->Draw();
    Engine::Instance()->PopColour();

    // Rotate m_sliceAngle around y-axis
    AmjuGL::Rotate(m_sliceAngleDegs, 0, 1.0f, 0); 

    total += sliceValue;
    if (total > v)
    {
      total = 0;
      ++colourIt;
      ++valueIt;
      c = colourIt->second;
      v = valueIt->second;
    }    
  }

  AmjuGL::PopMatrix();
}

void PieChart3d::SetSlice(PSolidComponent pSlice)
{
  m_pSlice = pSlice;
}


}

