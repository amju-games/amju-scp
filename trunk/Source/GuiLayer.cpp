/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiLayer.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
PoolGuiLayerTq::PoolGuiLayerTq(const TexturedQuad& tq) : m_tq(tq)
{
}

void PoolGuiLayerTq::Draw(float top, float left, float bottom, float right)
{
  m_tq.Draw(top, left, bottom, right);
}

PoolGuiLayerDecorator::PoolGuiLayerDecorator(PoolGuiLayer* pChild) : m_pChild(pChild)
{
}

void PoolGuiLayerDecorator::Draw(float top, float left, float bottom, float right)
{
  m_pChild->Draw(top, left, bottom, right);
}

PoolGuiLayerColourList::PoolGuiLayerColourList(
  PoolGuiLayer* pChild, 
  const ColourList& colours, 
  float colourTimeSecs, 
  bool interpolate) : 
  PoolGuiLayerDecorator(pChild),
  m_colours(colours),
  m_colourTime(colourTimeSecs),
  m_time(0),
  m_interpolate(interpolate),
  m_index(0)
{
  Assert(!m_colours.empty());
}

void PoolGuiLayerColourList::Draw(float top, float left, float bottom, float right)
{
  // Get current colour
  m_time += Engine::Instance()->GetDeltaTime();
  if (m_time > m_colourTime)
  {
    ++m_index;
    if (m_index == (int)m_colours.size())
    {
      m_index = 0;
    }
  }
  // TODO Interpolation
  const Colour& c = m_colours[m_index];
  Engine::Instance()->PushColour(c);
  PoolGuiLayerDecorator::Draw(top, left, bottom, right);
  Engine::Instance()->PopColour();
}
}


