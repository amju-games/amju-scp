/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionRotate.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(FUNCTION_ROTATE_H_INCLUDED)
#define FUNCTION_ROTATE_H_INCLUDED

#include "Function.h"

namespace Amju
{
// Very simply, rotate at constant speed.
class FunctionRotate : public Function
{
public:
  static const char* Name;

  FunctionRotate();

  virtual bool Load(File* pf);

  virtual void Recalc(Orientation* p);

protected:
  float m_xRotVel, m_yRotVel, m_zRotVel;


#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const;

  virtual bool Save(File*);

  void SetXRot(float f) { m_xRotVel = f; }
  void SetYRot(float f) { m_yRotVel = f; }
  void SetZRot(float f) { m_zRotVel = f; }

  float GetXRot() const { return m_xRotVel; }
  float GetYRot() const { return m_yRotVel; }
  float GetZRot() const { return m_zRotVel; }

#endif

};
}

#endif

