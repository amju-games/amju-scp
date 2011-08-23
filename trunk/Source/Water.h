/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Water.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_WATER_H_INCLUDED)
#define SCHMICKEN_WATER_H_INCLUDED

#include <set>
#include <vector>
#include "BoundingBox.h"
#include "VisibleGameObject.h"
#include "SolidComponent.h"

namespace Amju
{
// Water in a room. 
class Water : public VisibleGameObject
{
public:
  Water();

  virtual void Draw();
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual bool Load(File*);
  virtual const char* GetTypeName() const;
  virtual void Update();
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual void HandleObjectCollision(GameObject* pObj);
  virtual void RefreshHeightServer();
  virtual bool AddToHeightServer(const HeightServer& hs);
  virtual void DrawShadow() const;
  virtual void OnRoomEntry();
  // Handle messages to remove set elements, change water height, etc.
  virtual void ReceiveMessage(const Message& m);

  // Changes the height of the water: ONLY the y-coord of the orientation will
  // be used.
  virtual void SetOrientation(const Orientation&);
  // Height of the water: friendlier function names still affecting only
  // the y-coord of the water.
  void SetHeight(float y);
  float GetHeight() const;

protected:
  class Ripple
  {
  public:
    Ripple() { Reset(); }
    void Draw(float deltaTime);
    void Reset(float x = 0, float z = 0) 
    { 
      m_size = 0; 
      m_isActive = false; 
      m_x = x; 
      m_z = z; 
    }
    void Activate() { m_isActive = true; }
    bool IsActive() const { return m_isActive; }

    static void SetExpandVel(float ev) { s_expandVel = ev; }
    static void SetMaxSize(float s) { s_maxsize = s; }
    static void SetTexture(Texture* pTex) { s_pTexture = pTex; }
    void SetHeight(float y) { m_y = y; }

  private:
    float m_size;
    bool m_isActive;
    float m_x, m_z; // centre of ripple

    // Height is the same for all ripples. Also we will want to change it if
    // the water level changes!
    //static float s_y;
    float m_y;
    static float s_maxsize;
    static float s_expandVel;
    static Texture* s_pTexture;
  };

  // Container of ripples. To create a new ripple, find an inactive one
  // in this vector.
  typedef std::vector<Ripple> RippleVec;
  RippleVec m_ripples;

  // Set of objects which have recently caused a ripple to be created.
  // This stops us creating a new ripple every frame.
  // Objects are aged-out of this set, so they can create another ripple.
  typedef std::set<int> RippleObjectSet;
  RippleObjectSet m_rippleSet;

  // Returns true if the object ID is in the ripple set - i.e. has just
  // created a ripple.
  bool IsRippling(int objectId);

  // Create a ripple centred at the given coords.
  bool CreateRipple(float x, float y, float z);

protected:
  // The water: a horizontal poly.
  // Use a Leaf for code reuse.
  //PSolidComponent m_pWaterLeaf;
  Texture* m_pWaterTexture;
  
  // Height of this body of water, may not be at top of bounding box.
  float m_height;

  // Bounding Box: this is the volume of water.
  BoundingBox m_box;
  // Sphere enclosing box
  BoundingSphere m_sphere;

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);

  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif

