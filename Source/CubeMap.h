/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CubeMap.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CUBE_MAP_H_INCLUDED)
#define SCHMICKEN_CUBE_MAP_H_INCLUDED

#include <map>
#include "SharedPtr.h"

namespace Amju
{
class Level;
class BoundingBox;
class BoundingSphere;
class HeightServer;

// Hold state of cubes. A cube may be empty, or occupied by scenery 
// or a game object.
// This is to be used for 3D path finding, intersection tests, etc.
// Cubes are unit side length. This is not flexible for performance.
class CubeMap : public Shareable
{
public:
  struct Coord
  {
    Coord(int x, int y, int z);
    int m_x, m_y, m_z;
    bool operator<(const Coord&) const;
  };

  // TODO use bitfield to store additional info for occupying piece
  // Cubes can be set to empty, half or full, This lets us recognise
  // paths over sloping ground. Empty->half or half->full is traversable.
  typedef unsigned char CubeState;

  static const CubeState FLOOR;
  static const CubeState EMPTY;
  static const CubeState OCCUPIED;
  static const CubeState HALF;

public:
  CubeMap();

  // Create cube info, using the given level to get the scene and
  // objects in it.
  void Create(Level*);
  
  bool IsOccupied(const Coord&) const;

  bool IsFloor(const Coord& coord) const;

  void SetFloor(int f);

  void Set(const Coord&, CubeState);

  // Set each cube in the bounding box to state.
  void Set(const BoundingBox& box, CubeState state = OCCUPIED);

  // Set from a HeightServer. 
  // This should be able to handle slopes and irregular shapes.
  // May set cubes to half. state is only used if it's EMPTY, i.e. to
  // remove cubes from the map.
  void Set(const HeightServer& hs);
  void Unset(const HeightServer& hs);

  // Returns true if there is a horizontal unbroken floor between v1 and v2.
  bool IsHeightConstant(const Coord& v1, const Coord& v2) const;

  // Returns true if there are no sudden height changes from v1 to v2.
  // An empty->occupied change is not traversable, but empty<->half and
  // half<->full are traversable.
  bool IsTraversable(const Coord& v1, const Coord& v2) const;

  bool Intersects(const BoundingSphere&) const;

  bool Intersects(const BoundingBox&) const;

  bool IsBoxFullyOccupied(const BoundingBox&) const;

  void Clear();

#if defined(_DEBUG)
  void Draw();
#endif

private:
  // Don't expose this, just allow client to test for traversable path etc.
  CubeState Get(const Coord&) const;

  // Delete cube from map
  void Unset(const Coord& coord);

  // Implementation for setting and unsetting from a height server.
  void SetUnset(const HeightServer& hs, bool set);

private:
  // TODO use more efficent Map type for usage pattern

  // TODO Map coord to ref count if profiling shows it is a good idea. 
  typedef std::map<Coord, CubeState> Cubes;
  Cubes m_cubes;

  // Lowest y-value. Any coord below this is treated as an occupied cube.
  int m_floor;
};
}

#endif


