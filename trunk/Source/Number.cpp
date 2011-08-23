/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Number.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "Number.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
Number::Number() : 
  m_number(-1), 
  m_justify(CENTRE)
{
}

int Number::GetNumber() const
{
  return m_number;
}

void Number::SetJustify(Justification j)
{
  m_justify = j;
}
}
