/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CooperationGroupManager.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CooperationGroupManager.h"

namespace Amju
{
void SingleCooperationGroupManager::JoinOrCreateGroup(
  PieceMover* pPm, 
  const std::string& groupName)
{
  if (m_groupMap.find(groupName) == m_groupMap.end())
  {
    m_groupMap[groupName] = new CooperationGroup;
  }

  m_groupMap[groupName]->Join(pPm);
}

void SingleCooperationGroupManager::ClearAllGroups()
{
  for (GroupMap::iterator it = m_groupMap.begin(); it != m_groupMap.end(); ++it)
  {
    it->second->Clear();
  }
  // Delete existing groups, so new groups will be created.
  m_groupMap.clear(); 
}
}
