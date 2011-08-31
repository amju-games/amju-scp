/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddRoom.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_ROOM_H_INCLUDED)
#define LEVEL_WORKER_ADD_ROOM_H_INCLUDED

#include <string>
#include <vector>
#include "LevelWorker.h"

namespace Amju
{
class LevelWorkerAddRoom : public LevelWorker
{
public:
  static const char* TypeName;

  virtual bool Load(File* ); 
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*); 

  void AddRoomName(const std::string&);
  void ReplaceName(const std::string& oldname, const std::string& newname);

protected:
  std::vector<std::string> m_roomNames;
};
}

#endif

