/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolCharacter.h,v $
Revision 1.1.2.1  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:34  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.5  2005/07/30 12:20:09  jay
Distace from cue tip to body is now per-character so each character
can accurately intersect table

Revision 1.1.8.4  2005/07/05 09:38:10  jay
Character lean-over

Revision 1.1.8.3  2005/06/19 21:32:23  jay
Try to detect player intersection with table; draw animation accordingly.

Revision 1.1.8.2  2005/06/13 22:15:59  jay
Integrate Pool animations

Revision 1.1.8.1  2005/05/15 17:17:14  jay
Added trasparency setting

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_CHARACTER_H_INCLUDED)
#define GOLF_CHARACTER_H_INCLUDED

#include "Npc.h"

namespace Amju
{
class PoolCharacter : public CharacterGameObject
{
public:
  PoolCharacter();

  virtual void Draw();
  virtual bool Load(File*);

  // We must control the shadow as the player origin is centred on the ball,
  // not the player! So drawing a shadow at the local origin would draw it
  // under the ball, not the character.
  virtual void DrawShadow() const;

  void DrawSwish();

  void SetActionGo();
  void SetActionReady();
  void SetActionPoolShuffle();

  // Set character translucency
  void SetAlpha(float a);

protected:
  virtual void OnNoMove();
  virtual void OnArcMove();

protected:
  // Effect of golf club 'swishing' as shot is taken.
  PSolidComponent m_pSwish;

  bool m_swish; // true if we draw swish effect
  float m_swishtime; // time for which we have drawn swish effect

  // Set current player character to translucent so we can see the cue etc ?
  float m_alpha;

  // Spheres we use to detect if character instersects table
  BoundingSphere m_tableBs1;
  BoundingSphere m_tableBs2;

  float m_distCueBody;
};
}

#endif

