/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bean.h,v $
Revision 1.1  2004/09/08 15:42:33  jay
Added to repository
  
*/

#if !defined(BEAN_H_INCLUDED)
#define BEAN_H_INCLUDED

#include "Bonus.h"

namespace Amju
{
// Beans come in different values, which can be used to trigger events.
class Bean : public Bonus
{
public:
  Bean();
  virtual const char* GetTypeName() const;
  virtual void HandlePlayerCollision(CharacterGameObject* pPlayer);

protected:
  virtual void DoBeanAction() = 0; 

public:
//  virtual bool Save(File*);

#if defined(SCENE_EDITOR)
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);

#endif
};
}

#endif

