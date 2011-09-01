/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineRunningBase.cpp,v $
Revision 1.3.6.4  2007/06/10 21:31:24  Administrator
POOL-specific: check for all not-in-play states

Revision 1.3.6.3  2007/03/16 19:08:34  jay
Fix ball shadows

Revision 1.3.6.2  2007/03/12 22:52:56  jay
Try to avoid so many calls to OnRoomEntry.. but this seems to lose ball
 shadows ?

Revision 1.3.6.1  2006/08/14 17:37:29  jay
Rename "Pool"

Revision 1.3  2004/09/17 13:46:07  jay
Minor changes in SetUpPlatforms()

Revision 1.2  2004/09/15 09:20:33  Administrator
Commented out code referring to "The Player" which is unused in golf game.
May need to be reinstated in future, but for multiplayer games there are
multiple characters, and no single "player".

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineRunningBase.h"
#include "Level.h"
#include "Frustum.h"
#include "Engine.h"
#include "Camera.h"
#include "LeafData.h"
#include "LeafRenderer.h"
#include "GameState.h"
#include "LevelServer.h"
#include "CharacterManager.h"
#include "Mgc/MgcSegment3.h"
#include <string>
#include "Platform.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
EngineRunningBase::EngineRunningBase()
{
  m_pLevel = 0;
  m_scenePolys = 0;
}

CharacterGameObject* EngineRunningBase::GetPlayer()
{
  return GetEngine()->GetPlayer();
}

Level* EngineRunningBase::GetLevel()
{
  return m_pLevel.GetPtr();
}

bool EngineRunningBase::IsUnderwater()
{
  return false;
}

void EngineRunningBase::Update()
{
  EngineState::Update();

  Assert(m_pLevel.GetPtr());
//  GetPlayer()->SetLevel(m_pLevel.GetPtr());

  GetEngine()->GetDayNightSky()->Update();

  // Update static scene
  m_pLevel->GetScene()->Update();

  // Update Game Objects before updating the Player. That way
  // the player responds to the position of things _this_ frame,
  // rather than their position in the previous frame.
  UpdateGameObjects();

//  GetPlayer()->Update();
  // Above call should probably do the following too!
//  GetPlayer()->RefreshHeightServer(); 
  // Update player height server with any game objects which are close 
  // enough to collide.
//  UpdateGameObjectHeightServer(GetPlayer());

  // See if player has collided with a game object.
//  PlayerCollisionTest();
}

void EngineRunningBase::DrawLensflare()
{
  // Lensflare: draw if the sparkling sun is on screen.
  if (!m_pLevel->IsCurrentRoomIndoors() && 
      GetEngine()->GetDayNightSky()->IsSparkleVisible())
  {
    // Make a bounding sphere around the sun coords.
    VertexBase sunPos = GetEngine()->GetDayNightSky()->GetSunPosition();
    BoundingSphere sunBs(sunPos, 50.0f); // sun's "radius"
    // Find out if the sun intersects the view frustum
    if (Frustum::Instance()->Contains(sunBs))
    {
      PCamera pCam = GetCamera();
      Assert(pCam.GetPtr());
      VertexBase eyePos(pCam->GetOrientation()->GetX(), 
                        pCam->GetOrientation()->GetY(), 
                        pCam->GetOrientation()->GetZ());

      // We have to draw the lens flare, unless the sun is obscured by part of 
      // the scene.
      // Test the line from camera to sun for obstructions.
      //Mgc::Segment3 seg;
      //seg.Origin() = Mgc::Vector3(eyePos.x, eyePos.y, eyePos.z);
      //seg.Direction() = Mgc::Vector3(
      //  sunPos.x - eyePos.x, 
      //  sunPos.y - eyePos.y, 
      //  sunPos.z - eyePos.z);

      // Do intersection test on the scenery for the current room.
      if (m_pLevel->GetScene()->LineIntersects(eyePos, sunPos, 1.0f /* some radius */ ))
      {
        return; // Sun is obscured.
      }

      // We should draw the lens flare. Get the Camera eye position and 
      // "look at" position (some point along the line we are pointing at).

      GetEngine()->PushColour(1.0f, 1.0f, 1.0f, 1.0f);

      //GetEngine()->GetLensflare()->Draw(
      //  GetEngine()->GetDayNightSky()->GetSunPosition(),
      //  eyePos,
      //  pCam->GetLookAtPos() );

      GetEngine()->PopColour();
    }
  }
}

void EngineRunningBase::DrawSkybox()
{
  if (!m_pLevel->IsCurrentRoomIndoors())
  {
    // Draw the sky. This changes with the game time-of-day.
    GetEngine()->GetDayNightSky()->DrawSkybox();

    // In case the renderer sorts by Textures, draw the list so far.
    LeafData::GetRenderer()->DrawList();
  }
}

void EngineRunningBase::DoCameraWork()
{
  Assert(GetCamera().GetPtr());

  // Commenting this out caused jerky camera movement in v.1.1.
  // TODO Work out why - and is the camera now being updated twice per frame ??
  GetCamera()->Update();

  // Skybox: this should be rotated as the camera changes its orientation,
  // but the distance should always be fixed.
  // NB The scene moves w.r.t. the skybox. If the skybox is not big enough,
  // it will 'clip' the scene far from the player!
  // (not if we turn off depth buffer writing)
  AmjuGL::PushMatrix();

  GetCamera()->DrawRotation();

  DrawSkybox();

  // Undo the skybox rotation.
  AmjuGL::PopMatrix();

  // "Draw" the camera, i.e. transform the view.
  GetCamera()->Draw();
}

void EngineRunningBase::DrawBackground()
{
  Assert(m_pLevel.GetPtr());

  // Lighting: either let the DayNightSky provide lighting, or for indoor rooms, let
  // the room do the lighting.
  // (Do Indoor lighting here for a static light source.)
  if (m_pLevel->IsCurrentRoomIndoors())
  {
    DoCameraWork();
    m_pLevel->DrawLight();
  }
  else
  {
    DoCameraWork();
    GetEngine()->GetDayNightSky()->DrawLight();
  }
}

void EngineRunningBase::Draw()
{
  DrawBackground();

  DrawForeground();
}

void EngineRunningBase::DrawForeground()
{
  // Create the view frustum. This is used in the drawing calls to cull objects
  // out of view.
  Frustum::Instance()->Create();

  // TODO make configurable.. this is just a test
  if (IsUnderwater())
  {
    GetEngine()->PushColour(0.5f, 0.5f, 1.0f, 1.0f);
  }

#if defined(HS_DEBUG)
  if (GetEngine()->GetDrawHeights())
  {
    // Draw the Heights/walls around the player.
    // The player (and all characters) own a HeightServer.
    const HeightServer* hs = GetPlayer()->GetHeightServer();
    hs->DrawHeights();
    hs->DrawWalls();
  }
#endif

  // Don't draw player character in first person mode!
  // TODO Maybe draw bits, e.g. a GUN in Multiplayer Deathmatch mode.

  // Pool: don't draw player
//  DrawPlayer();

  // Draw static elements of scene. Get number of polys drawn.
  m_scenePolys = m_pLevel->Draw();

  // Scene display lists may be stored in a sorted list. 
  // This call executes the sorted display lists
  // then empties the container. 
  LeafData::GetRenderer()->DrawList();


  // Player shadow: draw it TWICE, once on the static scene, once on
  // the game objects.
//  if (!GetEngine()->GetEngineState()->PlayerIsDead())
//  {
//    DrawCharacterShadow(GetPlayer());
//  }

  //DrawCharacterShadow(GetPlayer()); // needed ??

  //DrawGameObjectShadows();

  // Draw Game Objects. These are stateful, interactive things, including
  // moving Platforms, bonuses, etc.
  DrawGameObjects(false); // false => non-translucent

  DrawGameObjectShadows();

  DrawGameObjects(true); // true => translucent, or with transparent areas.

  // Execute GameObject display lists.
  LeafData::GetRenderer()->DrawList();

  // Player shadow: draw it AGAIN
  //DrawCharacterShadow(GetPlayer());

  // We don't like lensflare any more.
  //DrawLensflare();

  // If underwater, we pushed a colour onto the colour stack.
  if (IsUnderwater())
  {
    GetEngine()->PopColour();   
  }
}

void EngineRunningBase::UpdateGameObjects()
{
  Assert(m_pLevel.GetPtr());

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  GameObjectMap::iterator it;
  // First update platforms. Then update other objects.
  // This is to prevent objects on platforms from lagging behind the
  // platform they are on.
  for (it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (!dynamic_cast<Platform*>(pGo.GetPtr()))
    {
      continue;
    }

    // Don't bother with more costly dynamic_cast - we know it's visible.
    VisibleGameObject* pVgo = (VisibleGameObject*)pGo.GetPtr();

    // Check state of object.
    State s = pVgo->GetState();

    // POOL: some pool-specific states here
    if (s != OUT_OF_PLAY &&
        s != BALL_IN_POCKET &&
        s != BALL_OUT_OF_BOUNDS)
    {
      pVgo->Update(); // Game objects can do things even when out of view.

      // Static scenery heights:
      // If the object has moved, we refresh the floor/wall info for it.
      // Heights of other objects.
      UpdateGameObjectHeightServer(pVgo);
    }
  }

  for (it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (dynamic_cast<Platform*>(pGo.GetPtr()))
    {
      continue;
    }

    if (pGo->IsVisible())
    {
      // Don't bother with more costly dynamic_cast - we know it's visible.
      VisibleGameObject* pVgo = (VisibleGameObject*)pGo.GetPtr();

      // Check state of object.
      State s = pVgo->GetState();
      //Engine::Instance()->GetEngineState()->GetState(gameObjId, &s);
    // POOL: some pool-specific states here
      if (s != OUT_OF_PLAY &&
          s != BALL_IN_POCKET &&
          s != BALL_OUT_OF_BOUNDS)
      {
        // Static scenery heights:
        // If the object has moved, we refresh the floor/wall info for it.
        // Heights of other objects.
        UpdateGameObjectHeightServer(pVgo);

        // This prevents objects on platforms from lagging behind the
        // platform movement.
        pVgo->RefreshHeightServer();
        pVgo->Update(); // Game objects can do things even when out of view.
      }
    }
    else
    {
      // For non-visible game objects, we should still update.
      pGo->Update();
    }
  }

}

void EngineRunningBase::DrawGameObjectShadows()
{
  Assert(m_pLevel.GetPtr());

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. Check each one in the Game State
  // to see if it has been collected/killed/whatever.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (pGo->IsVisible())
    {
      VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());

      // Check state of object.
      State s = pVgo->GetState();
      //Engine::Instance()->GetEngineState()->GetState(gameObjId, &s);
      // POOL: some pool-specific states here
      if (s != OUT_OF_PLAY &&
          s != BALL_IN_POCKET &&
          s != BALL_OUT_OF_BOUNDS)
      {
        pVgo->DrawShadow();
      }
    }
  }
}

void EngineRunningBase::DrawGameObjects(bool translucent)
{
  Assert(m_pLevel.GetPtr());

  Frustum* pFrustum = Frustum::Instance();

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. Check each one in the Game State
  // to see if it has been collected/killed/whatever.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    if (pGo->IsVisible() && pGo->IsTranslucent() == translucent)
    {
      VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());

      // Check state of object.
      State s = pVgo->GetState();
      //Engine::Instance()->GetEngineState()->GetState(gameObjId, &s);
    // POOL: some pool-specific states here
      if (s != OUT_OF_PLAY &&
          s != BALL_IN_POCKET &&
          s != BALL_OUT_OF_BOUNDS)
      {
////////        pVgo->SetLevel(m_pLevel);

        // Pass the view frustum to each Game Object - it can decide whether
        // it can cull. This allows effects to be visible even though the
        // Game Object itself is out of view.
        //pVgo->DrawShadow();
        AmjuGL::PushMatrix();
        pVgo->CullDraw(pFrustum);   
        AmjuGL::PopMatrix();
        pVgo->DrawSphere(); // Draws b.sphere if turned on.

      }
    }
#if defined(_DEBUG)
    else
    {
      // For debugging, show the position of non-visible game objects
      pGo->DrawSphere();
    }
#endif

  }
}

void EngineRunningBase::DrawPlayer()
{
  AmjuGL::PushMatrix();

#if defined(_DEBUG)
  GetPlayer()->DrawSphere();
#endif

  GetPlayer()->Draw();
  AmjuGL::PopMatrix();
}

void EngineRunningBase::PlayerCollisionTest()
{
  if (!m_pLevel.GetPtr())
  {
    return;
  }

  GetPlayer()->SetActivePlatform(0);

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
 
    State s = pGo->GetState();

    if (s != OUT_OF_PLAY && pGo->HasLocation())
    {
      if (GetPlayer()->Intersects(*(pGo.GetPtr())))
      {
        //collision = true;
        pGo->HandlePlayerCollision(GetPlayer());

        //if (pGo->IsBad())
        //{
        //  badcollision = true;
        //}
      }   
      else
      {
        // Let the object know that it _doesn't_ intersect player.
        pGo->HandlePlayerCollision(0);
      }
    }
  }
}

/*
void EngineRunningBase::ObjectCollisionTest(GameObject* pObj)
{
  // If the given object (probably a Takeable)
  // intersects with another object (a Locator), perform any action
  // required by the object.
  if (!m_pLevel)
  {
    return;
  }

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);
    
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;

    State s = pGo->GetState();
    //Engine::Instance()->GetEngineState()->GetState(gameObjId, &s);
    if (s != OUT_OF_PLAY)
    {
      PGameObject pGo = it->second;
      if (pObj != pGo.GetPtr() && pObj->Intersects(*(pGo.GetPtr())))
      {
        pGo->HandleObjectCollision(pObj);
       
        // TODO reinstate this if flexible level objectives return.
        //if (m_pLevel->TestObjective())
        //{
        //  Engine::Instance()->LevelCompleted();
        //} 
        return;
      }
    }
  }
}
*/

void EngineRunningBase::DrawCharacterShadow(CharacterGameObject* pCharacter)
{
  pCharacter->DrawShadow();
}

void EngineRunningBase::UpdateGameObjectHeightServer(VisibleGameObject* pVgo)
{
  // TODO currently there is no generic way for Game Objecs to detect collisions with 
  // each other.

/*
  // Update the height server for a Visible Game Object. 
  // We add the Heights of all the other game objects. This lets all Game Objects
  // collide.
  // Iterate through Visible Game Objects.   
  int levelId = pVgo->GetLevel()->GetId();
  int roomId = pVgo->GetRoom();
  // Get the game objects which are in the same room as the game object.
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. Check each one in the Game State
  // to see if it has been collected/killed/whatever.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    GameObject* pGo = it->second;
    
    if (pGo->IsVisible() && pGo != pVgo) // obviously, skip over self.
    {
      State s = UNKNOWN;
      Engine::Instance()->GetEngineState()->GetState(gameObjId, &s);
      if (s != OUT_OF_PLAY)
      {
        // Check if the bounding spheres intersect - if so there is the
        // possibility of a collision.
        if (pVgo->GetBoundingSphere()->Intersects(*(pGo->GetBoundingSphere())))
        {
          // Add the Collision data from this Game Object to pVgo.
          // Now pVgo will be able to detect collisions with this object.
          pVgo->AddToHeightServer(((VisibleGameObject*)pGo)->GetCollisionInfo());
        }
      }
    }
  }
*/
}

void EngineRunningBase::OnNewRoom()
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();
  
  // These 2 lines cause a lot of calls to OnRoomEntry() but they seem to
  //  be needed to set up Shadows.
  int roomId = m_pLevel->GetRoomId();
  SetPlayerRoom(roomId); 

  SetUpPlatforms();
}

void EngineRunningBase::SetUpPlatforms()
{
  // For all Visible game objects, find out if they are starting off on a platform.
  // Get the game objects which are in the same room as the player.
  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();

  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);
  GameObjectMap::iterator it;

  for (it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;
    VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    if (pVis)
    {
      pVis->SetActivePlatform(0);
      // This fixes problem where platforms' collision volumes were not
      // generated.
      pVis->RecalcCollisionVol(); 
    }
  }

  for (it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    for (GameObjectMap::iterator jt = objs.begin(); jt != objs.end(); ++jt)
    {
      PGameObject pGo2 = jt->second;

      VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
      Platform* pPlat = dynamic_cast<Platform*>(pGo2.GetPtr());

      if (pVis && pPlat && pVis != pPlat && pVis->Intersects(*pPlat))
      {
        pVis->SetActivePlatform(pPlat);
      }
    }
  }
}

void EngineRunningBase::SetPlayerRoom(int roomId)
{
  // Reset orientation of moving Game Objects (NPCs) in the room the player 
  // just entered:
  
  // Get the game objects which are in the same room as the player.
  int levelId = m_pLevel->GetId();
  GameObjectMap& objs = GetEngine()->GetGameObjects(levelId, roomId);

  // This sets the level to point to the new room. Do this before telling
  // the game objects, so the new room is valid!
  m_pLevel->SetRoomId(roomId);

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo = it->second;

    pGo->SetLevel(m_pLevel.GetPtr());

    //GetEngine()->GetEngineState()->GetState(gameObjId, &s);
    //if (s != OUT_OF_PLAY)
    //{
      pGo->OnRoomEntry();
    //}
  }

  //GetCamera()->Reset();
}

}

