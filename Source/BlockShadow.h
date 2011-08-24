/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BlockShadow.h,v $
Revision 1.1.10.1  2007/07/15 21:56:54  Administrator
DEBUG mode: Each Plane stores the name of the leaf which created it.
This is for debugging HeightServer issues, where you want to know where
a poly has come from.

Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_BLOCK_SHADOW_H_INCLUDED)
#define SCHMICKEN_BLOCK_SHADOW_H_INCLUDED

#include "BasicShadow.h"
#include "HeightServer.h"

namespace Amju
{
class SolidGameObject;

// Implement shadows for blocks.
class BlockShadow : public Shadow
{
public:
  static bool Init();

  BlockShadow();

  virtual void Draw(
    float x, float y, float z,
    float size,
    const HeightServer& hs);

  virtual void SetCaster(VisibleGameObject* pVgo);

protected:
  virtual void BindTexture();

  virtual void DrawList();

  virtual void RecalculateList(
    float x, 
    float y, 
    float z,
    float size,
    const HeightServer& hs);

protected:
  // Not a SharedPtr, as this points to the object which owns this Shadow
  // - via a SharedPtr!
  SolidGameObject* m_pCaster;

  // Block shadow is made up of many sub-shadows, one for each distinct plane
  // poly on the caster.
  typedef std::vector<PShadow> Shadows;
  Shadows m_shadows;

protected:
  // This type draws the shadow cast by one wall of a block.
  class BlockShadowImpl : public BasicShadow
  {
  public:
    static bool Init();
#ifdef _DEBUG
    BlockShadowImpl() :  
      m_plane("blockShadowImpl"), m_polyIndex(-1)
#else
    BlockShadowImpl() :  m_polyIndex(-1)
#endif
    {
    }

    void SetCaster(VisibleGameObject* pVgo);

    void SetPlane(const Plane& plane);

    // Set the poly index.
    // This is the index of the poly held by the object's heightserver,
    //  whose shadow this object renders.
    void SetIndex(int i, bool isWall);

  protected:
    virtual void RecalculateVerts();

    virtual void MapST(
      float centrex, 
      float centrez, 
      float x, 
      float z, 
      float size, 
      float* s, 
      float* t);

    virtual void BindTexture();

  protected:
    // Every BlockShadowImpl needs to know its caster.
    // TODO Plane/poly ?
    SolidGameObject* m_pCaster;

    // This is the plane poly whose shadow we cast.
    Plane m_plane;
    int m_polyIndex;
    // Is this plane poly a wall or floor ?
    bool m_isWall;

  public: // so BlockShadow can access it
    static Texture* s_pBlockTexture;
  };
};
}

#endif
