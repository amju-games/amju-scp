/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BeanControl.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:07  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:24  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.1  2005/03/17 20:51:15  jay
Make GetBall() less specific

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#include "BeanControl.h"
#include "PoolMisc.h"

namespace Amju
{
const char* BeanControl::GetTypeName() const
{
  return "bean_control";
}

void BeanControl::DoBeanAction()
{
}


}
