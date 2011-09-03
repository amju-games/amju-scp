/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Character.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(CHARACTER_H_INCLUDED)
#define CHARACTER_H_INCLUDED

#include "VisibleGameObject.h"
#include "Vertex.h"
#include "SolidComponent.h"
#include "Shadow.h"
#include "SharedPtr.h"
#include <queue>
#include <string>

namespace Amju
{
class SolidComponent;
class PoolTexture;
class WallPoly;

// Abstract Base class for animated meshes.
// "Character" is a bad name, as a mesh may look like anything.
class Character : public Shareable
{
public:
  // Characters are created by CharacterManager, which owns the attributes
  // common to all characters of the same type.
  Character();
  Character(const Character&);

  const std::string& GetName() const;
  void SetName(const std::string& name);

  virtual ~Character();

  virtual Character* Clone() const = 0;

  virtual bool Load(File*) = 0;

  virtual void Draw() = 0;

  // Update the character's animation.
  virtual void Recalculate();

  // Get total number of polygons in viewable solid.
  virtual int GetNumberOfPolygons() const = 0;

  virtual void SetActionPain() {}
  virtual void SetActionDead() {}
  virtual void SetActionIdle() {}
  virtual void SetActionWalk() {}
  virtual void SetActionRun() {}
  virtual void SetActionJump() {}
  virtual void SetActionFly() {}
  virtual void SetActionFall() {}
  virtual void SetActionShuffle() {}

  // Returns true if the current action the Character is performing can 
  // continue indefinitely, e.g. running, standing, or is asingle action,
  // e.g. jump.
  virtual bool CurrentActionRepeats() const = 0;

  // Returns true if current action should freeze on the last frame.
  virtual bool CurrentActionFreezes() const = 0;

  // Put an action in a queue, to be performed once the current action has 
  // finished.
  void QueueAction(int actionId);

  int GetCurrentAction() { return m_animation; }

  // Returns name of current action the character is performing.
  // (Used for debugging)
  std::string GetActionName();

  // Set Character to be frozen or not. If frozen, the Character is not animated.
  void SetFrozen(bool frozen);

  void SetFalling(bool falling);

  // Set an action for this character. 
  virtual void SetAction(int actionId);
  // Get the current action
  int GetAction() const;

  // Convert between action int and name for this Character.
  virtual std::string GetActionName(int action) = 0;
  virtual int GetAction(const std::string& actionName) = 0;

protected:
  // Convenience function: play a WAV from this Character's directory.
  void PlayCharacterWav(const std::string& wavFile);

  bool IsFalling() const { return m_isFalling; }

protected:

  // Current Action ID.
  int m_animation;

  // Name of current action, for stats.
  std::string m_currentAction;

  // Queue of actions to perform. 
  std::queue<int> m_actionQueue;

  // If true, animation is frozen. Subclasses need to take account of this in their
  // Recalculate() implementation.
  bool m_isFrozen;

  // If true, character is falling. This may mean that a falling animation gets 
  // top priority,
  bool m_isFalling;

  std::string m_name; // for saving
};
}
#endif

