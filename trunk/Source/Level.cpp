/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Level.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "Level.h"
#include "File.h"
#include "Vertex.h"
#include "SolidComponent.h"
#include "CompositeFactory.h"
#include "Engine.h"
#include "Matrix.h"
#include "PoolGameState.h"
#include "Frustum.h"
#include "StringUtils.h"

using namespace std;

namespace Amju
{
Level::Level()
{
  m_pRoom = 0; // points to current room
  m_levelId = 0;
  m_nextLevel = 0;
  m_startRoomId = 0;
  m_playAreaSize = 100; // a reasonable default

#if defined(SCENE_EDITOR)
  // default constructed Level must be saveable for Editor.
  m_playerName = "player";
  m_midiFilename = "music.midi";
#endif
}

Level::~Level()
{
  m_pRoom = 0; // ref count should now be 1 (map holds ptr to each room)
  for (iterator it = Begin(); it != End(); ++it)
  {
    PRoom& pRoom = *it;
    pRoom = 0; // ref count should now be zero.
  }
}

void Level::SetId(int id)
{
  m_levelId = id;
}

int Level::GetRoomId() const
{
  if (m_pRoom.GetPtr()) 
  {
    return m_pRoom->GetId();
  }
#ifdef _DEBUG
  Engine::Instance()->ReportError("No room set for level.");
#endif
  return -1;
}

int Level::GetNumberOfRooms() const
{
  return m_rooms.size();
}

Room* Level::GetRoom(int i)
{
  return m_rooms[i].GetPtr();
}

int Level::GetNumberOfPolygons() const
{
  if (m_pRoom.GetPtr())
  {
    return m_pRoom->GetNumberOfPolygons();
  }
  return 0;
}

int Level::Draw()
{
  if (m_pRoom.GetPtr())
  {
    return m_pRoom->Draw();
  }
  return 0;
}

void Level::DrawLight()
{
  if (m_pRoom.GetPtr())
  {
    m_pRoom->DrawLight();
  }
}

PSolidComponent Level::GetScene()
{
  if (m_pRoom.GetPtr())
  {
    return m_pRoom->GetScene();
  }
  return 0;
}

void Level::SetScene(PSolidComponent pScene)
{
  m_pRoom->SetScene(pScene);
}


#if defined(SCENE_EDITOR)
bool Level::Save(const std::string& filename)
{
  File f;

  if (filename.empty())
  {
    f.ReportError("No filename for Level specified.");
    return false;     
  }

  if (!f.OpenWrite(filename))
  {
    f.ReportError("Coudn't open level file for writing.");
    return false;
  }

  // File version 10+
  f.WriteComment("// Level ID");
  f.WriteInteger(m_levelId);

  f.WriteComment("// Number of rooms");
  f.WriteInteger(m_rooms.size());

  iterator it;
  f.WriteComment("// Rooms..");
  for (it = Begin(); it != End(); ++it)
  {
    f.WriteComment("// Room ID");
    f.WriteInteger((*it)->GetId());
    f.WriteComment("// Room filename");
    f.Write((*it)->GetName());
  }

  f.WriteComment("// Start room ID");
  f.WriteInteger(m_startRoomId);

  f.WriteComment("// Start orientation");
  if (!m_startOrientation.Save(&f))
  {
    return false;
  }

  f.WriteComment("// Character name");
  f.Write(m_playerName);

  f.WriteComment("// MIDI file");
  f.Write(m_midiFilename);

  if (!SaveGameObjects(&f))
  {
    return false;
  }
  return true;
}
#endif

bool Level::Load(const std::string& filename)
{
  File levelFile;
  if (!levelFile.OpenRead(filename))
  {
    levelFile.ReportError("Couldn't open level file.");
    return false;
  }

  // Get level ID. This needs to be consistent with the master list of levels
  // but it's handy for the Editor to load the level ID with the level.
  if (levelFile.GetVer() > 9)
  {
    if (!levelFile.GetInteger(&m_levelId))
    {
      levelFile.ReportError("Expected level ID.");
      return false;
    }
  }

  // Get number of rooms
  int rooms = 0;
  if (!levelFile.GetInteger(&rooms))
  {
    levelFile.ReportError("Expected number of rooms.");
    return false;
  }

  for (int r = 0; r < rooms; r++)
  {
    // Create each room on the Heap. Load each room, and add it to the RoomMap.
    // Get room ID. This is used to create the room.
    int roomId = 0;
    if (!levelFile.GetInteger(&roomId))
    {
      levelFile.ReportError("Expected Room ID.");
      return false;
    }

    Room* pRoom = new Room(roomId);
    pRoom->SetLevel(this);

    // Get the file for the room.
    string roomfilename;
    if (!levelFile.GetDataLine(&roomfilename))
    {
      levelFile.ReportError("Expected room file name.");
      return false;
    }

    if (!pRoom->Load(roomfilename))
    {
      return false;
    }

    m_rooms[roomId] = pRoom;
  }
  m_pRoom = 0; 

  // Get start room ID
  if (!levelFile.GetInteger(&m_startRoomId))
  {
    levelFile.ReportError("Expected start room ID.");
    return false;
  }

  // Get start orientation
  if (!m_startOrientation.Load(&levelFile))
  {
    return false;
  }

  // Get the Character Name for the level.
  if (!levelFile.GetDataLine(&m_playerName))
  {
    levelFile.ReportError("Expected character name.");
    return false;
  }

  // Get the MIDI file name for the level.
  if (!levelFile.GetDataLine(&m_midiFilename))
  {
    levelFile.ReportError("Expected song filename for level.");
    return false;
  }

  // Load level goal.
  if (!LoadLevelObjective(&levelFile))
  {
    levelFile.ReportError("Failed to load level objective.");
    return false;
  }
  
  if (!LoadGameObjects(&levelFile))
  {
    return false;
  }

  return true;
}

bool Level::LoadGameObjects(File* pf)
{
  // Get number of Game Objects.
  int numGameObjects = 0;
  if (!pf->GetInteger(&numGameObjects))
  {
    pf->ReportError("Expected number of game objects.");
    return false;
  }
  // Load each game object.
  for (int j = 0; j < numGameObjects; j++)
  {
    // Get the Game Object ID for this object.
    int id = 0;
    if (!pf->GetInteger(&id))
    {
      pf->ReportError("Expected Game Object ID.");
      return false;
    }

/*
    // Check that ID is unique. 
    if (Engine::Instance()->GetGameObject(id).GetPtr())
    {
      pf->ReportError("Non-unique ID for game object.");
      return false;
    }
*/

    // Get the type name, used by Factory to create a GameObject subclass.
    string gameObjectType;
    if (!pf->GetDataLine(&gameObjectType))
    {
      pf->ReportError("Expected game object type name.");
      return false;
    }

#ifdef _DEBUG
    std::string info = "Info: loading object ";
    info += ToString(id);
    info += ": ";
    info += gameObjectType;
    pf->ReportError(info);
#endif

    PPoolGameObject pGo;// = GameObjectFactory::Instance()->Create(gameObjectType);
    if (!pGo.GetPtr())
    {
      pf->ReportError("Bad type name.");
      return false;
    }
    pGo->SetId(id);
    pGo->SetLevel(this);

    // Get the room this Game Object lives/starts in.
    int room = 0;
    if (!pf->GetInteger(&room))
    {
      pf->ReportError("Expected room ID for Game Object.");
      return false;
    }
    pGo->SetRoomId(room);
    SetRoomId(room);

    if (!pGo->Load(pf))
    {
      return false;
    }

    // We expect "end" at the end of each object
    std::string endTest;
    if (!pf->GetDataLine(&endTest))
    {
      pf->ReportError("Expected \"end\".");
      return false;
    }
    if (endTest != "end")
    {
      pf->ReportError("Expected \"end\".");
      return false;
    }
    AddGameObject(pGo);
  }

  return true;
}

#ifdef SCENE_EDITOR
bool Level::SaveGameObjects(File* pf)
{
  int numGameObjects = 0;
  iterator it;
  for (it = Begin(); it != End(); ++it)
  {
    int roomId = (*it)->GetId();
    GameObjectMap& gmap = Engine::Instance()->GetGameObjects(GetId(), roomId);
    numGameObjects += gmap.size();
  }

  pf->WriteComment("// Number of game objects");
  pf->WriteInteger(numGameObjects);

  // For each roomID, get the map of game objects in the room.
  for (it = Begin(); it != End(); ++it)
  {
    int roomId = (*it)->GetId();
    GameObjectMap& gmap = Engine::Instance()->GetGameObjects(GetId(), roomId);
    GameObjectMap::iterator jt;
    for (jt = gmap.begin(); jt != gmap.end(); ++jt)
    {
      pf->WriteComment("//");
      pf->WriteComment("//");
      pf->WriteComment("// Start of Game Object");

      pf->WriteComment("// Game Object ID");
      pf->WriteInteger(jt->first);

      PPoolGameObject pGo = jt->second;

      pf->WriteComment("// Game Object type name");
      pf->Write(pGo->GetTypeName());

      pf->WriteComment("// Game Object room ID");
      pf->WriteInteger(roomId);

      // Write object specific data
      if (!pGo->Save(pf))
      {
        std::string error = std::string("Failed to save Game Object of type ") + 
          std::string(pGo->GetTypeName());
        pf->ReportError(error);
        return false;
      }

      // Mark end of object, helps debugging
      pf->Write("end");
    }
  }
  return true;
}
#endif

void Level::RefreshObjects()
{
  Engine::Instance()->ClearGameObjects();
  // TODO This requires us to store the game objects here. 
  for (GameObjectList::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
  {
    PPoolGameObject pGo = *it;
    Engine::Instance()->HoldGameObject(m_levelId, pGo->GetRoomId(), pGo);
  }
}

void Level::AddGameObject(PPoolGameObject pGo)
{
  // Create Bounding Sphere for game object. (Used for VFC and collision.)
  Matrix m;
  m.identity();
  pGo->CreateBoundingSphere(m);

  // Give GameObject the opportunity to hold a pointer to this level. Some
  // kinds of GameObject may find this useful.
  pGo->SetLevel(this);
  Engine::Instance()->HoldGameObject(m_levelId, pGo->GetRoomId(), pGo);

  // New - store the objecct here so we can refresh the level later, i.e.
  // instantly 'reload' it.
  m_gameObjects.push_back(pGo);
}

bool Level::LoadLevelObjective(File* pf)
{
  // The objective for the level is the ID of the target which the player
  // must intersect.
  if (!pf->GetInteger(&m_goalId))
  {
    pf->ReportError("Expected player goal ID.");
    return false;
  }

  return true;
}
/*
int Level::GetNumChicks() const
{
  return m_numChicks;
}
*/

bool Level::TestObjective()
{
  // TODO dynamically work out the objective from the level number.
  // I.e. levels get more difficult.
  // ?
  //return m_objective.IsSatisfied();
  return false; 
} 

void Level::GetHeightServer(HeightServer* pResult, const BoundingSphere& bs)
{
  if (m_pRoom.GetPtr())
  {
    m_pRoom->GetHeightServer(pResult, bs);
  }
}

void Level::SetRoomId(int roomId)
{
#ifdef _DEBUG
  std::cout << "Setting new room ID: " << roomId << "\n";
#endif

  // Make sure the room ID is valid. If not, no effect.
  if (roomId >= (int)m_rooms.size())
  {
    return;
  }
  // Get the new room object.
  PRoom pNewRoom = m_rooms[roomId];

  // If the new room pointer and old room pointer are the same, no effect.
  if (m_pRoom.GetPtr() == pNewRoom.GetPtr())
  {
    return;
  }
  m_pRoom = pNewRoom;

  m_pRoom->OnEntry();
}

Level::iterator Level::Begin()
{
  return m_rooms.begin();
}

Level::iterator Level::End()
{
  return m_rooms.end();
}

void Level::AddRoom(PRoom pRoom)
{
  int id = pRoom->GetId();
  pRoom->SetLevel(this);
  int numRooms = m_rooms.size();
  Assert(id == numRooms);
  m_rooms.push_back(pRoom);
  m_pRoom = pRoom;
}

void Level::SetMusicFilename(const std::string& s)
{
  m_midiFilename = s;
}

void Level::SetStartOrientation(const Orientation& o)
{
  m_startOrientation = o;
}

int Level::GetPlayAreaSize() const
{
  return m_playAreaSize;
}

void Level::SetPlayAreaSize(int s)
{
  m_playAreaSize = s;
}

std::string Level::GetLevelName() const
{
  return m_levelName;
}

void Level::SetLevelName(const std::string& name)
{
  m_levelName = name;
}

bool Level::IsCurrentRoomIndoors() const
{
  Assert(m_pRoom.GetPtr());
  return m_pRoom->IsIndoors();
}
}

