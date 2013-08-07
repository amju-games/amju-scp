/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterGameObject.h,v $
Revision 1.1.10.1  2005/05/08 08:16:48  jay
Fix to GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CHARACTER_GAME_OBJECT_H_INCLUDED)
#define CHARACTER_GAME_OBJECT_H_INCLUDED

#include "VisibleGameObject.h"
#include "FreeMovingGameObject.h"
#include "Orientation.h"
#include "SolidComponent.h"

namespace Amju
{
class Character;

class CharacterGameObject : 
  public FreeMovingGameObject
{
public:
  CharacterGameObject();
  ~CharacterGameObject();

  // Characters do things based on messages they receive.
  // This is more flexible, as players can be controlled by anything that
  // sends messages, e.g. a  network server.
  virtual void ReceiveMessage(const Message& m);

  virtual void Draw();
  virtual void Update();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual const char* GetTypeName() const;

  virtual bool Load(File*); 

  virtual void SetState(State newState);

  // Characters can get hurt by falling!
  virtual void FellFromHeight(float heightFallenFrom);

  virtual void DrawShadow() const;

  virtual void Recalculate();


  // Set up Bounding Sphere.
  void SetSphereParams(float radius, float xoff, float yoff, float zoff);

  // Add animated meshes. The same frame and position is used to draw
  // each mesh. This allows Characters to add on power-ups, weapons, clothes etc.

  // NB This object takes ownership of the Mesh, which must be 
  // allocated on the HEAP.
  void AddMesh(Character* pCharacter);
  void ClearMeshes();

  // Actually jump (don't just do the animation for jumping)
  void Jump();

  // Actions: The call is passed to the Character member, with extra work
  // done by this class. 
  void SetActionPain();
  void SetActionDead();
  void SetActionIdle();
  void SetActionWalk();
  void SetActionRun();
  void SetActionJump();
  void SetActionFly();
  void SetActionFall();
  void SetActionShuffle();

  // Set colour, and how long to have the new colour.
  void SetColourTimer(float time, float r, float g, float b);

  virtual const HeightServer* GetCollisionVol() const;

  // "Nearly Falling"
  // These functions are to control what happens to a character when she
  // goes off the edge of whatever she is standing on. There is a short
  // period when the character can move back onto solid ground, or else jump.
  void SetNearlyFalling(bool b);
  float UpdateNearlyFallingTime(float deltaT);
  bool IsNearlyFalling() const;

  // Unfortunately these functions are defined in both base classes.
  // So define them here to avoid having to dis-ambiguate all the time.
  virtual void SetOrientation(const Orientation& o); 

  virtual const Orientation* GetOrientation();

protected:
  // Called when Character is not moving forward or in an arc.
  // Default behaviour is Idle.
  virtual void OnNoMove();

  // Called when Character is moving around in an arc
  // Default behaviour is Shuffle.
  virtual void OnArcMove();

protected:
  
  // Load bounding sphere(s) radius and offset from character coord.
  bool LoadBoundingSpheres(File*);
  // Load collision volume, which other characters may not enter.
  bool LoadCollisionVolume(File* pf);

  virtual void RecalcCollisionVol();

  void CalcLeanAngle();

protected:

  void ExecuteJump(const PlayerMessage& m);

  // Exectute a member function for each mesh.
  // Necessary as for_each won't work with a container of smart pointers
  // (even using mem_fun).
  void ForEachMesh(void (Character::*pf)());

protected:

  // Centre/radius set when GetBoundingSphere() is called. 
  mutable BoundingSphere m_bsphere;
  // Bounding sphere radius - we get this from config file.
  float m_radius;

  // Offset bounding sphere centre from Character coords
  float m_sphereXOffset, m_sphereYOffset, m_sphereZOffset;

  // Point to a Character subclass. This object owns the Character pointed to.
  // One Character Game Object may consist of more than one animated mesh.
  typedef std::vector<RCPtr<Character> > Meshes;
  Meshes m_meshes;

  // Time how long we change colour 
  float m_colourTimer;
  // Normally white, we can change colour for effects.
  float m_colourR, m_colourG, m_colourB;

  // For smoothly leaning into turns
  float m_leanAngle;
  float m_maxLeanAngle;
  float m_leanVel;

  bool m_isNearlyFalling;
  float m_nearlyFallingTime;

  // Previous position. If it hasn't changed since last time, we don't have to
  // refresh height server.
  float m_oldx, m_oldy, m_oldz;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

protected:
  bool SaveBoundingSpheres(File* pf);
  bool SaveCollisionVolume(File* pf);

#endif
};
}

#endif


