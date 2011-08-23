/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipFactory.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ClipFactory.h"
#include "ClipLevel.h"
#include "ClipLevelCaption.h"

using namespace std;

namespace Amju
{
Clip* ClipFactory::CreateClip(const string& clipTypeName)
{
  if (clipTypeName == "cliplevel")
  {
    return new ClipLevel;
  }
  if (clipTypeName == "cliplevelcaption")
  {
    return new ClipLevelCaption;
  }

  return 0;
}
}
