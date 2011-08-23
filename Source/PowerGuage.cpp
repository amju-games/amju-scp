/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerGuage.cpp,v $
Revision 1.3.8.1  2005/07/17 23:00:24  jay
Allow vertical power guages

Revision 1.3  2004/09/17 13:50:44  jay
Added extra features for player stats

Revision 1.2  2004/09/15 09:13:17  Administrator
More functionality for Stats pages

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PowerGuage.h"
#include "File.h"
#include "Engine.h"

namespace Amju
{
PowerGuage::PowerGuage() : m_time(0), m_maxTime(0), m_anchor(0)
{
  m_isVertical = false;
}

void PowerGuage::SetAnchor(float f)
{
  m_anchor = f;
}

void PowerGuage::SetPowerTime(float time)
{
  m_time = time;
  m_maxTime = time;
}

float PowerGuage::GetPowerTime() const
{
  return m_maxTime;
}

void PowerGuage::IncreaseTime(float secs)
{
  m_time += secs;
  if (m_time > m_maxTime)
  {
    m_time = m_maxTime;
  }
}

void PowerGuage::Update()
{
/*
  m_time -= Engine::Instance()->GetDeltaTime();
  m_val = m_time / m_maxTime;
*/
}

bool PowerGuage::IsVertical() const
{
  return m_isVertical;
}

void PowerGuage::SetVertical(bool v)
{
  m_isVertical = v;
}

void PowerGuage::Draw()
{
  float scrollOffset = 0;

  m_backQuad.Draw(m_top + scrollOffset, m_left, m_bottom + scrollOffset, m_right); 

  // Get right-hand position of foreground quad.
  // We expect m_val to be between 0 (no power) and 1.0 (full power)
  if (m_val < 0)
  {
    m_val = 0;
  }
  if (m_val > 1.0f)
  {
    m_val = 1.0f;
  }

  if (IsVertical())
  {
    float anchor = m_ftop + m_anchor * (m_fbottom - m_ftop);
    // Position of "moving end" of the bar
    float pos = m_val * (float)(m_fbottom - m_ftop) + (float)m_ftop;
    float a = anchor;
    float b = pos;
    if (pos < anchor)
    {
      // Swap if value position is to the left of the anchor position.
      a = pos; 
      b = anchor;
    }
    Engine::Instance()->PushColour(m_colour);
    m_foreQuad.Draw(a, m_fleft, b, m_fright); // TODO scroll offset
    Engine::Instance()->PopColour();
  }
  else
  {
    float anchor = m_fleft + m_anchor * (m_fright - m_fleft);
    // Position of "moving end" of the bar
    float pos = m_val * (float)(m_fright - m_fleft) + (float)m_fleft;
    float a = anchor;
    float b = pos;
    if (pos < anchor)
    {
      // Swap if value position is to the left of the anchor position.
      a = pos; 
      b = anchor;
    }
    Engine::Instance()->PushColour(m_colour);
    m_foreQuad.Draw(m_ftop + scrollOffset, a, m_fbottom + scrollOffset, b);
    Engine::Instance()->PopColour();
  }
}

bool PowerGuage::Load(File* pf)
{
  if (!Guage::Load(pf))
  {
    return false; // Error is reported in base class.
  }
  // Get the foreground position.
  if (!pf->GetFloat(&m_ftop) ||
      !pf->GetFloat(&m_fleft) ||
      !pf->GetFloat(&m_fbottom) ||
      !pf->GetFloat(&m_fright) )
  {
    pf->ReportError("Expected foreground position for power guage.");
    return false;
  }
  return true;
}

void PowerGuage::SetColour(const Colour& c)
{
  m_colour = c;
}
}

