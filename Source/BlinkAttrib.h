/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BlinkAttrib.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(AMJU_ATTRIB_BLINK_H_INCLUDED)
#define AMJU_ATTRIB_BLINK_H_INCLUDED

#include "Attrib.h"

namespace Amju
{
class BlinkAttrib: public Attrib
{
public:
  static const char* TypeName;

  virtual void Draw(); 
  virtual void Restore(); 
  virtual bool Load(File* pf); 
  virtual PAttrib Clone();
  virtual const char* GetTypeName() const;

#if defined(SCENE_EDITOR)
  virtual bool Save(File* pf);
#endif
};
}

#endif

