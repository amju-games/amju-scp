/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Room.cpp,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Room.h"
#include "Engine.h"
#include "SolidComponent.h"
#include "CompositeFactory.h"
#include "Frustum.h"
#include "File.h"
#include "StringUtils.h"

using namespace std;

namespace Amju
{
Room::Room(int roomId) :
  m_roomId(roomId),
  m_pLevel(0),
  m_fogDensity(0)
{
  m_isIndoors = false;
}

Room::~Room()
{
  // Delete locators.. left to Engine
  // Delete platforms.. left to Engine
}

Level* Room::GetLevel()
{
  return m_pLevel;
}

void Room::SetLevel(Level* p)
{
  m_pLevel = p;
}

void Room::OnEntry()
{
  m_lighting.Init();
}

int Room::Draw()
{
  // Draw static scene. Return number of polys drawn.
  int polys = 0;
  if (m_pScene.GetPtr())
  {
    AmjuGL::PushMatrix();
    Frustum& f = *Frustum::Instance(); 
    polys = m_pScene->CullDraw(f);
    AmjuGL::PopMatrix();
    
    // Show bounding spheres, for debugging.
    if (Engine::Instance()->DoDrawSceneSpheres())
    {
      m_pScene->DrawSphere();
    }
  }
  return polys;
}

void Room::DrawLight()
{
  if (IsIndoors())
  {
    m_lighting.DrawLight();
  }
}

void Room::GetHeightServer(HeightServer* pResult, const BoundingSphere& bs)
{
  // The room doesn't have a HeightServer which covers the whole room, as this 
  // doesn't scale to big rooms. Instead, the HeightServer is created on demand
  // by glueing together the HeightServers owned by Leaves in the room.
  // If a Leaf does not intersect the Game Object which is interested in the
  // HeightServer, we don't need to glue the HeightServer for that Leaf.

  // Walk the scene, getting all leaves which intersect the bounding sphere.
  // (Visitor could be used, but I've just added another virtual
  // function to the SolidComponent heirarchy.)

  // Add the HeightServer for each intersecting Leaf to our HeightServer.
  if (m_pScene.GetPtr())
  {
    m_pScene->AddHeights(pResult, bs);
  }
  // Other Game Objects which intersect the sphere may also contribute to the 
  // HeightServer. This lets Game Objects correctly collide with each other.
}

bool Room::Load(const string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }

  m_name = filename; // for improved error reporting
  
  // Get lighting for the room. Lighting is in a separate file as many rooms
  // can have the same lighting.
  string lightfile;
  if (!f.GetDataLine(&lightfile))
  {
    f.ReportError("Expected lighting file name.");
    return false;
  }

  if (!m_lighting.Load(lightfile))
  {
    f.ReportError("Failed to load lighting for room.");
    return false;
  }

  // Get static scenery
  string sceneryName;
  if (!f.GetDataLine(&sceneryName))
  {
    f.ReportError("Expected composite scene name.");
    return false;
  }

  m_pScene = SolidComponent::LoadSolid(sceneryName);
  if (!m_pScene.GetPtr())
  {
    f.ReportError("Failed to load scene.");
    return false;
  }

  Matrix identityMatrix;
  identityMatrix.identity();
  m_pScene->CreateBoundingSphere(identityMatrix);

  // This static scenery data is converted to absolute coords, and "height"
  // polygons - i.e. floors etc - go in the Height Server.
  // StoreHeights() recurses - start off with the identity matrix.
  Matrix m;
  m.identity();
  m_pScene->StoreHeights(m);

  // Get Indoors flag. 1=outdoors, 0=indoors.
  int indoors = 0; 
  if (!f.GetInteger(&indoors))
  {
    f.ReportError("Expected indoors flag.");
    return false;
  } 
  m_isIndoors = (indoors != 1);

  if (!f.GetFloat(&m_fogDensity))
  {
    f.ReportError("Expected fog density.");
    return false;
  }
/*
  // Create visibility graphs up front. This is to avoid embarrassing pauses
  // during the game, this being an expensive job - O(n^2) line-of-sight tests.
  // Doing it this way means more config, of course.
  // Get the number of visibility graphs to create.
  int numVisGraphs = 0;
  if (!f.GetInteger(&numVisGraphs))
  {
    f.ReportError("Expected number of visibility graphs to create.");
    return false;
  }
  // For each graph, get the radius of the capsule which is used for the line-
  // of-sight tests.
  for (int i = 0; i < numVisGraphs; i++)
  {
    float visGraphRadius = 0;
    if (!f.GetFloat(&visGraphRadius))
    {
      f.ReportError("Expected vis. graph radius.");
      return false;
    }
    // This creates the graph if there is no existing one for this room for the
    // given radius. So the next time the graph is requested there shouldn't be
    // a pause.
    GetVisibilityGraph(visGraphRadius);
  }
*/
  return true;
}

#if defined(SCENE_EDITOR)
bool Room::Save(const std::string& filename)
{
  File f;
  if (!f.OpenWrite(filename))
  {
    f.ReportError("Couldn't open file for writing.");
    return false;
  }

  // Lighting filename
  f.WriteComment("// Lighting filename");
  f.Write("normal.lite"); // TODO

  // Scene (file)name
  f.WriteComment("// Scenery name");
  f.Write(StripPath(m_pScene->GetName()));

  // Indoors/outdoors flag
  f.WriteComment("// Indoor/outdoor flag");
  f.WriteInteger(m_isIndoors ? 0 : 1);

  // Fog
  f.WriteComment("// Fog density");
  f.WriteFloat(m_fogDensity);

  // Visibility graphs
  f.WriteComment("// Visibility graphs");
  f.WriteInteger(m_visibilityGraphs.size());
  for (VisGraphMap::iterator it = m_visibilityGraphs.begin(); it != m_visibilityGraphs.end(); ++it)
  {
    f.WriteComment("// Vis graph radius");
    f.WriteFloat(it->first);
  }
  return true;
}
#endif

int Room::GetNumberOfPolygons() const
{
  int scenePolys = m_pScene->GetNumberOfPolygons();
  return scenePolys;
}

}

