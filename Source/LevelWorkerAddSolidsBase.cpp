/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddSolidsBase.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/


#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddSolidsBase.h"
#include "File.h"
#include "SolidComponent.h"

namespace Amju
{
bool LevelWorkerAddSolidsBase::Load(File* pf)
{
  // Get each Solid name.
  // Load each one. This is done so that all Solids are marked as used.
  // Then none will be discarded because they appear to be unused.

  int numSolids = 0;
  if (!pf->GetInteger(&numSolids))
  {
    pf->ReportError("Expected number of solids.");
    return false;
  }
  for (int i = 0; i < numSolids; i++)
  {
    std::string solidName;
    if (!pf->GetDataLine(&solidName))
    {
      pf->ReportError("Expected solid name.");
      return false;
    }
    PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
    if (!pSolid.GetPtr())
    {
      std::string err = "Bad solid name: ";
      err += solidName;
      pf->ReportError(solidName);
      return false;
    }

    m_solidNames.push_back(solidName);
  }

  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddSolidsBase::Save(File* pf)
{
  int numSolids = m_solidNames.size();
  pf->WriteInteger(numSolids);
  for (int i = 0; i < numSolids; i++)
  {
    pf->Write(m_solidNames[i]);
  }
  return true;
}
#endif

}


