/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectWatcher.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(GAME_OBJECT_WATCHER_H_INCLUDED)
#define GAME_OBJECT_WATCHER_H_INCLUDED

#include "GameObject.h"
#include "Objective.h"
#include "Message.h"
#include "MessageList.h"

namespace Amju
{
// Watches the state of other Game Objects. When all objects being watched
// are in the required state, one or more messages are sent.
class GameObjectWatcher : public GameObject
{
public:
  GameObjectWatcher();

  // Watchers aren't visible
  virtual void Draw() {}
  virtual const BoundingSphere* GetBoundingSphere() const { return 0; }
  virtual void CreateBoundingSphere(const Matrix& cm) {}

  virtual void Update();
  virtual bool Load(File*);
  virtual const char* GetTypeName() const;

  virtual bool HasLocation() const { return false; }

protected:
  Objective m_objective;
  MessageList m_messages;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

  Objective* GetObjective() { return &m_objective; }
  MessageList* GetMessages() { return &m_messages; }

#endif
};
}

#endif

