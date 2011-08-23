/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionFactory.cpp,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FunctionFactory.h"
// include headers of all function types. 
// Bah - we should use Factory<Function> But it won't work!
#include "FunctionSimple.h"
#include "FunctionSimpleRotation.h"
#include "FunctionCtrlPoints.h"
#include "FunctionRotate.h"
#include "FunctionBlocker.h"
#include "FunctionTimedBlocker.h"

using namespace std;

namespace Amju
{
FunctionFactory* FunctionFactory::Instance()
{
  static FunctionFactory ff;
  return &ff;
}

FunctionFactory::FunctionFactory()
{
#if defined(SCENE_EDITOR)
  m_typenames.push_back(FunctionSimple::Name);
  m_typenames.push_back(FunctionSimpleRotation::Name);
  m_typenames.push_back(FunctionCtrlPoints::Name);
  m_typenames.push_back(FunctionRotate::Name);
  m_typenames.push_back(FunctionBlocker::Name);
  m_typenames.push_back(FunctionTimedBlocker::Name);
#endif
}

#if defined(SCENE_EDITOR)
const std::vector<std::string>& FunctionFactory::GetTypeNames() const
{
  return m_typenames;
}
#endif

Function* FunctionFactory::Create(const string& fnName)
{
  if (fnName == FunctionSimple::Name)
  {
    return new FunctionSimple;
  }
  else if (fnName == FunctionSimpleRotation::Name) 
  {
    return new FunctionSimpleRotation;
  }
  else if (fnName == FunctionCtrlPoints::Name)
  {
    return new FunctionCtrlPoints;
  }
  else if (fnName == FunctionRotate::Name)
  {
    return new FunctionRotate;
  }
  else if (fnName == FunctionBlocker::Name)
  {
    return new FunctionBlocker;
  }
  else if (fnName == FunctionTimedBlocker::Name)
  {
    return new FunctionTimedBlocker;
  }
  return 0; // unrecognised name.
}

}
