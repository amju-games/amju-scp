/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiCommandGo.h,v $
Revision 1.1.2.2  2006/08/14 17:50:20  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(GUI_COMMAND_GO_H_INCLUDED)
#define GUI_COMMAND_GO_H_INCLUDED 

#include "GuiCommandHandler.h"

namespace Amju
{
class PoolGuiCommandGo : public PoolGuiCommand
{
public:
  virtual bool Do();
};
}

#endif


