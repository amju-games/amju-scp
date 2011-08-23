/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStateShowTable.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:44:32  jay
Not finished; base class for showing tables/grids

*/

#include "EngineStateShowTable.h"

namespace Amju
{
void EngineStateShowTable::DrawOverlays()
{
  m_table.Draw();
}

void EngineStateShowTable::MouseButton(bool down, bool ctrl, bool shift)
{
  m_table.MouseButton(down, ctrl, shift);
}
}

