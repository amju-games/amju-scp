/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolHole.h,v $
Revision 1.1.2.1  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:34  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.5  2005/08/31 22:43:36  Administrator
Added number of hearts above each Pocket

Revision 1.1.8.4  2005/08/31 17:30:18  jay
Allow calls to create/add bonuses

Revision 1.1.8.3  2005/08/26 21:45:16  jay
Added bonuses

Revision 1.1.8.2  2005/07/05 09:38:35  jay
Added bonuses - disabled for now

Revision 1.1.8.1  2005/04/17 22:02:20  jay
Added API for nominating a hole

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_HOLE_H_INCLUDED)
#define GOLF_HOLE_H_INCLUDED

#include "SolidGameObject.h"
#include "Target.h"
#include "DecalShadow.h"
#include "ExplosionEffect.h"

namespace Amju
{
class Texture;

class PoolHole : public SolidGameObject
{
public:
  PoolHole();

  virtual const char* GetTypeName() const;

  virtual bool Load(File*);
  virtual void Draw();
  virtual void Update();
  virtual void CullDraw(Frustum*);
  virtual void CreateShadow();
  virtual void DrawShadow() const;
  virtual const BoundingSphere* GetBoundingSphere() const;
  virtual void CreateBoundingSphere(const Matrix& cm);
  virtual void SetOrientation(const Orientation&);
  virtual void OnRoomEntry();

  // Get the position of the hole.
  VertexBase GetTargetVertex() const;

  void SetIsNominated(bool b) { m_isNominated = b; }
  bool GetIsNominated() const { return m_isNominated; }

  virtual void HandleObjectCollision(GameObject* pObj);

  // POOL
  // Bonuses - awarded if you get a valid ball in this pocket.
  // Create bonuses - called on Room Entry.
  void CreateBonuses();

  // Create bonuses without clearing any existing ones.
  void CreateExtraBonuses();

  void DrawBonuses();

public:
  // Called if player gets a valid pot in this pocket.
  void PlayerGetsBonuses();

protected:
  Target m_target;

  // POOL: Only turn on arrow if pocket is nominated
  bool m_isNominated;

  // POOL: List of bonuses for this pocket - if you pot a ball in this
  // pocket (legally..?) you get these bonuses.
  // TODO IS this the right place - need to know if pot was legal,
  // and to set up the list, need to know if this is human or AI.
  // ?????
  struct PoolBonus
  {
    PSolidComponent m_pComp;
    // TODO type etc..?
  };
  std::vector<PoolBonus> m_bonuses;

  float m_bonusTime;
  // Index of bonus currently falling down
  float m_fallVel; 
  // Current y-pos of falling bonus
  float m_fall;
  // If set to true, bonuses start falling down.
  bool m_getBonuses;

  ExplosionEffect m_explosion;

  // Translucency for the column of bonuses above this pocket
  float m_alpha;

  // Number of bonuses which initially appear over this pocket.
  int m_numBonuses;

#if defined(SCENE_EDITOR)
public:
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

};
}

#endif

