/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Level.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEVEL_H_INCLUDED)
#define LEVEL_H_INCLUDED

//#include "Common.h"
#include "Room.h"
#include "States.h"
#include "Orientation.h"
#include "Objective.h"
#include "GameObject.h"
#include "Camera.h"
#include "SharedPtr.h"
#include <list>
#include <map>
#include <utility> // pair
#include <string>
#include <vector>

namespace Amju
{
class SolidComponent;
class Locator;
class Character;
class HeightServer;

class Level : public Shareable
{
public:
  ~Level();

  int GetId() { return m_levelId; }
  void SetId(int id);

  // Add Game Object to the Level. (In fact the Engine holds all pointers to
  // GameObjects.)
  void AddGameObject(PGameObject pGo);

  // Re-add all objects in this level to the Engine.
  // This is to refresh a level without reloading it. 
  void RefreshObjects();

  void GetHeightServer(HeightServer* pResult, const BoundingSphere& bs);

  // Get the static scenery for the current room.
  PSolidComponent GetScene(); 
  // Set static scenery for current room.
  void SetScene(PSolidComponent);

  // Draw light(s). This only has an effect if the current room is indoors.
  // Outdoor rooms are lit by the DayNightSky.
  void DrawLight();
  
  // Draw static scenery.
  int Draw();

  int GetNumberOfPolygons() const;

  bool Load(const std::string& filename);

  // Called by Load(), but maybe used independently: load the game objects
  // for this level.
  bool LoadGameObjects(File*);

#ifdef SCENE_EDITOR
  bool SaveGameObjects(File*);
#endif

  // Returns true if the objective for the level has been completed.
  bool TestObjective();

  // Get Music file for the level. (Music is not currently per-room.)
  std::string GetMusicFilename() const { return m_midiFilename; }
  void SetMusicFilename(const std::string& );

  // Get character ID for this level.
  std::string GetPlayerName() const { return m_playerName; }

  // Get orientation for player when this level is entered.
  // (Only one entry point is allowed therefore.)
  Orientation GetStartOrientation() const { return m_startOrientation; }
  void SetStartOrientation(const Orientation&);

  // Set current room: no effect if room ID is invalid. If it is valid, the 
  // current room is changed. If the old room and new room are the same, the
  // player coords are still changed.
  void SetRoomId(int roomId);

  // Get the current room ID.
  int GetRoomId() const;

  // Get the room player is in when theis level is entered. (This allows for 
  // only one entrance to level.)
  int GetStartRoomId() const { return m_startRoomId; }

  bool IsCurrentRoomIndoors() const;

//protected:
  // LevelFactory must be able to create Levels.
  // Scene Editor must be able to create Levels.
  Level(); // Only LevelServer is allowed to construct Levels.

  PRoom GetRoom() { return m_pRoom; }

  // Add a new room to the Level. Used by Editor.
  void AddRoom(PRoom pRoom);

  int GetPlayAreaSize() const;
  void SetPlayAreaSize(int s);

  std::string GetLevelName() const;
  void SetLevelName(const std::string&);

protected:
  bool LoadLevelObjective(File* pf);

  // Filename of MIDI music for the level.
  // TODO Should be a Music Object responsible for the music for the level.
  std::string m_midiFilename;

  // ID for the level, so GameState can map levels to lists of objects 
  // collected/killed etc.
  int m_levelId;

  // ID of the Level after this one - player must complete objective 
  // (see below) to get there.
  int m_nextLevel;

  // Initial position/direction of player on entering the level.
  Orientation m_startOrientation;

  // ID of start room in level.
  int m_startRoomId;

  // Character ID for this level. The ID must be recognised by the 
  // CharacterManager. Having the player name per-level means you can have
  // different characters in different levels, which may be useful. 
  std::string m_playerName;

  // The level objective: a list of <object ID, state> pairs, all of which must
  // match.
  Objective m_objective;

  // Level is made up of one or more connected "rooms", or enclosed spaces.
  // Map of room IDs to rooms.
  // IDs start at zero and are contiguous, so we can use a vector.
  typedef std::vector<PRoom> RoomMap;
  RoomMap m_rooms;
  // Point to the current room. Not the room ID, to avoid looking up the ID 
  // in the RoomMap all the time. 
  PRoom m_pRoom;

  int m_goalId;

  int m_playAreaSize;

  std::string m_levelName;

public:
  // Allow access to the map of rooms. This is for Scene Editor.
  typedef RoomMap::iterator iterator;
  iterator Begin();
  iterator End();
 
  int GetNumberOfRooms() const;
  Room* GetRoom(int);

#if defined(SCENE_EDITOR)
public:
  bool Save(const std::string& filename);

#endif

protected:
  // New: store the game objects here, so we can instantly 'reload' the 
  // level if required.
  typedef std::vector<PGameObject> GameObjectList;
  GameObjectList m_gameObjects;

};

typedef SharedPtr<Level> PLevel;
}
#endif

