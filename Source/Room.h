/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Room.h,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if !defined(ROOM_H_INCLUDED)
#define ROOM_H_INCLUDED

#include "Common.h"
#include "SolidComponent.h"
#include "States.h"
#include "Lighting.h"
#include "SharedPtr.h"
#include <map>
#include <utility> // pair
#include <string>
#include <vector>

namespace Amju
{
class Locator;
class GameObject;
class Character;
class HeightServer;
class Level;

// A room is a part of a level. It may really look like a room, but can also
// be outdoors. It is connected to other Rooms by Portals.
class Room : public Shareable
{
public:
  Room(int roomId);
  virtual ~Room();

  int GetId() const { return m_roomId; }

  std::string GetName() const { return m_name; }
  void SetName(const std::string& name) { m_name = name; }

  Level* GetLevel();
  void SetLevel(Level*);

  int GetNumberOfPolygons() const;

  // Populates a HeightServer on
  // the fly, which only covers Leaves which intersect the given Bounding Sphere.
  void GetHeightServer(HeightServer* pResult, const BoundingSphere& bs);

  // Get highest height in y-range at the 2D coords (x, z).
  bool GetHeight(float x, float ymax, float ymin, float z, float* pResult);

  // Call this function when the room is first entered. 
  virtual void OnEntry();

  // Draw static scenery. Returns number of polygons drawn.
  int Draw();
  
  // Set up lighting. This only affects indoor rooms. Outdoor rooms are lit by the 
  // DayNightSky.
  void DrawLight();

  // Draw things which interact with player.
  void DrawGameObjects();

  bool Load(const std::string& filename);

  bool IsIndoors() const { return m_isIndoors; }

  PSolidComponent GetScene() { return m_pScene; }
  // Set scene, used by Editor, LevelFactory
  void SetScene(PSolidComponent pScene) { m_pScene = pScene; }
  
  float GetFogDensity() const { return m_fogDensity; }

protected:
  // Static Scenery.
  PSolidComponent m_pScene;

  // True if the room is indoors, meaning we use room lighting rather than 
  // the DayNightSky.
  bool m_isIndoors;

  // Each room has a unique ID for the level it belongs to.
  int m_roomId;
  
  // Pointer to level which owns this room.
  Level* m_pLevel;

  // The room's lighting. This is only used if the room is Indoors.
  // TODO Make this a pointer, with object managed by a server.
  Lighting m_lighting;

  // true if there is water in this room.
  bool m_hasWater;
  // If there's water in this room, this is the current height.
  float m_waterHeight;

  // Density of fog in this room (zero == no fog)
  float m_fogDensity;

  // Filename for this room, used for stats reporting.
  std::string m_name;


#if defined(SCENE_EDITOR)
public:
  bool Save(const std::string& filename);

  void SetId(int id) { m_roomId = id; }
  void SetIndoors(bool b) { m_isIndoors = b; }
  void SetFogDensity(float f) { m_fogDensity = f; }

#endif  
};

typedef SharedPtr<Room> PRoom;
}
#endif

