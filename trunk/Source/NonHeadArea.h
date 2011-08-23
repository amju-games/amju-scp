/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: NonHeadArea.h,v $
Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:36  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.2  2005/04/18 22:39:59  Administrator
Scene Ed fixes

Revision 1.1.2.1  2005/04/17 22:00:06  jay
Added not-behind-headstring object for placing cue ball

*/

#ifndef AMJU_NON_HEAD_AREA_H_INCLUDED
#define AMJU_NON_HEAD_AREA_H_INCLUDED

#include "SolidGameObject.h"

namespace Amju
{
// POOL
// This object is for enforcing the area in which the player can place
// the ball after a foul.
// Set the Solid to cover all the table except for the behind-the-headstring
// area.
class NonHeadArea : public SolidGameObject
{
public:
  NonHeadArea();
  virtual const char* GetTypeName() const;
  virtual bool Load(File*);
  virtual void Draw();  

public:
#if defined(SCENE_EDITOR)
  virtual bool Save(File*);
  virtual PropertyMap GetProperties() const;
  virtual void SetProperties(const PropertyMap&);
#endif

};
}

#endif


