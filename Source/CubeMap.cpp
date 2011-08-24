/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CubeMap.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CubeMap.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Level.h"
#include "SolidComposite.h"
#include "SolidLeaf.h"
#include "Engine.h"
#include "Common.h"
#include "Snap.h"
#include "Takeable.h"
#include "HeightServer.h"
#include "SchAssert.h"

namespace Amju
{
const CubeMap::CubeState CubeMap::FLOOR =    0x11;
const CubeMap::CubeState CubeMap::EMPTY =    0x00;
const CubeMap::CubeState CubeMap::OCCUPIED = 0x01;
const CubeMap::CubeState CubeMap::HALF =     0x03;


CubeMap::Coord::Coord(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
}

bool CubeMap::Coord::operator<(const CubeMap::Coord& rhs) const
{
  if (m_x < rhs.m_x)
  {
    return true;
  }
  if (m_x > rhs.m_x)
  {
    return false;
  }
  if (m_y < rhs.m_y)
  {
    return true;
  }
  if (m_y > rhs.m_y)
  {
    return false;
  }
  return m_z < rhs.m_z;
}

#if defined(CUBEMAP_DEBUG)
void CubeMap::Draw()
{
  for (Cubes::iterator it = m_cubes.begin(); it != m_cubes.end(); ++it)
  {
    const Coord& c = it->first;
    CubeState cs = it->second;
    float h = 1.0f;
    if (cs == HALF)
    {
      h = 0.5f;
    }
    if (cs != EMPTY)
    {
      glBegin(GL_LINES);
      // bottom
      glVertex3f(c.m_x, c.m_y, c.m_z);
      glVertex3f(c.m_x, c.m_y, c.m_z + 1.0f);

      glVertex3f(c.m_x, c.m_y, c.m_z + 1.0f);
      glVertex3f(c.m_x + 1.0f, c.m_y, c.m_z + 1.0f);

      glVertex3f(c.m_x + 1.0f, c.m_y, c.m_z + 1.0f);
      glVertex3f(c.m_x + 1.0f, c.m_y, c.m_z);

      glVertex3f(c.m_x + 1.0f, c.m_y, c.m_z);
      glVertex3f(c.m_x, c.m_y, c.m_z);

      // top
      glVertex3f(c.m_x, c.m_y + h, c.m_z);
      glVertex3f(c.m_x, c.m_y + h, c.m_z + 1.0f);

      glVertex3f(c.m_x, c.m_y + h, c.m_z + 1.0f);
      glVertex3f(c.m_x + 1.0f, c.m_y + h, c.m_z + 1.0f);

      glVertex3f(c.m_x + 1.0f, c.m_y + h, c.m_z + 1.0f);
      glVertex3f(c.m_x + 1.0f, c.m_y + h, c.m_z);

      glVertex3f(c.m_x + 1.0f, c.m_y + h, c.m_z);
      glVertex3f(c.m_x, c.m_y + h, c.m_z);

      // sides
      glVertex3f(c.m_x, c.m_y, c.m_z);
      glVertex3f(c.m_x, c.m_y + h, c.m_z);

      glVertex3f(c.m_x, c.m_y, c.m_z + 1.0f);
      glVertex3f(c.m_x, c.m_y + h, c.m_z + 1.0f);

      glVertex3f(c.m_x + 1.0f, c.m_y, c.m_z + 1.0f);
      glVertex3f(c.m_x + 1.0f, c.m_y + h, c.m_z + 1.0f);

      glVertex3f(c.m_x + 1.0f, c.m_y, c.m_z);
      glVertex3f(c.m_x + 1.0f, c.m_y + h, c.m_z);
      glEnd();
    }
  }
}
#endif // CUBEMAP_DEBUG

CubeMap::CubeMap() 
{
  m_floor = 0;
}

void CubeMap::Clear()
{
  m_cubes.clear();
}

void CubeMap::SetFloor(int f)
{
  m_floor = f;
}

bool CubeMap::Intersects(const BoundingSphere& s) const
{
  BoundingBox b;
  b.SetFromSphere(s);
  return Intersects(b);
  return false;
}

bool CubeMap::Intersects(const BoundingBox& box) const
{
  int xmax = int(Snap(box.m_xmax, 1.0f)); 
  int xmin = int(Snap(box.m_xmin, 1.0f)); 
  int ymax = int(Snap(box.m_ymax, 1.0f)); 
  int ymin = int(Snap(box.m_ymin, 1.0f)); 
  int zmax = int(Snap(box.m_zmax, 1.0f)); 
  int zmin = int(Snap(box.m_zmin, 1.0f));

  for (int i = xmin; i < xmax; i++)
  {
    for (int j = ymin; j < ymax; j++)
    {
      for (int k = zmin; k < zmax; k++)
      {
        if (IsOccupied(Coord(i, j, k)))
        {
          return true;
        }
      }
    }
  }

  return false;
}

void FillCubes(CubeMap* pMap, PSolidComponent pSolid)
{
  SolidComposite* pComp = dynamic_cast<SolidComposite*>(pSolid.GetPtr());
  if (pComp)
  {
    // Recurse over children.
    int numChildren = pComp->GetNumChildren();
    for (int i = 0; i < numChildren; i++)
    {
      FillCubes(pMap, pComp->GetChild(i));
    }
    return;
  }

  SolidLeaf* pLeaf = dynamic_cast<SolidLeaf*>(pSolid.GetPtr());
  if (pLeaf)
  {
    pMap->Set(pLeaf->GetHeightServer());
    return;
  }
}

void CubeMap::Create(Level* pLevel)
{
  // Create cubes for static scene
  PSolidComponent pSolid = pLevel->GetScene();
  // Yuck. We should use Visitor.

  // This looks like it doesn't work, maybe HeightServer not set up yet ?
  FillCubes(this, pSolid);

  // Create cubes for bounding box for each Game Object.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    //GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    if (pVis && 
        pVis->GetState() == UNKNOWN && 
        pVis->GetCollisionVol())
    {
      Set(*(pVis->GetCollisionVol()));
    }
  }
}
  
bool CubeMap::IsFloor(const Coord& coord) const
{
  if (coord.m_y < m_floor)
  {
    return true;
  }
  return false;
}

bool CubeMap::IsOccupied(const Coord& coord) const
{
  if (IsFloor(coord))
  {
    return true;
  }
  Cubes::const_iterator it = m_cubes.find(coord);
  if (it == m_cubes.end())
  {
    return false;
  }
  CubeState c = it->second;
  return c != EMPTY;
}

CubeMap::CubeState CubeMap::Get(const Coord& coord) const
{
  // handle floor values.
  if (IsFloor(coord))
  {
    return FLOOR;
  }
  Cubes::const_iterator it = m_cubes.find(coord);
  if (it == m_cubes.end())
  {
    return EMPTY;
  }
  CubeState c = it->second;
  return c;
}

bool CubeMap::IsBoxFullyOccupied(const BoundingBox& bb) const
{
  for (int i = (int)bb.m_xmin; i < bb.m_xmax; i++)
  {
    for (int j = (int)bb.m_ymin; j < bb.m_ymax; j++)
    {
      for (int k = (int)bb.m_zmin; k < bb.m_zmax; k++)
      {
        CubeState cs = Get(Coord(i, j, k));
        if (cs != OCCUPIED && cs != FLOOR)
        {
          return false;
        }
      }
    }
  }
  return true;
}

void CubeMap::Set(const Coord& coord, CubeState state)
{
  m_cubes[coord] = state;
}

void CubeMap::Set(const BoundingBox& box, CubeState state)
{
  // TODO This fills in the centres of boxes, but in fact we only need the
  // shell.
  int xmax = int(Snap(box.m_xmax, 1.0f)); 
  int xmin = int(Snap(box.m_xmin, 1.0f)); 
  int ymax = int(Snap(box.m_ymax, 1.0f)); 
  int ymin = int(Snap(box.m_ymin, 1.0f)); 
  int zmax = int(Snap(box.m_zmax, 1.0f)); 
  int zmin = int(Snap(box.m_zmin, 1.0f));

  for (int i = xmin; i < xmax; i++)
  {
    for (int j = ymin; j < ymax; j++)
    {
      for (int k = zmin; k < zmax; k++)
      {
        Set(Coord(i, j, k), state);
      }
    }
  }
}

void CubeMap::Unset(const Coord& coord)
{
  m_cubes.erase(coord);
}

void CubeMap::Set(const HeightServer& hs)
{
  SetUnset(hs, true);
}

void CubeMap::SetUnset(const HeightServer& hs, bool set)
{
  // Get the bounding box. 
  // For each x,z get the height. Set cubes below the height. 
  // If the height isn't an integer, set the cube above the top integer to HALF.
  // This handles simple block and ramp shapes.
  const BoundingBox& box = hs.GetBoundingBox();

  int xmax = int(Snap(box.m_xmax, 1.0f)); 
  int xmin = int(Snap(box.m_xmin, 1.0f)); 
  //int ymax = int(Snap(box.m_ymax, 1.0f)); 
  int ymin = int(Snap(box.m_ymin, 1.0f)); 
  int zmax = int(Snap(box.m_zmax, 1.0f)); 
  int zmin = int(Snap(box.m_zmin, 1.0f));

  float xoffset = 0;
  float yoffset = 0;

  for (int i = xmin; i < xmax; i++)
  {
    for (int j = zmin; j < zmax; j++)
    {
      // Get the height at this (x, z)
      float height = 0;
      if (hs.GetHeight(((float)i + xoffset), ((float)j + yoffset), &height))
      {
        float intheight = (float)(int)(height + SMALLEST); // sigh, fix float inaccuracy
        float frac = height - intheight; 
        if (frac > SMALLEST)
        {
          if (set)
          {
            Set(Coord(i, (int)intheight, j), HALF); 
          }
          else
          {
            Set(Coord(i, (int)intheight, j), EMPTY); 
          }

          for (int k = (int)intheight; k > ymin; k--)
          {
            if (set)
            {
              Set(Coord(i, k - 1, j), OCCUPIED);
            }
            else
            {
              Set(Coord(i, k - 1, j), EMPTY);
            }
          }
        }
        else
        {
          for (int k = (int)intheight - 1; k >= ymin; k--)
          {
            if (set)
            {
              Set(Coord(i, k, j), OCCUPIED);
            }
            else
            {
              Set(Coord(i, k, j), EMPTY);
            }
          }
        }
      }
      else
      {
/*
        Assert(0);
*/
      }
    }
  }

  // TODO
  // Now we must handle shapes with unoccupied cubes, like bridges.
  // Check each (x, y, z) cube using intersection test. If no intersection,
  //  clear the cube.
}

void CubeMap::Unset(const HeightServer& hs)
{
  // As for set, but delete any occupied or half-occupied cube.
  SetUnset(hs, false);
}

// Used by Bresenham code below
int sign(int a)
{
  if (a<0) 
    return(-1);
  else if (a==0) 
    return(0);
  else 
    return(1);
}

bool CubeMap::IsTraversable(
  const Coord& v1, 
  const Coord& v2) const
{
  if (IsFloor(v1) && IsFloor(v2))
  {
    return true;
  }

  // Uses Bresenham's straight line algo.
  int h = v1.m_y;
  bool swap = false;
  int xa = v1.m_x;
  int ya = v1.m_z;
  int xb = v2.m_x;
  int yb = v2.m_z;
  int dy = abs(yb - ya);
  int dx = abs(xb - xa);
  int s1 = sign(xb - xa);
  int s2 = sign(yb - ya);
  int x = xa;
  int y = ya;
  if (dy > dx)
  {
    int temp = dx;
    dx = dy;
    dy = temp;
    swap = true;
  }
  int e = 2 * dy - dx;

  CubeState previousState = Get(Coord(x, h, y)); 
  previousState &= 0x0f;

  if (dx == 0)
  {
    // Difference in height only. This is a drop - it's NOT traversable.
    return false;
  }

  // idealheight is the height we expect h to be assuming the floor has a 
  // constant gradient. If h deviates from this too much, we reject the line.
  // This is hacky but culls lines that, in practice, NPCs get stuck on.
  float hdest = v2.m_y;
  float stepheight = (hdest - h) / dx;
  float idealheight = h;

  for (int i = 0; i < dx; i++)
  {
    idealheight += stepheight;

    // TODO FIX THIS!
/*
    if (h < (idealheight - 1.0f) || h > (idealheight + 1.0f))
    {
      return false;
    }
*/
    CubeState thisState = Get(Coord(x, h, y));
    thisState &= 0x0f;

    // Compare this state with the previous state. If we go from half to empty
    // (which is traversable), drop the height.
    // If we go from half to full, increase the height.
    if (previousState == HALF && thisState == HALF)
    {
      // Traversable, do nothing
    }
    else if (previousState == HALF && thisState == OCCUPIED)
    {
      // Traversable so long as height at h+1 is zero
      if (Get(Coord(x, h + 1, y)) != EMPTY)
      {
        return false;
      }
    }
    else if (previousState == HALF && thisState == EMPTY)
    {
      // Traversable so long as height at h-1 is 1
      if (Get(Coord(x, h - 1, y)) != OCCUPIED)
      {
        return false;
      }
    }
    else if (previousState == EMPTY && thisState == EMPTY)
    {
      // Traversable so long as height at h-1 is half or 1
      CubeState cs = Get(Coord(x, h - 1, y));
      cs &= 0x0f;
      if (cs == EMPTY)
      {
        return false;
      }
      // Change height if necessary
      if (cs == HALF)
      {
        h--;
        thisState = HALF;
      }
    }
/*
    else if (previousState == EMPTY && thisState == HALF)
    {
      // Traversable, do nothing
    }
*/
    else if (previousState == EMPTY && thisState == OCCUPIED)
    {
      // Not traversable
      return false;
    }
    else if (previousState == OCCUPIED && thisState == EMPTY)
    {
      // Not traversable
      return false;
    }
/*
    else if (previousState == OCCUPIED && thisState == HALF)
    {
      // Traversable, do nothing
    }
*/
    else if (previousState == OCCUPIED && thisState == OCCUPIED)
    {
      // Traversable so long as height at h+1 is zero or half
      CubeState cs = Get(Coord(x, h + 1, y));
      cs &= 0x0f;
      if (cs == OCCUPIED)
      {
        return false;
      }
      // Change height if necessary
      if (cs == HALF)
      {
        h++;
        thisState = HALF;
      }
    }

    previousState = thisState;

    while (e >= 0)
    {
      if (swap)
      {
        x += s1;
      }
      else
      {
        y += s2;
      }
      e -= 2 * dx;
    }
    if (swap)
    {
      y+=s2;
    }
    else 
    {
      x+=s1;
    }
    e += 2 * dy;
  }

  // Is the height we have reached by tracersing the ground at the same height
  // as the destination point ?
  // Allow some tolerance TODO CONFIG
  if (h > (hdest + 1.0f) || h < (hdest - 1.0f))
  {
    return false;
  }
  // Path is traversable!
  return true;
}

bool CubeMap::IsHeightConstant(
  const CubeMap::Coord& v1, 
  const CubeMap::Coord& v2) const
{
  // Heights of v1 and v2 should be the same.
  // If heights are below floor, we know all cubes will be occupied.
  if (IsFloor(v1) && IsFloor(v2))
  {
    return true;
  }

  // Uses Bresenham's straight line algo.
  int h = v1.m_y;
  bool swap = false;
  int xa = v1.m_x;
  int ya = v1.m_z;
  int xb = v2.m_x;
  int yb = v2.m_z;
  int dy = abs(yb - ya);
  int dx = abs(xb - xa);
  int s1 = sign(xb - xa);
  int s2 = sign(yb - ya);
  int x = xa;
  int y = ya;
  if (dy > dx)
  {
    int temp = dx;
    dx = dy;
    dy = temp;
    swap = true;
  }
  int e = 2 * dy - dx;
  for (int i = 0; i < dx; i++)
  {
    if (!IsOccupied(Coord(x, h, y)))
    {
      // Cube not occupied, so there is a gap in the floor.
      return false;
    }
    while (e >= 0)
    {
      if (swap)
      {
        x += s1;
      }
      else
      {
        y += s2;
      }
      e -= 2 * dx;
    }
    if (swap)
    {
      y+=s2;
    }
    else 
    {
      x+=s1;
    }
    e += 2 * dy;
  }
  return true;
}
}

