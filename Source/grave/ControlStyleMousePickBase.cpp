/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMousePickBase.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <iostream>
#include "ControlStyleMousePickBase.h"
#include "Common.h"
#include "Engine.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "Level.h"
#include "VisibleGameObject.h" // TODO remove dynamic_cast ?
#include "EngineRunning.h"
//#include "IdleBehaviour.h"
//#include "AiCollectBehaviour.h"
//#include "AiPlaceBehaviour.h"
//#include "AiRotateBehaviour.h"
//#include "AiJumpDownBehaviour.h"
//#include "AiRunToBehaviour.h"
#include "Takeable.h"
#include "PickHelper.h"
#include "Mouse.h"

namespace Amju
{
ControlStyleMousePickBase::ControlStyleMousePickBase()
{
  Reset();
/*
  m_behaviours["idle"] = new IdleBehaviour;
  m_behaviours["idle"]->SetNpc(Engine::Instance()->GetPlayer());

  m_behaviours["collect"] = new AiCollectBehaviour;
  m_behaviours["collect"]->SetNpc(Engine::Instance()->GetPlayer());

  m_behaviours["place"] = new AiPlaceBehaviour;
  m_behaviours["place"]->SetNpc(Engine::Instance()->GetPlayer());

  m_behaviours["rotate"] = new AiRotateBehaviour;
  m_behaviours["rotate"]->SetNpc(Engine::Instance()->GetPlayer());

  m_behaviours["jump"] = new AiJumpDownBehaviour;
  m_behaviours["jump"]->SetNpc(Engine::Instance()->GetPlayer());

  m_behaviours["runto"] = new AiRunToBehaviour;
  m_behaviours["runto"]->SetNpc(Engine::Instance()->GetPlayer());

  m_pBehaviour = 0;
*/
}

void ControlStyleMousePickBase::Reset()
{
  m_selectedObjectId = -1; // meaning no object selected

  m_newMousePos = false;
  m_doPicking = false;
  //m_pBehaviour = 0;
}

void ControlStyleMousePickBase::MousePos(int x, int y)
{
  static const int MOUSEMOVE = 1;

  if (fabs(x - Mouse::s_mousex) > MOUSEMOVE || 
      fabs(y - Mouse::s_mousey) > MOUSEMOVE)
  {
    m_newMousePos = true;
  }
  else
  {
    m_newMousePos = false;
  }
}

void ControlStyleMousePickBase::MouseButton(bool down, bool ctrl, bool shift)
{
  static bool lastValue = !down;
  if (lastValue == down)
  {
    return;
  }
  lastValue = down;

  // Only work when the mouse button is released without movement.
  if (down)
  {
    return;
  }
  if (m_newMousePos)
  {
    return;
  }

  m_doPicking = true;
}

void ControlStyleMousePickBase::Update()
{
  if (m_pBehaviour)
  {
    m_pBehaviour->Update();
  }
}

void ControlStyleMousePickBase::Draw()
{
  // TODO we need a behaviour controlling the player.

  if (m_doPicking)
  {
    PickDraw();
    // If a Piece is clicked:
    //  - if not already taken, go to the piece and take it
    //  - if already taken, drop it
    //  - if current action allows, rotate the piece
    if (m_selectedObjectId != -1)
    {
      // User has selected a piece. Decide what to do with it.
      PieceMover::Action a = Engine::Instance()->GetPlayer()->GetCurrentAction();

      if (a == PieceMover::ROTATE)
      {
          // Is the piece mouse-clicked on the carried piece ?
          // If so, rotate it.
        Engine::Instance()->GetPlayer()->ActionRotatePiece();
        /*
        PGameObject pDroppedPiece = Engine::Instance()->GetGameObject(m_selectedObjectId);
        if (pDroppedPiece->GetState() == UNKNOWN) // no longer TAKEN
        {
          // TODO!
          Takeable* pTakeable = dynamic_cast<Takeable*>(pDroppedPiece.GetPtr());
          Notifier::Instance()->NotifyPieceRotated(pTakeable, pAi);
        }
        */
      }
      else if (a == PieceMover::DROP)
      {
        // Is the piece mouse-clicked on the carried piece ?
        // If so, drop it at the current location.
        Engine::Instance()->GetPlayer()->ActionDropPiece();
        /*
        PGameObject pDroppedPiece = Engine::Instance()->GetGameObject(m_selectedObjectId);
        if (pDroppedPiece->GetState() == UNKNOWN) // no longer TAKEN
        {
          // TODO!
          Takeable* pTakeable = dynamic_cast<Takeable*>(pDroppedPiece.GetPtr());
          Notifier::Instance()->NotifyPieceDropped(pTakeable, pAi);
        }
        */
      }
      else if (a == PieceMover::TAKE)
      {
      }
      else
      {
        //m_pBehaviour = m_behaviours["collect"];
        // The block that the user has clicked on
        PGameObject pObj = Engine::Instance()->GetGameObject(m_selectedObjectId);
        //m_pBehaviour->SetTarget(pObj);
        //m_pBehaviour->OnActivated();
      }
    }
    else
    {
      // If not piece, get the coords of the ground clicked, and go there.
      // Find cubes which the mouse cursor is over.
      PickHelper ph;
      if (ph.PickSpaceDraw())
      {
        // Make a Target game object at the point the user clicked on.
        Target* pTarget = new Target;
        BoundingSphere bs;
        bs.SetCentre(m_lastGoodCoord);
        bs.SetRadius(1.0f); // TODO needed ?
        pTarget->SetSphere(bs);

        //m_behaviours["runto"]->SetTarget(pTarget);
        //m_pBehaviour = m_behaviours["runto"];
      }
    }

    m_doPicking = false;
  }
}

static const Colour colours[] = 
{
  Colour(1.0f,  0,     0,     1.0f),
  Colour(0,     1.0f,  0,     1.0f),
  Colour(1.0f,  1.0f,  0,     1.0f),
  Colour(0,        0,  1.0f,  1.0f),
  Colour(1.0f,     0,  1.0f,  1.0f),
  Colour(0,     1.0f,  1.0f,  1.0f),
  Colour(1.0f,  1.0f,  1.0f,  1.0f)
};

static const int MAX_COLOURS = sizeof(colours) / sizeof(Colour);

Colour ControlStyleMousePickBase::GetObjectPickColour(int objId)
{
  // Get false colour for an object
  static int c = 0;
  Colour col = colours[c];

  // store mapping from colour to object ID
  m_colourMap[col] = objId;

  c++; // hey!
  if (c == MAX_COLOURS)
  {
    c = 0;
  }
  return col;
}

bool ControlStyleMousePickBase::GetObjectPickId(const Colour& c, int* pResult)
{
  // Get object ID given false colour
  ColourMap::iterator it = m_colourMap.find(c);
  if (it != m_colourMap.end())
  {
    *pResult = it->second;
    return true;
  }

  return false;
}

bool ControlStyleMousePickBase::GetSelectedObject()
{
  // Return true if we successfully find the object under the cursor.
  Colour c = PickHelper::ReadCursorColour();

  if (GetObjectPickId(c, &m_selectedObjectId))
  {
    return true;
  }
  return false;
}

void ControlStyleMousePickBase::PickDrawGameObjects()
{
  m_selectedObjectId = 
    PickHelper::PickDrawGameObjects(ER()->GetLevel());
/*
  Frustum* pF = Frustum::Instance();

  // Draw objects in groups using this counter.
  int count = 0;

  // Iterate over the game objects which are in the current level/room.
  int levelId = ER()->GetLevel()->GetId();
  int roomId = ER()->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. Check each one in the Game State
  // to see if it has been collected/killed/whatever.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    if (pGo->IsVisible())
    {
      VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());

      // Check state of object.
      State s = pVgo->GetState();

      if (s != OUT_OF_PLAY)
      {
// ?        pVgo->SetLevel(m_pLevel);

        Colour c = GetObjectPickColour(gameObjId);
        Engine::Instance()->PushAbsColour(c.m_r, c.m_g, c.m_b, c.m_a);

        glPushMatrix();
        pVgo->CullDraw(pF);   
        glPopMatrix();

        Engine::Instance()->PopColour();

        count++;
        if (count == MAX_COLOURS) // max number of distinct false colours
        {
          if (GetSelectedObject())
          {
            return;
          }
          count = 0;
        }

      }
    }
  }
  if (GetSelectedObject())
  {
    return;
  }
  m_selectedObjectId = -1; // no object selected
*/
}

void ControlStyleMousePickBase::PickDrawScene()
{
  Engine::Instance()->PushColour(0, 0, 0, 1.0f);
  ER()->GetLevel()->Draw();
  Engine::Instance()->PopColour();
}

void ControlStyleMousePickBase::PickDraw()
{
  // Draw game objects in false colours. Then check the colour under the cursor
  // to find which object is selected (if any)

  ER()->DoCameraWork();

  Frustum::Instance()->Create();
  // Draw scene in false colour, so occluded objects can't be selected
  PickDrawScene();

  // Draw game objects in false colour 
  glPushAttrib(GL_LIGHTING);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  PickDrawGameObjects();

  glEnable(GL_TEXTURE_2D);
  glPopAttrib();

  // If an object has been selected, check its state to make sure it can be
  // dragged around.
  if (m_selectedObjectId != -1)
  {
    PGameObject pGo = Engine::Instance()->GetGameObject(m_selectedObjectId);
    Assert(pGo.GetPtr());
    if (pGo->GetState() == UNKNOWN &&
        pGo->GetOrientation() )
    {
      // Object is OK to select. Get the coords, which we then compare to
      // possible sleceted cubes when the piece is dragged. The only valid
      // cubes will be adjacent to this one.
      m_lastGoodCoord = pGo->GetOrientation()->GetVertex();
    }
    else
    {
      // Can't select
      // TODO wav
      m_selectedObjectId = -1; 
    }
  }
}


}

