/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CooperationGroupManager.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if !defined(COOPERATION_GROUP_MANAGER_H_INCLUDED)
#define COOPERATION_GROUP_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include "Singleton.h"
#include "AiCooperationGroup.h"

namespace Amju
{
class SingleCooperationGroupManager
{
public:
  void JoinOrCreateGroup(PieceMover*, const std::string& groupName);

  void ClearAllGroups();

protected:
  typedef std::map<std::string, PCooperationGroup> GroupMap;
  GroupMap m_groupMap;
};

typedef Singleton<SingleCooperationGroupManager>
  CooperationGroupManager;
}

#endif

