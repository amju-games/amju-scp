/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolGuiFactory.cpp,v $
Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "GuiFactory.h"
#include "File.h"

namespace Amju
{
PoolGuiElement* SinglePoolGuiFactory::Create(File* pf)
{
  std::string s;
  if (!pf->GetDataLine(&s))
  {
    pf->ReportError("Expected GUI element name.");
    return 0;
  }
  return Factory<PoolGuiElement>::Create(s);
}
}

