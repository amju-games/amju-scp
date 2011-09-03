/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBlock.h,v $
Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/09/14 10:07:56  jay
Added separate collision solid. This is mainly to stop blocks falling
through each other, but will also give better performance.

Revision 1.1.2.2  2005/09/11 19:03:20  jay
Added SCENE_EDITOR functions

Revision 1.1.2.1  2005/09/08 20:02:37  jay
Added destroyable block for CRAZY POOL

*/

#ifndef POOL_BLOCK_H_INCLUDED
#define POOL_BLOCK_H_INCLUDED

#include "FreeMovingSolidGameObject.h"
#include "BlastWaveEffect.h"

namespace Amju
{
// CRAZY POOL:
// These blocks explode when hit by the cue ball. Anything on top of them
// should then drop down - e.g. a ball, or more blocks.
// We derive from FMSGO so we get gravity - other than than these blocks
// won't move (I shouldn't think anyway).
class PoolBlock : public FreeMovingSolidGameObject
{
public:
  PoolBlock();
  virtual const char* GetTypeName() const;

  virtual void Draw();
  virtual void Update();
  virtual bool Load(File* pFile);
  virtual void OnRoomEntry();
  virtual void CullDraw(Frustum*);
  virtual void HandleObjectCollision(PoolGameObject* pObj);
  virtual void RecalcCollisionVol();
  // Returns 0 if we are 'DEAD', i.e. exploding
  virtual const HeightServer* GetCollisionVol() const;

  virtual void SetOrientation(const Orientation&);

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

protected:
  // Position at start of game
  Orientation m_startOrientation;

  // Effect when block is hit
  BlastWaveEffect m_blastWave;

  // Simple solid for Collision Volume.
  // This should be a simplified version of the 'real' solid.
  // This is partly for speed, but mainly to stop stacked blocks falling
  // through each other in the case of funny-shaped blocks.
  PSolidComponent m_pCollisionSolid;
};
}

#endif

