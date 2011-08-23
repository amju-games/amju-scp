/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GuiTurnKnob.cpp,v $
Revision 1.1.10.1  2007/03/09 21:48:56  jay
Copy latest GUI library from trunk

Revision 1.3  2006/09/17 23:00:14  Administrator
Big APZ weekend changes

Revision 1.2  2006/07/06 07:55:38  jay
Use new GuiButton Layers

Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <math.h>
#include <iostream>
#include "GuiTurnKnob.h"
#include "AngleCompare.h"
#include "TexturedQuad.h"
#include "File.h"

namespace Amju
{
GuiTurnKnob::GuiTurnKnob()
{
  m_angleDegs = 0;
}

bool GuiTurnKnob::Load(File* pf)
{
  if (!GuiButton::Load(pf))
  {
    return false;
  }

  // Get final texture which is rotated around the centre of the Knob
  TexturedQuad tq;
  if (!tq.Load(pf))
  {
    pf->ReportError("Failed to load rotating texture for Knob.");
    return false;
  }
  AddLayer(new GuiLayerTq(tq));

  return true;
}

void GuiTurnKnob::DrawImpl()
{
  const int numQuads = GetNumLayers();
  if (numQuads < 1)
  {
    return;
  }
  for (int i = 0; i < numQuads - 1; i++)
  {
    m_layers[i]->Draw(
      m_absTop, m_absLeft, m_height + m_absTop, m_width + m_absLeft);
  }

  // Draw final quad
  float height2 = m_height * 0.5f;
  float width2 = m_width * 0.5f;
  float midY = m_absTop + height2; 
  float midX = m_absLeft + width2;

  float width3 = m_width * 0.25f;
  float height3 = m_height * 0.25f;
  float x = midX + sin(DegToRad(m_angleDegs)) * width3 - width3 * 0.5f;
  float y = midY + cos(DegToRad(m_angleDegs)) * height3 - height3 * 0.5f;

  m_layers[numQuads - 1]->Draw(y, x, y + height3, x + width3);

}

void GuiTurnKnob::MousePos(int x, int y)
{
  static int oldx = x;
  static int oldy = y;
  if (!m_downSelected)
  {
    oldx = x;
    oldy = y;
    return;
  }
  int xdiff = x - oldx;
  int ydiff = y - oldy;
  oldx = x;
  oldy = y;

  // TODO Convert mouse coords (pixels) to our Gui coord system.
  float dx = (float)xdiff * 1.0f; // TODO TEMP TEST
  float dy = (float)ydiff * 1.0f; // TODO TEMP TEST

  m_angleDegs += dx;
  m_angleDegs += dy;
}

void GuiTurnKnob::SetAngleDegs(float degs)
{
  m_angleDegs = degs;
}

float GuiTurnKnob::GetAngleDegs()
{
  return m_angleDegs;
}

}


