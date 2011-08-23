/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PulsatingBillboard.cpp,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#include "PulsatingBillboard.h"
#include "Engine.h"

namespace Amju
{
PulsatingBillboard::PulsatingBillboard()
{
  m_periodtimer = 0;
  m_period = 1.0f;
  m_originalSize = 0;
  m_isPulsing = true;
}

void PulsatingBillboard::Draw()
{
  if (m_isPulsing)
  {
    m_periodtimer += Engine::Instance()->GetDeltaTime();
    if (m_periodtimer > m_period)
    {
      m_periodtimer = 0;
      m_size = m_originalSize;
    }
  }
  ExpandingBillboard::Draw();
}

bool PulsatingBillboard::Load(File* pf)
{
  if (!ExpandingBillboard::Load(pf))
  {
    return false;
  }
  if (!pf->GetFloat(&m_period))
  {
    pf->ReportError("Expected period.");
    return false;
  }
  m_originalSize = m_size;
  return true;
}

}

