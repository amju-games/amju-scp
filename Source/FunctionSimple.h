/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionSimple.h,v $
Revision 1.1.10.1  2005/05/15 17:11:45  jay
Set parameters from code, not just Scene Ed or Load from file.

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(PLATFORM_FUNCTION_SIMPLE_H_INCLUDED)
#define PLATFORM_FUNCTION_SIMPLE_H_INCLUDED

// FunctionSimple.h
// This kind of function is a simple sine function.

#include "Function.h"

namespace Amju
{
class FunctionSimple : public Function
{
public:
  static const char* Name;

  FunctionSimple();

  virtual bool Load(File* pf);
  //virtual void SetTime(float secs);
  virtual void Recalc(Orientation* p);

protected:
  // The parameters for this simple function.
  float m_periodSecsX; // Time taken to complete a cycle.
  float m_periodOffsetX; // Added to angle 
  float m_multiplierX;
  float m_offsetX;

  float m_periodSecsY; 
  float m_periodOffsetY;
  float m_multiplierY;
  float m_offsetY;

  float m_periodSecsZ; // Time taken to complete a cycle.
  float m_periodOffsetZ;
  float m_multiplierZ;
  float m_offsetZ;


#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const;
  virtual bool Save(File*);
#endif

public:
  float GetMaxX() const;
  float GetMinX() const;
  float GetPeriodX() const;

  void SetMaxX(float);
  void SetMinX(float);
  void SetPeriodX(float);

  float GetMaxY() const;
  float GetMinY() const;
  float GetPeriodY() const;

  void SetMaxY(float);
  void SetMinY(float);
  void SetPeriodY(float);

  float GetMaxZ() const;
  float GetMinZ() const;
  float GetPeriodZ() const;

  void SetMaxZ(float);
  void SetMinZ(float);
  void SetPeriodZ(float);
};
}
#endif

