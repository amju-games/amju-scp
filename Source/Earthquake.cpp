/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Earthquake.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Earthquake.h"
#include "Common.h"
#include "Engine.h"

namespace Amju
{
Earthquake::Earthquake()
{
  Reset();
}

void Earthquake::Reset()
{
  m_time = 0;
  m_activeTime = 0;
  m_active = false;
  m_severity = 1.0f;
}

void Earthquake::Draw()
{
  if (!m_active)
  {
    return;
  }

  m_time += Engine::Instance()->GetDeltaTime();
  if (m_time > m_activeTime)
  {
    Reset();
    return;
  }

  // Get jitter in x- and y- axes, between -1 and 1.
  float rx = float(rand()) / (float)(RAND_MAX) * 2.0f - 1.0f;
  float ry = float(rand()) / (float)(RAND_MAX) * 2.0f - 1.0f;
  // Multiplier: starts at 1, decreaing to 0.
  float m = (m_activeTime - m_time) / m_activeTime;
  // Multiply m by earthquake severity.
  m *= m_severity;
  rx *= m;
  ry *= m;

  AmjuGL::Translate(rx, ry, 0);
}

void Earthquake::SetActive(float activeSeconds, float severity)
{
  m_active = true;
  m_time = 0;
  m_activeTime = activeSeconds;
  m_severity = severity;
}

bool Earthquake::IsActive()
{
  return m_active;
}

}

