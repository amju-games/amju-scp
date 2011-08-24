/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PickHelper.cpp,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "PickHelper.h"
#include "Engine.h"
#include "Level.h"
#include "Frustum.h"
#include "Mouse.h"
#include "Colour.h"
#include "StringUtils.h"
#include "TextWriter.h"

namespace Amju
{
void PickHelper::DebugDraw()
{
#if defined(_DEBUG)
  int size = m_selectedCubes.size();
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  Engine::Instance()->PushColour(0, 0, 0, 1.0f);
  //glLineWidth(2);
  for (int i = 0; i < size; i++)
  {
    const VertexBase& v = m_selectedCubes[i];
    BoundingBox bb(v.x, v.x + 1.0f, v.y, v.y + 1.0f, v.z, v.z + 1.0f);
    bb.Draw();
  }
  //glLineWidth(1);
  Engine::Instance()->PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
#endif

}

bool PickHelper::PickSpaceDrawOneSection(
  float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float size)
{
  // Cube colour. We only need one colour because we draw the cubes one at a time.
  Colour col(1.0f, 1.0f, 1.0f, 1.0f);

  Engine::Instance()->PushAbsColour(col.m_r, col.m_g, col.m_b, col.m_a);
  // Draw a cube
  BoundingBox bb(xmin, xmax, ymin, ymax, zmin, zmax);
  // Disable back-face culling. This is so cubes close to the camera don't get
  // culled and so ignored.
//  AmjuGL::Disable(GL_CULL_FACE);
  bb.DrawSolid();
//  glFlush();// TODO TEMP TEST
  //SwapBuffers(wglGetCurrentDC()); // TODO TEMP TEST
  //Sleep(50); // TODO TEMP TEST
  Engine::Instance()->PopColour();

  // Get the colour under the mouse cursor
  Colour c = PickHelper::ReadCursorColour();
 
  // Get rid of the cube we just drew. 
  Engine::Instance()->PushAbsColour(0, 0, 0, 1.0f);
  bb.DrawSolid();
  //glFlush();// TODO TEMP TEST
  //SwapBuffers(wglGetCurrentDC());// TODO TEMP TEST
  //Sleep(50);// TODO TEMP TEST
  Engine::Instance()->PopColour();
//  AmjuGL::Enable(GL_CULL_FACE);

  // If the cursor colour is the cube colour, the mouse selects the cube.
  bool b = (c == col);
  // TODO TEMP TEST
  //bool b = !(c == Colour(0, 0, 0, 1.0f));
  if (b)
  {
    // Subdivide the section. If the current size is small enough, add the
    // cube to the list of selected cubes.
    if (size > 1.0f) // TODO cube size
    {
      float newx = (xmin + xmax)/ 2.0f;
      float newy = (ymin + ymax)/ 2.0f;
      float newz = (zmin + zmax)/ 2.0f;
      float newsize = size / 2.0f;
      PickSpaceDrawSections(newx, newy, newz, newsize);
    }
    else
    {
      m_selectedCubes.push_back(VertexBase(xmin, ymin, zmin));
    }
  }
  return b;
}

void PickHelper::PickSpaceDrawSections(float x, float y, float z, float s)
{
  // Draw 8 sections around (x, y, z) in false colours.
  // Each section is recursively subdivided if the mouse cursor hits it, so
  // we get a list of all unit cubes which the mouse cursor points to.
  PickSpaceDrawOneSection(x - s, x, y, y + s, z, z + s, s);
  PickSpaceDrawOneSection(x, x + s, y, y + s, z, z + s, s);
  PickSpaceDrawOneSection(x - s, x, y - s, y, z, z + s, s);
  PickSpaceDrawOneSection(x, x + s, y - s, y, z, z + s, s);
  PickSpaceDrawOneSection(x - s, x, y, y + s, z - s, z, s);
  PickSpaceDrawOneSection(x, x + s, y, y + s, z - s, z, s);
  PickSpaceDrawOneSection(x - s, x, y - s, y, z - s, z, s);
  PickSpaceDrawOneSection(x, x + s, y - s, y, z - s, z, s);
}
/*
struct CubeRemover
{
  CubeRemover(const VertexBase& v) : m_v(v) {}
  bool operator()(const VertexBase& v)
  {
    static const float T = 1.1f;

    if (v.x < (m_v.x - T) ||
        v.x > (m_v.x + T) ||
        v.y < (m_v.y - T) ||
        v.y > (m_v.y + T) ||
        v.z < (m_v.z - T) ||
        v.z > (m_v.z + T) )
    {
      return true;
    }
    return false;
  }
  VertexBase m_v;
};
*/
bool PickHelper::PickSpaceDraw()
{
  // Draw false colours to back buffer to work out where the mouse cursor is
  // pointing to.

  // Draw each octant in turn - because mouse cursor can point to more than one,
  // and some may be occluded if we draw them all at once.

  // For each octant which is under the cursor, recurse until the octant size 
  // is small enough - i.e. 1 unit cube.

  // Turn off depth testing. This is so we can draw and erase cubes without
  // having to clear the colour and depth buffers.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

//  ER()->DoCameraWork();

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  // Initial size of cubes. These are divided by 2 until a lower limit is reached.
  float cubesize = 64.0f; // TODO CONFIG some max

  m_selectedCubes.clear();

  PickSpaceDrawSections(0, cubesize, 0, cubesize);

  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);

  AmjuGL::PopAttrib();

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

  // Get rid of cubes which the mouse cursor points to but which don't make
  // sense. I.e. cubes under the floor, or not supported by anything.

  // For the remaining cubes, we must choose the one most likely to be
  // intended by the user.
  //  - Get rid of cubes which are not adjacent to the last position.
  /*
  CubeRemover cubeRemover(m_lastGoodCoord);
  m_selectedCubes.erase(std::remove_if(
    m_selectedCubes.begin(),
    m_selectedCubes.end(),
    cubeRemover), m_selectedCubes.end());
  */

#if defined(_DEBUG)
  int size = m_selectedCubes.size();
  std::cout << size << " unit cubes are selected:\n";
  for (int i = 0; i < size; i++)
  {
    std::cout << m_selectedCubes[i] << std::endl;
  }
#endif

  return false;
}


Colour PickHelper::ReadCursorColour() 
{
  return Colour();
}

int PickHelper::PickDrawGameObjects(Level* pLevel)
{
  int sel = -1;
  Frustum* pF = Frustum::Instance();

  // Iterate over the game objects which are in the current level/room.
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();

  // Get the game objects which are in the current room.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
#ifdef _DEBUG
  std::cout << "Objects: " << objs.size() << "\n";
#endif
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE); // DON'T write to z-buffer for this test.

  // Iterate through map of Game Objects. Check each one in the Game State
  // to see if it has been collected/killed/whatever.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (pGo->IsVisible())
    {
      VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
/*
      if (dynamic_cast<Water*>(pVgo))
      {
        continue;
      }
*/
      // Check state of object.
      State s = pVgo->GetState();

      if (s != OUT_OF_PLAY)
      {
        // Draw the object; check to see if the colour under the mouse cursor
        // has changed.
        Colour c1 = ReadCursorColour();
        AmjuGL::PushMatrix();
        pVgo->CullDraw(pF); 
        pVgo->DrawShadow();
        AmjuGL::PopMatrix();
        Colour c2 = ReadCursorColour();

        std::string s;
        s = "Active object: ";
        s += ToString(pVgo->GetId());
        s += "  ";
        s += pVgo->GetTypeName();
        //Engine::Instance()->GetTextWriter()->Print(0, 0, s.c_str());
#ifdef _DEBUG
        std::cout << s.c_str() << "\n";
#endif
/*
        glFlush();// TODO TEMP TEST
        SwapBuffers(wglGetCurrentDC());// TODO TEMP TEST
        Sleep(500); // TODO TEMP TEST
*/        
        if (c1 != c2)
        {
          sel = pVgo->GetId();
          break;
        }
      }
    }
  }

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE); // turn z-buffer writing back on
  return sel;
}

Orientation PickHelper::ObjectDragOrientation(PGameObject pGo, float dx, float dy, float dz)
{
  // Object pGo is being dragged. We want to find the new orientation.
  // We do this by drawing the object moved by gridsize in each 
  // of six directions. 
  Orientation oldPos = *(pGo->GetOrientation()); 
  Orientation newOr = oldPos;

  // Six possible differences to the position: forward and backward in
  // each of the three axes. 
  // The final result can be changed by a combination of these differences.
  Orientation diffs[6];
  diffs[0] = Orientation(dx,  0, 0, 0, 0, 0);
  diffs[1] = Orientation(-dx, 0, 0, 0, 0, 0);
  diffs[2] = Orientation(0, dy,  0, 0, 0, 0);
  diffs[3] = Orientation(0, -dy, 0, 0, 0, 0);
  diffs[4] = Orientation(0, 0, dz,  0, 0, 0);
  diffs[5] = Orientation(0, 0, -dz, 0, 0, 0);

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE); // DON'T write to z-buffer for this test.
  for (int i = 0; i < 6; i++)
  {
    // Draw object in new position. If colour under cursor changes,
    // add the position change to newOr.
    Colour c1 = ReadCursorColour();
    AmjuGL::PushMatrix();

    Orientation newPos = oldPos + diffs[i];
    pGo->SetOrientation(newPos);

    pGo->Draw();

    //glFlush();// TODO TEMP TEST
    //SwapBuffers(wglGetCurrentDC());// TODO TEMP TEST
    //Sleep(500); // TODO TEMP TEST

    Colour c2 = ReadCursorColour();
    if (c1 != c2)
    {
      newOr += diffs[i];
    }
    // Erase the object. NB background should be black for this to work.
    Engine::Instance()->PushColour(0, 0, 0, 1.0f);
    pGo->Draw();
    Engine::Instance()->PopColour();
    AmjuGL::PopMatrix();
  }
  pGo->SetOrientation(oldPos);

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE); // turn z-buffer writing back on

  return newOr;
}

}

