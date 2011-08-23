/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolCourseManager.cpp,v $
Revision 1.1.2.4  2007/07/01 20:36:10  jay
Fix room select to check for Online flag

Revision 1.1.2.3  2007/03/09 21:50:42  jay
Online game - ongoing fixes

Revision 1.1.2.2  2006/08/30 21:12:19  Administrator
Get Pool to build in MSVC

Revision 1.1.2.1  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:34  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.5  2005/09/14 19:43:40  jay
Rooms now have one- or two-player only flags

Revision 1.1.8.4  2005/09/01 20:44:24  jay
Fix const warning

Revision 1.1.8.3  2005/09/01 19:51:36  jay
Some rooms are unlockable during trial period

Revision 1.1.8.2  2005/08/26 21:44:54  jay
Get room costs from file; don't check directory

Revision 1.1.8.1  2005/06/22 21:41:17  jay
Added some debug output

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolCourseManager.h"
#include "ConfigFile.h"
#include "Engine.h"
#include "StringUtils.h"
#include "LevelServer.h"
#include "Directory.h"
#include "StringUtils.h"
#include "ExtraDir.h"
#include "SchAssert.h"

namespace Amju
{
static const int TRIAL_UNLOCKABLE = 1;
static const int ONE_PLAYER = 2;
static const int TWO_PLAYER = 4;
static const int ONLINE_OK = 8;

SinglePoolCourseManager::SinglePoolCourseManager()
{
  m_pUnlockedConfigFile = new ConfigFile;
}

int SinglePoolCourseManager::GetNumCourses() const
{
  return m_courseMap.size();  
}

std::string SinglePoolCourseManager::GetCourseName(int courseId) const
{
  const CourseInfo& ci = m_courseMap[courseId];
  return ci.m_name;
}

std::string SinglePoolCourseManager::GetCourseFileName(int courseId) const
{
  const CourseInfo& ci = m_courseMap[courseId];
  return ci.m_filename;
}

void SinglePoolCourseManager::GetRoomCost(
  int levelId, int roomId, int* pType, int*pNumber)
{
  std::pair<int, int> v = m_roomCosts[std::make_pair(levelId, roomId)];
  *pType = v.first;
  *pNumber = v.second;
}

bool SinglePoolCourseManager::Load()
{
  // POOL
  // Level info file has list of all levels, cost of each room, user-friendly
  // name for level, etc.
  std::string filename = Engine::Instance()->GetConfigValue("levels_file");
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Failed to open pool levels file");
    return false;
  }
  // Get number of levels
  int numLevels;
  if (!f.GetInteger(&numLevels))
  {
    f.ReportError("Expected number of levels");
    return false;
  }
  for (int i = 0; i < numLevels; i++)
  {
    // Get filename, user-friendly name
    std::string name, filename;
    if (!f.GetDataLine(&filename))
    {
      f.ReportError("Expected level filename");
      return false;
    }
    if (!f.GetDataLine(&name))
    {
      f.ReportError("Expected level user friendly name");
      return false;
    } 
    // Get number of rooms
    int numRooms = 0;  
    if (!f.GetInteger(&numRooms))
    {
      f.ReportError("Expected number of rooms");
      return false;
    }
    for (int j = 0; j < numRooms; j++)
    {
      int type = 0;
      int cost = 0;
      if (!f.GetInteger(&type))
      {
        f.ReportError("Expected cost type for room");
        return false;
      }
      if (!f.GetInteger(&cost))
      {
        f.ReportError("Expected cost number for room");
        return false;
      }
      m_roomCosts[std::make_pair(i, j)] = std::make_pair(type, cost);

      // Get BITFIELD: is this room unlockable in trial period, or is it
      // one- or two- player only ? (Bits defined at top of file)
      int bits = 0;
      if (!f.GetInteger(&bits))
      {
        f.ReportError("Expected trial unlock/one/two player flags");
        return false;
      }
      m_trialUnlock[std::make_pair(i, j)] = bits;
    }

    CourseInfo ci;
    ci.m_id = i;
    ci.m_isUnlocked = true; 
    ci.m_name = name;
    ci.m_filename = filename;
    ci.m_isUser = false;
    ci.m_maxHole = 0;
    m_courseMap.push_back(ci);
  }

  return true;
}

bool SinglePoolCourseManager::IsRoomOnePlayerOnly(
  int levelId, int roomId) const
{
  // Check for undefined level/room
  // TODO
  TrialUnlockMap::const_iterator it = 
    m_trialUnlock.find(std::make_pair(levelId, roomId));
  int bits = it->second;
  return (bits & ONE_PLAYER) != 0;
}

bool SinglePoolCourseManager::IsRoomTwoPlayerOnly(
  int levelId, int roomId) const
{
  // Check for undefined level/room
  // TODO
  TrialUnlockMap::const_iterator it = 
    m_trialUnlock.find(std::make_pair(levelId, roomId));
  int bits = it->second;
  return (bits & TWO_PLAYER) != 0;
}

bool SinglePoolCourseManager::IsRoomUnlockableDuringTrialPeriod(
  int levelId, int roomId) const
{
  // Check for undefined level/room
  // TODO
  TrialUnlockMap::const_iterator it = 
    m_trialUnlock.find(std::make_pair(levelId, roomId));
  int bits = it->second;
  return bits & TRIAL_UNLOCKABLE;
}

bool SinglePoolCourseManager::IsRoomOkForOnline(
  int levelId, int roomId) const
{
  // Check for undefined level/room
  // TODO
  TrialUnlockMap::const_iterator it = 
    m_trialUnlock.find(std::make_pair(levelId, roomId));
  int bits = it->second;
  return bits & ONLINE_OK;
}
}

