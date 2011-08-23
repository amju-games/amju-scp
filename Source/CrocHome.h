/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CrocHome.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(CROC_HOME_H_INCLUDED)
#define CROC_HOME_H_INCLUDED

#include "Target.h"

namespace Amju
{
// This is where Crocs take Chicks when they catch one. 
class CrocHome : public Target
{
public:
  CrocHome();

  virtual const char* GetTypeName() const;

  virtual bool Load(File* pf);

  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

  virtual void HandleObjectCollision(GameObject* pObj);

protected:
  bool m_collided;
};
}

#endif

