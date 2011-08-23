/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipFactory.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CLIP_FACTORY_H_INCLUDED)
#define CLIP_FACTORY_H_INCLUDED

#include <string>

namespace Amju
{
class Clip;

class ClipFactory
{
public:
  static Clip* CreateClip(const std::string& clipTypeName);
};
}
#endif
