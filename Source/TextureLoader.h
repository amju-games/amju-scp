/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureLoader.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_LOADER_H_INCLUDED)
#define TEXTURE_LOADER_H_INCLUDED

#include <string>

namespace Amju
{
// Abstract base class. Subclasses implement platform-specific ways of loading 
// bitmaps.
// Bitmaps should be 24-bit, with dimensions 2^m * 2^n.
class TextureLoader
{
public:
  TextureLoader() {}
  virtual ~TextureLoader() {}

  virtual bool Load(const std::string& filename,
                    unsigned char** ppBits,
                    int* pWidth,
                    int* pHeight) = 0;
};
}
#endif
