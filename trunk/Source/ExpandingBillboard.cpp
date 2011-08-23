/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ExpandingBillboard.cpp,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#include "ExpandingBillboard.h"
#include "Engine.h"

namespace Amju
{
ExpandingBillboard::ExpandingBillboard() : m_expansionVel(0)
{
}

void ExpandingBillboard::Draw()
{
  m_size += m_expansionVel * Engine::Instance()->GetDeltaTime();
  BillBoard::Draw();
}

bool ExpandingBillboard::Load(File* pf)
{
  if (!BillBoard::Load(pf))
  {
    return false;
  }
  if (!pf->GetFloat(&m_expansionVel))
  {
    pf->ReportError("Expected expansion vel.");
    return false;
  }
  return true;
}

}
