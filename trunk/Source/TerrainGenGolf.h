/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TerrainGenGolf.h,v $
Revision 1.1.2.1  2006/07/24 18:20:41  jay
Moved Pool code from Golf/ to Pool/

Revision 1.1  2004/09/08 15:43:07  jay
Added to repository
  
*/

#if !defined(TERRAIN_GEN_GOLF_H_INCLUDED)
#define TERRAIN_GEN_GOLF_H_INCLUDED

#include <vector>
#include "Terrain/TerrainGenComp.h"
#include "Terrain/TerrainGenLeaf.h"

namespace Amju
{
class Texture;


class TerrainGenGolf : public TerrainGenComp
{
public:
  TerrainGenGolf();

  bool Load(File*);
#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

  void AddTexture(Texture* pTexture);

  void SetHeightmap(PHeightmap);

  virtual PSolidComponent CreateSides(float tx, float tz);
  virtual PSolidComponent CreateBottom(float tx, float tz);

protected:
  virtual PSolidComponent CreateLeaf(int leafIndex, float x, float z, float size);

  // Return the texture to apply to a leaf.
  Texture* GetTextureForLeaf(int index);

  // Get heightmap with which to generate a leaf.
  Heightmap* GetHeightmapForLeaf(int index, float x, float z, float size);

protected:
  TerrainGenLeaf m_leafgen;

  // Textures to apply to leaves in the quadtreee,
  std::vector<Texture*> m_textures;

  // Heightmap for entire terrain. Leaves of the quadtree each use a square
  // region of this heightmap.
  PHeightmap m_pHeightmap;

};
}

#endif

