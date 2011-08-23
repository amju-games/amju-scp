/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PolyLoader.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(POLYLOADER_H_INCLUDED)
#define POLYLOADER_H_INCLUDED

namespace Amju
{
class File;
class Polygon;

class PolyLoader
{
public:
  bool Load(File* pf, Polygon* poly, bool hasTexCoords);

protected:
  bool LoadVersion6(File* pf, Polygon* poly);
  bool LoadTexture(File* pf, Polygon* poly);
  bool LoadTexVert(File* pf, Polygon* poly);

#if defined(SCENE_EDITOR)
public:
  bool Save(File* pf, Polygon* poly);
#endif

};
}
#endif

