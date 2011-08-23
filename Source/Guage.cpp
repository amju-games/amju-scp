/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Guage.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Guage.h"
#include "File.h"

namespace Amju
{
Guage::Guage() : m_val(0)
{
  m_top = m_left = m_bottom = m_right = 0;
}

Guage::~Guage()
{
}

bool Guage::Load(File* pf) 
{
  // Load BACKGROUND quad
  if (!m_backQuad.Load(pf))
  {
    pf->ReportError("Failed to load guage background.");
    return false;
  }

  // Load FOREGROUND quad
  if (!m_foreQuad.Load(pf))
  {
    pf->ReportError("Failed to load guage foreground.");
    return false;
  }

  // Get position
  if (!pf->GetFloat(&m_top) ||
      !pf->GetFloat(&m_left) ||
      !pf->GetFloat(&m_bottom) ||
      !pf->GetFloat(&m_right) )
  {
    pf->ReportError("Expected guage position (t, l, b, r).");
    return false;
  }

  return true;
}
}
