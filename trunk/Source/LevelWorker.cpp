/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorker.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorker.h"
#include "Engine.h"
#include "SolidGameObject.h"
#include "Level.h"
#include "LevelFactory.h"

namespace Amju
{
int LevelWorker::s_seed = 0;

LevelWorker::LevelWorker() : m_pFactory(0)
{
}

void LevelWorker::SetFactory(LevelFactory* pFactory)
{
  m_pFactory = pFactory;
}

int LevelWorker::GetUniqueId()
{
  static int id = 100;
  ++id;
  return id;
}

// Return random float [0..1)
float LevelWorker::Rnd()
{
  return (float)rand() / (float)RAND_MAX;//genrand_real2();
}

void LevelWorker::RndSeed(int s)
{
  s_seed = s;
  // Seed random number code
  //init_genrand(s_seed * 1000 + 1000);
}

// Convert a random number 0..1 to a position between -gridSize and gridSize,
//  which is a multiple of gridSpacing.
float RndToPos(float rnd, float gridSize, float gridSpacing)
{
  // rand1 is a random no from -1 to +1, not incl.
  float rand1 = (rnd - 0.5f) * 1.99f; // not 2, so we don't hit -1 or +1

  // rand2 is an integer between -gridsize and +gridsize, rounded down.
  int rand2 = (int)(rand1 * gridSize);

  // Finally, get an integer which is a multiple of gridSize, and
  // lies between -gridSize * gridSpacing and +gridSize * gridSpacing.
  int r = rand2 * (int)gridSpacing;

  return (float)r;
}

bool LevelWorker::GetOrientation(
  const BoundingBox& bb, 
  int absPlayAreaSize,
  Orientation* pResult)
{
  // Space between positions.
  int gridSpacing = (int)Engine::Instance()->GetConfigFloat("level_grid_spacing");
  // gridSize is number of different positions in x and z on each side of the 
  // origin. I.e. area side length is 2 * absPlayAreaSize.
  float gridSize = (float)absPlayAreaSize / (float)gridSpacing;

  // Get candidate list of available positions.
  // If there are none, we fail.
  // This is better than looping forever looking for a spot!
  // The client can increase the play area size in response to this error.

  typedef std::vector<Orientation> Candidates;
  Candidates candidates;
  float xmin = RndToPos(0, gridSize, gridSpacing);
  float xmax = RndToPos(1, gridSize, gridSpacing);
  float zmin = xmin; // play area is square
  float zmax = xmax;
  for (float x = xmin; x <= xmax; x += gridSpacing)
  {
    for (float z = zmin; z <= zmax; z += gridSpacing)
    {
      // TODO find y
      Orientation o(x, 0, z, 0, 0, 0);

      BoundingBox bbCopy(bb);
      bbCopy.Translate(x, 0, z);
      bool conflict = false; //pCubeMap->Intersects(bbCopy);
      if (!conflict)
      {
        candidates.push_back(o);
      }
    }
  }
  
  // No candidates ?
  if (candidates.empty())
  {
    // Start building the level again, with a larger play area.
    return false;
  }

  int num = candidates.size();
  int index = (int)(Rnd() * num);
  Orientation o = candidates[index];
  BoundingBox bbCopy(bb);
  bbCopy.Translate(o.GetX(), 0, o.GetZ());
  //pCubeMap->Set(bbCopy);
  *pResult = o;

  return true;
}

bool LevelWorker::GetOrientationAboveWater(
  Level* pLevel,
  Orientation* pResult)
{
    // TODO This implementation loops until it finds a good spot.
    // This could loop forever!!!!!

    float waterLevel = 0;
    bool foundGoodPos = false;
    while (!foundGoodPos)
    {
      if (!GetOrientation(pLevel->GetPlayAreaSize(), pResult))
      {
        return false; // play area too small
      }

      // Get height of ground
      // Make a bounding sphere at the (x, z) coord of the object. 
      // The y-coord/radius will have to be a guess which will ensure the
      // sphere will enclose the ground poly at (x, z).
      // Get a HeightServer containing the height polys which intersect 
      //  the sphere, then get the height.
      float y = 0;
      float r = 10.0f; // TODO CONFIG
      BoundingSphere bs(VertexBase(pResult->GetX(), y, pResult->GetZ()), r);
      HeightServer hs;
      pLevel->GetHeightServer(&hs, bs);
      if (!hs.GetHeight(pResult->GetX(), pResult->GetZ(), &y))
      {
        // Sphere not big enough, or terrain not built, etc.
        Assert(0);
        return false;
      }
      if (y >= waterLevel)
      {
        pResult->SetY(y);
        foundGoodPos = true;
      }
    }
    return true;
}

bool LevelWorker::GetOrientation(
  int absPlayAreaSize,
  Orientation* pResult)
{
  // This version of the function DOES NOT use a Cube Map to ensure that
  // objects do not intersect.

  // Space between positions.
  static int gridSpacing = (int)Engine::Instance()->GetConfigFloat("level_grid_spacing");

  // gridSize is number of different positions in x and z on each side of the 
  // origin. I.e. area side length is 2 * absPlayAreaSize.
  float gridSize = (float)absPlayAreaSize / (float)gridSpacing;

  // Get candidate list of available positions.
  // If there are none, we fail.
  // This is better than looping forever looking for a spot!
  // The client can increase the play area size in response to this error.

  // Optimized version
  // -----------------
  float x = RndToPos(Rnd(), gridSize, gridSpacing);
  float z = RndToPos(Rnd(), gridSize, gridSpacing);
  Orientation o(x, 0, z, 0, 0, 0);
  *pResult = o;

/*
  typedef std::vector<Orientation> Candidates;
  Candidates candidates;
  float xmin = RndToPos(0, gridSize, gridSpacing);
  float xmax = RndToPos(1, gridSize, gridSpacing);
  float zmin = xmin; // play area is square
  float zmax = xmax;
  for (float x = xmin; x <= xmax; x += gridSpacing)
  {
    for (float z = zmin; z <= zmax; z += gridSpacing)
    {
      // TODO find y
      Orientation o(x, 0, z, 0, 0, 0);
      candidates.push_back(o);
    }
  }
  
  if (candidates.empty())
  {
    return false;
  }

  int num = candidates.size();
  int index = (int)(Rnd() * num);
  Orientation o = candidates[index];
  //BoundingBox bbCopy(bb);
  //bbCopy.Translate(o.GetX(), 0, o.GetZ());
  //pCubeMap->Set(bbCopy);
  *pResult = o;
*/

  return true;
}

std::string LevelWorker::GetInfoString() const
{
  return m_infoString;
}

void LevelWorker::AddGameObject(Level* pLevel, PoolGameObject* pGo)
{
  GetFactory()->SetWorkerForObject(this, pGo->GetId());
  pLevel->AddGameObject(pGo);
}

LevelFactory* LevelWorker::GetFactory()
{
  Assert(m_pFactory);
  return m_pFactory;
}

}

