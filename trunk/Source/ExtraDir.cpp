/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ExtraDir.cpp,v $
Revision 1.1.10.1  2006/08/30 21:09:35  Administrator
Get Pool to build in MSVC

Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ExtraDir.h"
#include "Engine.h"

namespace Amju
{
std::string GetExtraDir()
{
    std::string filename = Engine::Instance()->GetConfigValue("extra_dir");

#ifdef MACOSX
    // TODO TEMP TEST MACOSX
    filename = "/Users/jay/pet/" + filename;
#endif

  return filename;
}

}


