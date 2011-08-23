/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureMethodFactory.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_METHOD_FACTORY_H_INCLUDED)
#define TEXTURE_METHOD_FACTORY_H_INCLUDED

namespace Amju
{
class TextureMethod;
class File;

class TextureMethodFactory
{
public:
  static TextureMethod* Create(File* pf);
};
}

#endif

