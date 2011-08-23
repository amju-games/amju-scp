/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: InventoryBox.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "InventoryBox.h"
#include "Engine.h"

namespace Amju
{
void InventoryBox::Init()
{
  m_boxQuad.Load(Engine::Instance()->GetConfigValue("box"),
                 Engine::Instance()->GetConfigValue("box_alpha"));
}

void InventoryBox::Draw()
{
  m_boxQuad.Draw(0, 1, 4, 4); // t, l, b, r
  m_boxQuad.Draw(4, 1, 8, 4); // t, l, b, r
  m_boxQuad.Draw(8, 1, 12, 4); // t, l, b, r
  m_boxQuad.Draw(8, 4, 12, 8); // t, l, b, r
}
}

