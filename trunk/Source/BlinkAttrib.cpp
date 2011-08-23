/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BlinkAttrib.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "BlinkAttrib.h"

namespace Amju
{
const char* BlinkAttrib::TypeName = "blink";

void BlinkAttrib::Draw()
{
}

void BlinkAttrib::Restore()
{
}

bool BlinkAttrib::Load(File* pf)
{
  return true;
}

PAttrib BlinkAttrib::Clone()
{
  return new BlinkAttrib(*this);
}

const char* BlinkAttrib::GetTypeName() const
{
  return BlinkAttrib::TypeName;
}

#if defined(SCENE_EDITOR)
bool BlinkAttrib::Save(File* pf)
{
  return true;
}
#endif
}

