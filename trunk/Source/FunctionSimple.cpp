/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionSimple.cpp,v $
Revision 1.1.10.1  2005/05/15 17:11:44  jay
Set parameters from code, not just Scene Ed or Load from file.

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionSimple.h"
#include "SolidComponent.h"
#include "File.h"
#include "Common.h"
#include "FunctionFactory.h"
#include "Engine.h"
#include <math.h>
#include "SchAssert.h"

namespace Amju
{
/*
// Register class with Factory.
Function* CreateFunctionSimple() { return new FunctionSimple; }
static const bool isRegistered = FunctionFactory::Instance()->
  RegisterFactoryFunction(FunctionSimple::Name, CreateFunctionSimple);
*/

const char* FunctionSimple::Name = "shm_move";

FunctionSimple::FunctionSimple()
{
  m_time = 0;

  m_periodSecsX = 0;
  m_periodOffsetX = 0;
  m_multiplierX = 0;
  m_offsetX = 0;

  m_periodSecsY = 0;
  m_periodOffsetY = 0;
  m_multiplierY = 0;
  m_offsetY = 0;

  m_periodSecsZ = 0;
  m_periodOffsetZ = 0;
  m_multiplierZ = 0;
  m_offsetZ = 0;
}

float FunctionSimple::GetMaxX() const
{
  return m_offsetX + m_multiplierX;
}

float FunctionSimple::GetMinX() const
{
  return m_offsetX - m_multiplierX;
}

float FunctionSimple::GetPeriodX() const
{
  return m_periodSecsX;
}

void FunctionSimple::SetMaxX(float f)
{
  // Offset is avg of min and max.
  m_offsetX = (f + GetMinX()) / 2.0f;
  // Multiplier is max - offset, or offset - min
  m_multiplierX = fabs(f - m_offsetX);
}

void FunctionSimple::SetMinX(float f)
{
  // Offset is avg of min and max.
  m_offsetX = (f + GetMaxX()) / 2.0f;
  // Multiplier is max - offset, or offset - min
  m_multiplierX = fabs(f - m_offsetX);
}

void FunctionSimple::SetPeriodX(float f)
{
  m_periodSecsX = f;
}

float FunctionSimple::GetMaxY() const
{
  return m_offsetY + m_multiplierY;
}

float FunctionSimple::GetMinY() const
{
  return m_offsetY - m_multiplierY;
}

float FunctionSimple::GetPeriodY() const
{
  return m_periodSecsY;
}

void FunctionSimple::SetMaxY(float f)
{
  // Offset is avg of min and max.
  m_offsetY = (f + GetMinY()) / 2.0f;
  // Multiplier is max - offset, or offset - min
  m_multiplierY = fabs(f - m_offsetY);
}

void FunctionSimple::SetMinY(float f)
{
  // Offset is avg of min and max.
  m_offsetY = (f + GetMaxY()) / 2.0f;
  // Multiplier is max - offset, or offset - min
  m_multiplierY = fabs(f - m_offsetY);
}

void FunctionSimple::SetPeriodY(float f)
{
  m_periodSecsY = f;
}

float FunctionSimple::GetMaxZ() const
{
  return m_offsetZ + m_multiplierZ;
}

float FunctionSimple::GetMinZ() const
{
  return m_offsetZ - m_multiplierZ;
}

float FunctionSimple::GetPeriodZ() const
{
  return m_periodSecsZ;
}

void FunctionSimple::SetMaxZ(float f)
{
  // Offset is avg of min and max.
  m_offsetZ = (f + GetMinZ()) / 2.0f;
  // Multiplier is max - offset, or offset - min
  m_multiplierZ = fabs(f - m_offsetZ);
}

void FunctionSimple::SetMinZ(float f)
{
  // Offset is avg of min and max.
  m_offsetZ = (f + GetMaxZ()) / 2.0f;
  // Multiplier is max - offset, or offset - min
  m_multiplierZ = fabs(f - m_offsetZ);
}

void FunctionSimple::SetPeriodZ(float f)
{
  m_periodSecsZ = f;
}

#if defined(SCENE_EDITOR)
const char* FunctionSimple::GetTypeName() const
{
  return FunctionSimple::Name;
}

bool FunctionSimple::Save(File* pf)
{
  pf->WriteComment("// Function period x");
  pf->WriteFloat(m_periodSecsX);
  pf->WriteComment("// Period offset x");
  pf->WriteFloat(m_periodOffsetX);
  pf->WriteComment("// Multiplier x");
  pf->WriteFloat(m_multiplierX);
  pf->WriteComment("// Offset x");
  pf->WriteFloat(m_offsetX);

  pf->WriteComment("// Function period y");
  pf->WriteFloat(m_periodSecsY);
  pf->WriteComment("// Period offset y");
  pf->WriteFloat(m_periodOffsetY);
  pf->WriteComment("// Multiplier y");
  pf->WriteFloat(m_multiplierY);
  pf->WriteComment("// Offset y");
  pf->WriteFloat(m_offsetY);

  pf->WriteComment("// Function period z");
  pf->WriteFloat(m_periodSecsZ);
  pf->WriteComment("// Period offset z");
  pf->WriteFloat(m_periodOffsetZ);
  pf->WriteComment("// Multiplier z");
  pf->WriteFloat(m_multiplierZ);
  pf->WriteComment("// Offset z");
  pf->WriteFloat(m_offsetZ);

  return true;
}
#endif

bool FunctionSimple::Load(File* pf)
{
  if (!pf->GetFloat(&m_periodSecsX))
  {
    pf->ReportError("Expected function period X.");
    return false;
  }
  if (!pf->GetFloat(&m_periodOffsetX))
  {
    pf->ReportError("Expected function period offset X.");
    return false;
  }
  if (!pf->GetFloat(&m_multiplierX))
  {
    pf->ReportError("Expected function multiplier X.");
    return false;
  }
  if (!pf->GetFloat(&m_offsetX))
  {
    pf->ReportError("Expected function offset X.");
    return false;
  }

  if (!pf->GetFloat(&m_periodSecsY))
  {
    pf->ReportError("Expected function period Y.");
    return false;
  }
  if (!pf->GetFloat(&m_periodOffsetY))
  {
    pf->ReportError("Expected function period offset Y.");
    return false;
  }
  if (!pf->GetFloat(&m_multiplierY))
  {
    pf->ReportError("Expected function multiplier Y.");
    return false;
  }
  if (!pf->GetFloat(&m_offsetY))
  {
    pf->ReportError("Expected function offset Y.");
    return false;
  }

  if (!pf->GetFloat(&m_periodSecsZ))
  {
    pf->ReportError("Expected function period Z.");
    return false;
  }
  if (!pf->GetFloat(&m_periodOffsetZ))
  {
    pf->ReportError("Expected function period offset Z.");
    return false;
  }
  if (!pf->GetFloat(&m_multiplierZ))
  {
    pf->ReportError("Expected function multiplier Z.");
    return false;
  }
  if (!pf->GetFloat(&m_offsetZ))
  {
    pf->ReportError("Expected function offset Z.");
    return false;
  }

  return true;
}

void FunctionSimple::Recalc(Orientation* pOrientation)
{
  m_time += Engine::Instance()->GetDeltaTime();

  Assert(pOrientation);
  if (m_periodSecsX > SMALLEST)
  {
    float x = sin((m_time + m_periodOffsetX) / m_periodSecsX * 2.0f * (float)pi) *
                m_multiplierX + m_offsetX;
    pOrientation->SetX(x);
  }

  if (m_periodSecsY > SMALLEST)
  {
    float y = sin((m_time + m_periodOffsetY) / m_periodSecsY * 2.0f * (float)pi) * 
                m_multiplierY + m_offsetY;
    pOrientation->SetY(y);
  }

  if (m_periodSecsZ > SMALLEST)
  {
    float z = sin((m_time + m_periodOffsetZ) / m_periodSecsZ * 2.0f * (float)pi) * 
                m_multiplierZ + m_offsetZ;
    pOrientation->SetZ(z);
  }
}
}
