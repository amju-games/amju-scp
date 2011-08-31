/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddRoom.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "LevelWorkerAddRoom.h"
#include "Level.h"
#include "Room.h"
#include "File.h"
#include "SolidComponent.h"
#include "SolidComposite.h"
#include "ScaleSolid.h"
#include "StringUtils.h"

namespace Amju
{
const char* LevelWorkerAddRoom::TypeName = "add_room";
const char* LevelWorkerAddRoom::GetTypeName() { return TypeName; }

bool LevelWorkerAddRoom::Load(File* pf)
{
  // Get number of rooms.
  // Then load each one to mark it as in use, and to check for errors.
  int numRooms = 0;
  if (!pf->GetInteger(&numRooms))
  {
    pf->ReportError("Expected number of rooms.");
    return false;
  }
  for (int i = 0; i < numRooms; i++)
  {
    // Temporary room, to test that it loads ok.
    PRoom pRoom = new Room(0); 
    std::string roomFileName;
    if (!pf->GetDataLine(&roomFileName))
    {
      pf->ReportError("Expected room file name.");
      return false;
    }
    // Check that room loads now.
    if (!pRoom->Load(roomFileName))
    {
      std::string err = "Failed to load room: ";
      err == roomFileName;
      pf->ReportError(err);
      return false;
    }

    m_roomNames.push_back(roomFileName);
  }

  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddRoom::Save(File* pf)
{
  int numRooms = m_roomNames.size();
  pf->WriteInteger(numRooms);
  for (int i = 0; i < numRooms; i++)
  {
    pf->Write(StripPath(m_roomNames[i]));
  }
  return true;
}
#endif

bool LevelWorkerAddRoom::AddFeature(Level* pLevel)
{
  int numRooms = m_roomNames.size();

  if (numRooms == 0)
  {
    std::cout << "Adding a room.\n";

    // In this case, just add a new empty room.
    int roomId = pLevel->GetNumberOfRooms();
    Room* pRoom = new Room(roomId);
    pRoom->Load("empty.room");
    pLevel->AddRoom(pRoom);
    pLevel->SetRoomId(roomId);
    std::cout << pLevel << " Level rooms (add feature): " << pLevel->GetNumberOfRooms() << "\n";
    return true;
  }

  for (int r = 0; r < numRooms; r++)
  {
    int roomId = r + pLevel->GetNumberOfRooms();
    Room* pRoom = new Room(roomId); 

    // Add scenery to room. This will depend on the level ID.
    std::string roomFileName = m_roomNames[r]; 
    pRoom->Load(roomFileName);
    pLevel->AddRoom(pRoom); 
    pLevel->SetRoomId(roomId);
  }

  return true;
} 

void LevelWorkerAddRoom::AddRoomName(const std::string& s)
{
  m_roomNames.push_back(s);
}

void LevelWorkerAddRoom::ReplaceName(
  const std::string& oldname, const std::string& newname)
{
  std::vector<std::string>::iterator it = std::find(m_roomNames.begin(), m_roomNames.end(), oldname);
  if (it != m_roomNames.end())
  {
    *it = newname;
  }
}

}

