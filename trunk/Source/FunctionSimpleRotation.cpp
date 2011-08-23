/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionSimpleRotation.cpp,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionSimpleRotation.h"
#include "Common.h"
#include "Orientation.h"
#include "FunctionFactory.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
/*
// Register class with Factory.
Function* CreateFunctionSimpleRotation() { return new FunctionSimpleRotation; }
static const bool registeredFnSimpleRotation = FunctionFactory::Instance()->
  RegisterFactoryFunction(FunctionSimpleRotation::Name, CreateFunctionSimpleRotation);
*/

const char* FunctionSimpleRotation::Name = "shm_rotate";

#if defined(SCENE_EDITOR)
const char* FunctionSimpleRotation::GetTypeName() const
{
  return FunctionSimpleRotation::Name;
}
#endif

void FunctionSimpleRotation::Recalc(Orientation* pOrientation)
{
  m_time += Engine::Instance()->GetDeltaTime();

  // Like in FunctionSimple, but we change the angle of rotation
  // for x, y, z, and leave the position unchanged. 
  Assert(pOrientation);
  if (m_periodSecsX > SMALLEST)
  {
    float x = (float)sin((m_time + m_periodOffsetX) / m_periodSecsX * 2.0f * pi) *
                m_multiplierX + m_offsetX;
    pOrientation->SetXRot(x);
  }

  if (m_periodSecsY > SMALLEST)
  {
    float y = (float)sin((m_time + m_periodOffsetY) / m_periodSecsY * 2.0f * pi) * 
                m_multiplierY + m_offsetY;
    pOrientation->SetYRot(y);
  }

  if (m_periodSecsZ > SMALLEST)
  {
    float z = (float)sin((m_time + m_periodOffsetZ) / m_periodSecsZ * 2.0f * pi) * 
                m_multiplierZ + m_offsetZ;
    pOrientation->SetZRot(z);
  }
}
}
