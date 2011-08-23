/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: InventoryBox.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(INVENTORY_BOX_H_INCLUDED)
#define INVENTORY_BOX_H_INCLUDED

#include "TexturedQuad.h"

namespace Amju
{
class InventoryBox
{
public:
  void Init();
  void Draw();

protected:
  TexturedQuad m_boxQuad;
};
}

#endif

