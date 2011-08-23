/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMouseBase.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyleMouseBase.h"

namespace Amju
{
ControlStyleMouseBase::ControlStyleMouseBase()
{
  m_newMousePos = true;
  m_drag = false;
}
}

