/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObject.h,v $
Revision 1.1.10.2  2007/06/17 20:37:06  jay
POOL ONLINE: Add flag to GameObject to send state to server this frame.
Moved from PoolBall because other types may need to send their state, e.g. Rules

Revision 1.1.10.1  2005/05/08 08:03:41  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(GAME_OBJECT_H_INCLUDED)
#define GAME_OBJECT_H_INCLUDED

#include <map>
#include <string>
#include "MessageRecipient.h"
#include "BoundingSphere.h"
#include "Variable.h"

namespace Amju
{
class SolidComponent;
class CharacterGameObject;
class Matrix;
class Level;
class Orientation;

// GameObject is the base class for stateful game things, like missiles,
// characters, the player, etc. It receives messages, and also has a
// location.
class GameObject : public MessageRecipient
{
public:
  GameObject();
  virtual ~GameObject();

  // Pure virtuals
  // -------------
  virtual void Draw() = 0;

  virtual const BoundingSphere* GetBoundingSphere() const = 0;

  virtual void CreateBoundingSphere(const Matrix& cm) = 0;

  virtual const char* GetTypeName() const = 0;

  // Virtuals
  // --------
  virtual void Update() {}

  virtual bool IsVisible() const { return false; }

  // Load object from a currently open file. Default implementation does nothing,
  // so you can create GameObjects in code.
  virtual bool Load(File*) { return true; }

  // Called when this Game Object collides with the player. The player
  // Character is passed in so the action can be changed, etc.
  // pPlayer _can_ be zero. This indicates that the player does not
  // collide with the object (allows us to reset any collision flags, etc).
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer) {}

  // Called when pObj collides with this object.
  virtual void HandleObjectCollision(GameObject* pObj) {}

  virtual void SetOrientation(const Orientation&) {}

  // Get orientation if it exists.. this may return zero.
  virtual const Orientation* GetOrientation();

  // Return translation component of Orientation. 
  // Undefined if no orientation is defined for the object!
  virtual VertexBase GetPosition() const;

  // Game Objects have a location.
  virtual bool HasLocation() const { return true; }

  // This function is called when the Player enters the room where this
  // Game Object lives. This gives us the opportunity to reset coords, etc.
  virtual void OnRoomEntry() {}

  // Game Objects which cause damage to the player are bad. 
  virtual bool IsBad() { return false; }

  // For editing, return a solid which can be used to select this object.
  virtual void DrawEditSolid();

  // Returns true if this GameObject intersects with the other GameObject.
  // POOL: Virtual, so we can do special capsule intersection test for
  // pool balls.
  virtual bool Intersects(const GameObject& g);


  // Non virtuals
  // ------------

  bool Load(const std::string& filename);

  // Game Objects know which room they are in.
  void SetRoomId(int room) { m_room = room; }
  int GetRoomId() const { return m_room; }

  // Game objects know which level they belong to.
  void SetLevel(Level* pLevel) { m_pLevel = pLevel; }
  Level* GetLevel() { return m_pLevel; }

  // If visible, is this obejct transluacent, or with transparent areas ?
  // (If so, this type of game object is drawn after non-translucent ones.)
  bool IsTranslucent() const;

  void DrawSphere();

  // POOL_ONLINE
  // State change flag: if set, upload new state to server.
  bool SendStateThisFrame() const;
  void SetSendState(bool send); 


protected:
  // ID of room the Game Object is in.
  int m_room;

  // Point to Level where this Game object is.
  Level* m_pLevel;

  bool m_isTranslucent;

  // POOL_ONLINE 
  // If true, send state to server
  bool m_sendStateThisFrame;

public:
#if defined(SCENE_EDITOR)
  // Save object. Used by editing tool.
  virtual bool Save(File*) = 0;

  // Get/Set properties of this object. Each subclass has to convert the
  // <key, value> pairs to/from member data. Which is a bit crap but at
  // least the editor doesn't have to know anything about all the 
  // different types.
  typedef Variable Property;
  typedef std::map<std::string, Property> PropertyMap;

  virtual PropertyMap GetProperties() const = 0;
  virtual void SetProperties(const PropertyMap&) = 0;

#endif
};

typedef SharedPtr<GameObject> PGameObject;
typedef int GameObjectId;
typedef std::map<GameObjectId, PGameObject> GameObjectMap;
}
#endif

