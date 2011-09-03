/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CHARACTER_MD2_H_INCLUDED)
#define CHARACTER_MD2_H_INCLUDED

#include "Character.h"
#include "Md2Model.h"

namespace Amju
{
class PoolTexture;

// Character loaded from an MD2 file.
// This is an abstract base class; it just provides Mesh drawing and animation.
// Subclasses provide skins.
class CharacterMd2 : public Character
{
public:
  CharacterMd2();
  CharacterMd2(const CharacterMd2&);

  virtual ~CharacterMd2();

  virtual bool Load(File*);
  virtual Character* Clone() const = 0;

  virtual void Draw();

  virtual void Recalculate();
  virtual bool CurrentActionRepeats() const;
  virtual bool CurrentActionFreezes() const;

  virtual int GetNumberOfPolygons() const;

  virtual void SetActionPain();
  virtual void SetActionDead();
  virtual void SetActionIdle();
  virtual void SetActionWalk();
  virtual void SetActionRun();
  virtual void SetActionJump();
  virtual void SetActionFly();
  virtual void SetActionFall();
  virtual void SetActionShuffle();

  // Convert between action int and name for this Character.
  virtual std::string GetActionName(int action);
  virtual int GetAction(const std::string& actionName);


protected:
  // Called by Draw()
  virtual void BindSkin() = 0;

  // Called by SetActionX() functions above
  virtual void SetAction(int actionId);

  // Point to mesh. The mesh is owned by CharacterManager, so only one mesh of any
  // character type is created.
  Md2Model* m_pModel;

  // Use frame number and animation number to get current MD2 frame.
  int m_frame;
  // Time spent so far showing this frame. Each frame should be shown for 0.1s.
  float m_animtime;
};
}
#endif


