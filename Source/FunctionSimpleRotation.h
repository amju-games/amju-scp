/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionSimpleRotation.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(FUNCTION_SIMPLE_ROTATION_H_INCLUDED)
#define FUNCTION_SIMPLE_ROTATION_H_INCLUDED

#include "FunctionSimple.h"

namespace Amju
{
// FunctionSimpleRotation
// Like FunctionSimple, but acts on X, Y, Z angles instead of
// translation.
// I.e. the rotation velocity changes according to an S.H.M. function.
class FunctionSimpleRotation : public FunctionSimple
{
public:
  static const char* Name;

  virtual void Recalc(Orientation* p);


#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const;

#endif

};
}
#endif

