/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Function.cpp,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Function.h"
#include "File.h"

namespace Amju
{
Function::Function() : m_time(0)
{
}

Function::~Function()
{
}

bool Function::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }
  return Load(&f);
}

#if defined(SCENE_EDITOR)
bool Function::Save()
{
  File f;
  if (!f.OpenWrite(GetName()))
  {
    f.ReportError("Failed to open function file for writing.");
    return false;
  }
  return Save(&f);
}
#endif

}


