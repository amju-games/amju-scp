/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerName.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerName.h"
#include "File.h"
#include "Level.h"

namespace Amju
{
const char* LevelWorkerName::TypeName = "name";
const char* LevelWorkerName::GetTypeName() { return TypeName; }

#ifdef SCENE_EDITOR
bool LevelWorkerName::Save(File* pf)
{
  pf->Write(m_name);
  return true;
}
#endif

bool LevelWorkerName::Load(File* pf)
{
  if (!pf->GetDataLine(&m_name))
  {
    pf->ReportError("Expected level name.");
    return false;
  }
  return true;
}

bool LevelWorkerName::AddFeature(Level* pLevel, CubeMap* )
{
  pLevel->SetLevelName(m_name);
  return true;
}

}

