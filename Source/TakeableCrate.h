/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TakeableCrate.h,v $
Revision 1.1  2004/09/08 15:42:42  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_TAKEABLE_CRATE_H_INCLUDED)
#define SCHMICKEN_TAKEABLE_CRATE_H_INCLUDED

#include "Takeable.h"
#include "SolidGameObject.h"

namespace Amju
{
// Well, what game doesn't have crates!?
// Crates are used to gain height. When used, the crate is dropped onto the
// ground, and the player goes on top.
class TakeableCrate : public Takeable, public SolidGameObject
{
public:
  TakeableCrate();

  virtual const char* GetTypeName() const;
  virtual void Use();
  virtual void RecalcCollisionVol();
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);
  virtual bool Load(File* pf);
  virtual void Update();
  virtual void Draw();
  void DrawTaken();

  // Special case - we want to cast shadows on other crates, so we need
  // their heights.
  virtual void RefreshHeightServer();

  // Return the type of crate - there may be different sizes/shapes.
  // This returns the solid name - maybe return a code in future. TODO
  std::string GetType() const;

  virtual void CreateShadow();
  virtual void DrawShadow() const;

protected:
  std::string m_crateType;
  
#if defined(SCENE_EDITOR)
public:
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif
  
};
}

#endif


