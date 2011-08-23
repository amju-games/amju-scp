/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CameraPath.cpp,v $
Revision 1.1.10.1  2005/06/05 00:22:49  Administrator
Compile fix for const Orientation

Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "CameraPath.h"
#include "File.h"
#include "Function.h"
#include "FunctionFactory.h"
#include "Engine.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
CameraPath::CameraPath()
{
}

CameraPath::~CameraPath()
{
}

void CameraPath::Reset()
{
  Assert(m_pFunc.GetPtr());
  if (m_pFunc.GetPtr())
  {
    m_pFunc->SetTime(0);
  }
}

bool CameraPath::Load(File* pf)
{
  // Get the function name.
  string fnName;
  if (!pf->GetDataLine(&fnName))
  {
    pf->ReportError("Expected path function name.");
    return false;
  }

  if (fnName.empty())
  {
    pf->ReportError("Empty path function name.");
    return false;
  }
  m_pFunc = FunctionFactory::Instance()->Create(fnName);

  if (!m_pFunc.GetPtr())
  {
    pf->ReportError("Bad path function name.");
    return false;
  }

  // The function for a Camera Path is likely to be a curve based on control points.
  // It's better to have this data in a separate file.
  // Get the filename for the function data.
  string camFileName;
  if (!pf->GetDataLine(&camFileName))
  {
    pf->ReportError("Expected file name of camera function data.");
    return false;
  }

  File camFile;
  if (!camFile.OpenRead(camFileName))
  {
    pf->ReportError("Couldn't open camera path data file.");
    return false;
  }

  if (!m_pFunc->Load(&camFile))
  {
    return false;
  }

  return true;
}

void CameraPath::Draw()
{
  if (!m_pFunc.GetPtr())
  {
    return;
  }

  Orientation o = *(GetOrientation());
  m_pFunc->Recalc(&o);
  SetOrientation(o);
 
  FirstPersonCamera::Draw();
}

}
