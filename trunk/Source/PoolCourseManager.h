/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolCourseManager.h,v $
Revision 1.1.2.2  2007/03/09 21:50:42  jay
Online game - ongoing fixes

Revision 1.1.2.1  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:34  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.3  2005/09/14 19:43:40  jay
Rooms now have one- or two-player only flags

Revision 1.1.8.2  2005/09/01 19:51:37  jay
Some rooms are unlockable during trial period

Revision 1.1.8.1  2005/08/26 21:44:54  jay
Get room costs from file; don't check directory

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if !defined(GOLF_COURSE_MANAGER_H_INCLUDED)
#define GOLF_COURSE_MANAGER_H_INCLUDED

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "Singleton.h"

namespace Amju
{
class ConfigFile;

class SinglePoolCourseManager
{
public:
  SinglePoolCourseManager();

  // Get number of available courses. 
  // This is all predefined courses, plus any user courses.
  // User courses are always unlocked.
  // Predefined courses must be unlocked - although the first course
  // is always unlocked.
  int GetNumCourses() const;

  // Get the user-friendly name of a course. 
  std::string GetCourseName(int) const;

  // Get filename for a course (level).
  std::string GetCourseFileName(int courseId) const;

  // POOL
  // Get info about a table/game (i.e. level/room). Each room has a
  // "cost". If the current player (or one of the current players for 
  // two player) has collected enough bonuses, then the room is unlocked.
  void GetRoomCost(int levelId, int roomId, int* pType, int* pNumber);

  bool IsRoomUnlockableDuringTrialPeriod(int levelId, int roomId) const;

  // If true, the room can only be played in a one-player, non-practice game.
  bool IsRoomOnePlayerOnly(int levelId, int roomId) const;

  // If true, the room can only be played in a two (human) player game.
  bool IsRoomTwoPlayerOnly(int levelId, int roomId) const;

  // If true, the room is OK for online game
  bool IsRoomOkForOnline(int levelId, int roomId) const;

  // Load all courses, and list of unlocked courses.
  bool Load();

private:
  // File of unlocked course IDs.
  // This is updated whenever Unlock() is called.
  ConfigFile* m_pUnlockedConfigFile;

  struct CourseInfo
  {
    int m_id; 
    // Filename of next level, which is unlocked when this level is completed.
    // May be empty if nothing to unlock.
    std::string m_nextFilename;
    std::string m_name;  // user-friendly level name 
    std::string m_filename; // filename of level data file
    bool m_isUnlocked; 
    bool m_isUser; // if true, this is user defined, else predefined.
    int m_maxHole; // highest unlocked hole. All holes up to and including this one
     // are unlocked.
  };
  typedef std::vector<CourseInfo> CourseMap;
  CourseMap m_courseMap;

  // Cost of each room. This is a map of (levelId, RoomId) to (type, number)
  typedef std::map<std::pair<int, int>, std::pair<int, int> > RoomCosts;
  RoomCosts m_roomCosts;

  // BITFIELD for each room: can this room be unlocked in trial period;
  // one player only or two player only ?
  // The bits are only used in the .cpp file - access is via IsRoom...() 
  // functions.
  typedef std::map<std::pair<int, int>, int> TrialUnlockMap;
  TrialUnlockMap m_trialUnlock;
};

typedef Singleton<SinglePoolCourseManager> PoolCourseManager;
}

#endif

