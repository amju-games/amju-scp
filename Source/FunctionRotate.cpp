/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionRotate.cpp,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionRotate.h"
#include "File.h"
#include "Engine.h"
#include "Orientation.h"
#include "FunctionFactory.h"

namespace Amju
{
/*
// Register class with Factory.
Function* CreateFunctionRotate() { return new FunctionRotate; }
static const bool isRegistered = FunctionFactory::Instance()->
  RegisterFactoryFunction(FunctionRotate::Name, CreateFunctionRotate);
*/

const char* FunctionRotate::Name = "rotate";

FunctionRotate::FunctionRotate()
{
  m_xRotVel = 0;
  m_yRotVel = 0;
  m_zRotVel = 0;
}

#if defined(SCENE_EDITOR)
const char* FunctionRotate::GetTypeName() const
{
  return FunctionRotate::Name;
}

bool FunctionRotate::Save(File* pf)
{
  pf->WriteComment("// x rotate vel");
  pf->WriteFloat(m_xRotVel);

  pf->WriteComment("// y rotate vel");
  pf->WriteFloat(m_yRotVel);

  pf->WriteComment("// z rotate vel");
  pf->WriteFloat(m_zRotVel);

  return true;
}
#endif

bool FunctionRotate::Load(File* pf)
{
  if (!pf->GetFloat(&m_xRotVel))
  {
    pf->ReportError("Function rotate expected x vel");
    return false;
  }
  if (!pf->GetFloat(&m_yRotVel))
  {
    pf->ReportError("Function rotate expected y vel");
    return false;
  }
  if (!pf->GetFloat(&m_zRotVel))
  {
    pf->ReportError("Function rotate expected z vel");
    return false;
  }
  return true;
}

void FunctionRotate::Recalc(Orientation* pOrientation)
{
  float deltaT = Engine::Instance()->GetDeltaTime();

  float xRot = pOrientation->GetXRot();
  float newXRot = xRot + m_xRotVel * deltaT;
  if (newXRot > 360.0f)
  {
    newXRot -= 360.0f;
  }
  if (newXRot < -360.0f)
  {
    newXRot += 360.0f;
  }
  pOrientation->SetXRot(newXRot);


  float yRot = pOrientation->GetYRot();
  float newYRot = yRot + m_yRotVel * deltaT;
  if (newYRot > 360.0f)
  {
    newYRot -= 360.0f;
  }
  if (newYRot < -360.0f)
  {
    newYRot += 360.0f;
  }
  pOrientation->SetYRot(newYRot);


  float zRot = pOrientation->GetZRot();
  float newZRot = zRot + m_zRotVel * deltaT;
  if (newZRot > 360.0f)
  {
    newZRot -= 360.0f;
  }
  if (newZRot < -360.0f)
  {
    newZRot += 360.0f;
  }
  pOrientation->SetZRot(newZRot);
}
}

