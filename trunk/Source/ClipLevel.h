/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipLevel.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CLIP_LEVEL_H_INCLUDED)
#define CLIP_LEVEL_H_INCLUDED

#include "Clip.h"

namespace Amju
{
class Level;

// A movie-clip-like sequence, showing a level.
class ClipLevel : public Clip
{
public:
  ClipLevel();
  virtual ~ClipLevel();

  void SetLevel(Level* pLevel) { m_pLevel = pLevel; }
  virtual Level* GetLevel() { return m_pLevel; }

  virtual void Draw();

  virtual bool Load(File* pf);
  virtual void Start();
  // Return to Running state.
  virtual void Blue(bool);

protected:
  void Clear();

  Level* m_pLevel;
  // The clip takes place in ONE room in the Level.
  int m_roomId;
};
}
#endif

