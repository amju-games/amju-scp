/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiButton.cpp,v $
Revision 1.1.10.3  2005/07/30 12:30:15  jay
Improved enlarge-on-select

Revision 1.1.10.2  2005/07/17 23:06:47  jay
When selected expands proportionately

Revision 1.1.10.1  2005/07/05 09:43:29  jay
Added background box for tool tip help - currently disabled - needs
to automatically size.

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "GuiButton.h"
#include "File.h"
#include "Engine.h"

namespace Amju
{
PoolGuiButton::PoolGuiButton()
{
  m_isSelected = false;
}

void PoolGuiButton::SetFromQuad(const TexturedQuad& tq)
{
  m_quads.push_back(tq);
}

bool PoolGuiButton::Load(File* pf)
{
  // Get number of textures; load each one.
  int numTextures = 0;
  if (!pf->GetInteger(&numTextures))
  {
    pf->ReportError("Expected number of textures for GUI button.");
    return false;
  }
  for (int i = 0; i < numTextures; i++)
  {
    TexturedQuad tq;
    if (!tq.Load(pf))
    {
      pf->ReportError("Failed to load GUI button texture.");
      return false;
    }
    m_quads.push_back(tq);
  }

  if (!LoadToolTipText(pf))
  {
    pf->ReportError("Failed to load tooltip text.");
    return false;
  }

  return true;
}

void PoolGuiButton::DrawImpl()
{
  // Draw larger if mouse is over button (i.e. it's "selected")
  // Increase size proportionately in x and y.
  static const float EXTRA = Engine::Instance()->GetConfigFloat(
    "gui_button_extra");

  float s = 0; 
  float t = 0;
  if (m_isSelected)
  {
    s = EXTRA; 
    t = s * m_width / m_height;
  }
  const int numQuads = m_quads.size();
  for (int i = 0; i < numQuads; i++)
  {
    m_quads[i].Draw(m_absTop - s, m_absLeft - t, m_height + m_absTop + s, m_width + m_absLeft + t);
  }
}
}

