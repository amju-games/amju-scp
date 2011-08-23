/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStateShowTable.h,v $
Revision 1.1.2.2  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/10/21 17:44:32  jay
Not finished; base class for showing tables/grids

*/

#ifndef ENGINE_STATE_SHOW_TABLE_H_INCLUDED
#define ENGINE_STATE_SHOW_TABLE_H_INCLUDED

#include "EngineStateText.h"
#include "GuiTable.h"

namespace Amju
{
class EngineStateShowTable : public EngineStateText
{
public:
  virtual void DrawOverlays();
  virtual void MouseButton(bool, bool, bool);

protected:
  // TODO on heap ?
  GuiTable m_table;
};
}

#endif

