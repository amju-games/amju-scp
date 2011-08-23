/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PieceMover.cpp,v $
Revision 1.1.10.1  2005/06/05 00:26:06  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PieceMover.h"
#include "EngineRunning.h"
#include "Engine.h"
#include "Player.h"
#include "Character.h"
#include "Takeable.h"
#include "VisibilityGraphWithObjects.h"
#include "Level.h"
#include "CharacterControl.h"
#include "MoveMemorizer.h"

namespace Amju
{
extern void ReportError(const std::string& error);

static const float EXP_Y_OFFSET = 2.0f; // explosion height

// Called whenever a piece is moved. ALL game objects in the same room have to refresh
// their shadow.
void RefreshShadows(GameObject* pGo)
{
/*
  VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(pGo);
  pVgo->SetOrientation(*(pVgo->GetOrientation()));
  pVgo->RefreshHeightServer();
  pVgo->RefreshShadow();
  pVgo->DrawShadow();
*/
  
  int levelId = pGo->GetLevel()->GetId();
  int roomId = pGo->GetRoomId();

  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  GameObjectMap::iterator it;

  // TODO TEMP TEST
  for (it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo2 = it->second;
    VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(it->second.GetPtr());
    if (pVgo)
    {
      pVgo->SetOrientation(*(pVgo->GetOrientation()));
      pVgo->RefreshHeightServer();
      pVgo->RefreshShadow();
      pVgo->DrawShadow();
    }
  }
  
  
  for (it = objs.begin(); it != objs.end(); ++it)
  {
    PGameObject pGo2 = it->second;
    VisibleGameObject* pVgo = dynamic_cast<VisibleGameObject*>(it->second.GetPtr());
    if (pVgo)
    {
      pVgo->SetOrientation(*(pVgo->GetOrientation()));
      pVgo->RefreshHeightServer();
      pVgo->RefreshShadow();
      pVgo->DrawShadow();
    }
  }
  
}

PieceMover::PieceMover()
{
  m_canRotatePiece = false;
  m_isCarrying = false;
  m_cantDrop = false;
  m_pieceHeight = 0;
  m_pLastPiece = 0; // last piece placed
}

PieceMover::~PieceMover()
{
}

void PieceMover::NotifyPieceTaken(Takeable*, bool)
{
}

void PieceMover::NotifyPieceDropped(Takeable*, bool)
{
}

void PieceMover::NotifyPieceRotated(Takeable*, bool)
{
}

// Aghh - same code as in EngineStateTitle
bool PieceMover::LoadExplosion()
{
  std::string solidName = Engine::Instance()->GetConfigValue("piece_particle");

  // Load the explosion effect
  PSolidComponent pBb = SolidComponent::LoadSolid(solidName);
  if (!pBb.GetPtr())
  {
    ReportError("Bad particle name for piece.");
    return false;
  }

  // TODO CONFIG num particles, max vel/time 
  float numParticles = atof(Engine::Instance()->GetConfigValue("piece_num_particles").c_str());
  if (numParticles == 0)
  {
    return false;
  }

  for (int i = 0; i < NUM_EXPLOSIONS; i++)
  {
    m_explosion[i].Init(numParticles, pBb, 10, 10);
  }

  return true;
}

bool PieceMover::PieceIntersects(Takeable* pTakeable)
{
  Assert(m_pPMCharacter);
  return EngineRunningVisGraph::PieceIntersects(pTakeable, m_pPMCharacter->GetLevel());
}

bool PieceMover::PieceSupportsOthers(Takeable* pTakeable)
{
  Assert(m_pPMCharacter);
  return EngineRunningVisGraph::PieceSupportsOthers(pTakeable, m_pPMCharacter->GetLevel());
}

bool PieceMover::AllPieceCornersSupported(Takeable* pTakeable)
{
  Assert(m_pPMCharacter);
  return EngineRunningVisGraph::AllPieceCornersSupported(pTakeable, m_pPMCharacter->GetLevel());
}

void PieceMover::DrawTakenPiece()
{
  if (!m_isCarrying)
  {
    return;
  }

  Takeable* pTakeable = dynamic_cast<Takeable*>(m_pRotateablePiece.GetPtr());
  if (pTakeable)
  {
    Orientation o = *(GetPMCharacter()->GetOrientation());
    o.SetY(o.GetY() + 2.0f); // TODO CONFIG
    AmjuGL::PushMatrix();
    o.Draw();
    pTakeable->DrawTaken();
    AmjuGL::PopMatrix();
  }
}

void PieceMover::SetPMCharacter(CharacterGameObject* pCharacter)
{
  m_pPMCharacter = pCharacter;
}

CharacterGameObject* PieceMover::GetPMCharacter()
{
  return m_pPMCharacter;
}

bool PieceMover::IsCarryingPiece() const 
{
  return m_isCarrying;
}

void PieceMover::ResetPieceMover()
{
  m_canRotatePiece = false;
  m_isCarrying = false;
  m_pLastPiece = 0;
}

void PieceMover::ResetPieceCanRotate()
{
  m_canRotatePiece = false;
}

void PieceMover::CantDrop()
{
}

void PieceMover::CantTake()
{
}

void PieceMover::CantRotate()
{
}

bool PieceMover::ActionRotatePiece()
{
  // If we just dropped a takeable, rotate it to the next position.
  // (This flag gets reset when player moves.)
  if (m_canRotatePiece)
  {
    // Make sure the dropped piece doesn't intersect anything prior to rotating it - 
    // it shouldn't, but we must be sure or the following loop may
    // never terminate.
    Takeable* pTakeable = dynamic_cast<Takeable*>(m_pRotateablePiece.GetPtr());
    Assert(pTakeable);
    Assert(!PieceIntersects(pTakeable));
    if (PieceIntersects(pTakeable))
    {
      CantRotate();
      return false;
    }

    bool done = false;
    // Repeat until we don't intersect anything.
    while (!done)
    {
      // Turn it 90 degrees, check that this doesn't intersect anything.
      Orientation o = *(pTakeable->GetOrientation());
      o.SetYRot(o.GetYRot() + 90.0f); // TODO SNAP CONFIG
      m_pRotateablePiece->SetOrientation(o);

      done = !PieceIntersects(pTakeable); // done when nothing intersects.
    }

    RefreshShadows(m_pRotateablePiece.GetPtr());

    // Reset player height to the top of the piece's bounding box.
    // Otherwise we can rotate the piece into where the player is standing.
    GetPMCharacter()->SetHeight(
      pTakeable->GetCollisionVol()->GetBoundingBox().m_ymax);

//    EngineRunningVisGraph::RecalcVisGraph(m_pPMCharacter->GetLevel());
// Handled in AiNotifier

    m_pLastPiece = pTakeable;

    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_rotate_piece"));

    return true;
  }
  return false;
}

void PieceMover::UpdatePieceFootprint()
{
  if (!m_isCarrying)
  {
    return;
  }

  Takeable* pTakeable = dynamic_cast<Takeable*>(m_pRotateablePiece.GetPtr());
  if (!pTakeable)
  {
    return;
  }

  Orientation o = *(GetPMCharacter()->GetOrientation());
  o.SnapTo(1.0f, 90.0f); // TODO SNAP CONFIG

  // If the snapped orientation hasn't changed since last time, we can avoid
  // doing some stuff.

  // TODO Cache the last good pos and last snapped pos, or this gets done
  // every frame when the current player pos would cause an intersection.

  if (o == m_snappedPiecePos && !m_cantDrop)
  {
    // Position hasn't changed, and we CAN drop
    return;
  }

  m_snappedPiecePos = o;

  m_cantDrop = false;

  // Check if we dropped the piece, the character would go through a ceiling.
  // (The character gets placed on top of the piece.)
  const BoundingSphere& bsBefore = *(m_pPMCharacter->GetBoundingSphere());
  VertexBase v(bsBefore.GetCentre());
  v.y += m_pieceHeight;
  BoundingSphere bsAfter(v, bsBefore.GetRadius());
  CharacterControl cc;
  if (cc.IsIntersectingCeiling(m_pPMCharacter, bsBefore, bsAfter))
  {
    m_cantDrop = true;
    return; 
  }
  if (!IsOkToDropPiece())
  {
    m_cantDrop = true;
    return;
  }

  // Recalculate the height server for the carried piece.
  Orientation temp = *(pTakeable->GetOrientation());
  m_pRotateablePiece->SetOrientation(o);
  State tempState = pTakeable->GetState();
  pTakeable->SetState(UNKNOWN);
  if (pTakeable->GetCollisionVol()) // zero if taken, sigh
  {
    // Intersection test: we must make sure the footprint doesn't intersect
    // the static scene or any game objects.
    // If the current snapped position is no good, don't update it. So the
    // footprint will show the last good position until a new position is
    // good.
    // TODO try to find a good position nearest to ther player pos.
    // Otherwise the player can move, and the snapped position doesn't,
    // which is confusing.
    if (PieceIntersects(pTakeable))
    {
      m_cantDrop = true;
    }
    else
    {
      //m_snappedPiecePos = o;
      m_snappedPieceHs = *(pTakeable->GetCollisionVol());

      m_pieceFootprint.SetOrientation(o);
      m_pieceFootprint.SetPiece((VisibleGameObject*)m_pRotateablePiece.GetPtr());
      m_pieceFootprint.SetHeightServer(GetPMCharacter()->GetHeightServer());
      m_pieceFootprint.BuildList();
    }
  }
  m_pRotateablePiece->SetOrientation(temp); // yuck!
  pTakeable->SetState(tempState); // yuck!

  //m_cantDrop = false;

}

void PieceMover::DrawPieceFootprint()
{
  for (int i = 0; i < NUM_EXPLOSIONS; i++)
  {
    m_explosion[i].Draw();
  }


  if (!m_isCarrying)
  {
    return;
  }

  if (m_cantDrop)
  {
    return;
  }

  if (GetPMCharacter()->IsFalling())
  {
    return;
  }

  m_pieceFootprint.Draw();
}

bool PieceMover::IsOkToDropPiece()
{
  if (!m_isCarrying)
  {
    // No piece to drop
    return false;
  }

  // Don't use if player is falling
  if (GetPMCharacter()->IsFalling())
  {
    // Return true because this action is the appropriate one
    CantDrop();
    return false;
  }

  if (GetPMCharacter()->GetActivePlatform())
  {
    // Player on platform: don't drop the Takeable.
    CantDrop();
    return false;
  }

  Takeable* pTakeable = dynamic_cast<Takeable*>(m_pRotateablePiece.GetPtr());
  if (!pTakeable)
  {
    CantDrop();
    return false;
  }

  // Check that the floor under the player is flat. 
  const BoundingSphere* bs = GetPMCharacter()->GetBoundingSphere();
  Assert(bs);

  const float x = bs->x();
  const float y = bs->y();
  const float z = bs->z();

  const HeightServer* pPlayerHeightServer = GetPMCharacter()->GetHeightServer();
  const HeightPoly* pHp = pPlayerHeightServer->GetHeightPoly(x, y, z);
  Assert(pHp);

  // Is the ground flat ? 
  if (pHp->b() < 0.95f)
  {
    CantDrop();
    return false;
  }

  // Make sure the piece doesn't jut out over a slope.
  // (We can do this by checking that all the corners are supported at the
  // same height)
  if (!AllPieceCornersSupported(pTakeable))
  {
    CantDrop();
    return false;
  }

  // Make absolutely sure we won't intersect something if we drop the
  // piece here.
  State tempState = pTakeable->GetState();
  pTakeable->SetState(UNKNOWN);
  Orientation temp = *(pTakeable->GetOrientation());
  m_pRotateablePiece->SetOrientation(m_snappedPiecePos);
  Assert(pTakeable->GetCollisionVol());
  bool intersects = PieceIntersects(pTakeable);
  pTakeable->SetState(tempState); // yuck!
  m_pRotateablePiece->SetOrientation(temp); // yuck!
  if (intersects)
  {
    CantDrop();
    return false;
  }
  return true;
}

bool PieceMover::ActionDropPiece()
{
  if (!IsOkToDropPiece())
  {
    return false;
  }

  // Can drop piece
  Takeable* pTakeable = dynamic_cast<Takeable*>(m_pRotateablePiece.GetPtr());
  Assert(pTakeable);
  Orientation previousOr = *(pTakeable->GetOrientation());

  pTakeable->SetState(UNKNOWN);

  // Add the bounding box height to the character y-coord
  float playerY = GetPMCharacter()->GetOrientation()->GetY();
  playerY += m_pieceHeight + 0.2f; // TODO CONFIG
  //GetPMCharacter()->GetOrientation()->SetY(playerY);

  m_pRotateablePiece->SetOrientation(m_snappedPiecePos);
  Assert(!PieceIntersects(pTakeable));

  pTakeable->SetState(UNKNOWN);
  RefreshShadows(m_pRotateablePiece.GetPtr());

  m_isCarrying = false;

  // Now pressing the action button again will rotate the takeable we just
  // placed.
  m_canRotatePiece = true;

//  EngineRunningVisGraph::RecalcVisGraph(m_pPMCharacter->GetLevel());
// Handled in AiNotifier

  m_pLastPiece = pTakeable;

  // TODO TEMP TEST
  float severity = atof(Engine::Instance()->GetConfigValue("quake_severity").c_str());
  Engine::Instance()->GetEarthquake().SetActive(0.5f, severity); 

  const BoundingBox& bb = pTakeable->GetCollisionVol()->GetBoundingBox();
  m_explosion[0].Reset(VertexBase(bb.m_xmax, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmax));
  m_explosion[1].Reset(VertexBase(bb.m_xmin, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmin));
  m_explosion[2].Reset(VertexBase(bb.m_xmin, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmax));
  m_explosion[3].Reset(VertexBase(bb.m_xmax, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmin));

  // Store the move (i.e. piece ID and orientation).
  // This can be replayed later - or even undone ??
  MemMove move(pTakeable->GetId(), previousOr, m_snappedPiecePos);
  MoveMemorizer::Instance()->AddMove(move);

  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_drop_piece"));

  return true;
}

bool PieceMover::CharacterIsOnPiece(Takeable* pTakeable)
{
  // Get the Height Poly the player is standing on.
  float x = GetPMCharacter()->GetOrientation()->GetX();
  float y = GetPMCharacter()->GetOrientation()->GetY();
  float z = GetPMCharacter()->GetOrientation()->GetZ();

  const HeightPoly* pPlayerHeightPoly = GetPMCharacter()->
    GetHeightServer()->GetHeightPoly(x, y + 0.9f, y - 0.9f, z);
  
  if (!pPlayerHeightPoly)
  {
    return false;
  }

  // Get the height poly on the takeable, if any, corresponding to the player
  // coords.
  const HeightPoly* pTakeableHeightPoly = pTakeable->
    GetCollisionVol()->GetHeightPoly(x, y + 0.9f, y - 0.9f, z);

  if (!pTakeableHeightPoly)
  {
    return false;
  }

  if (*pTakeableHeightPoly == *pPlayerHeightPoly)
  {
    return true;
  }

  return false; 
}

bool PieceMover::ActionTakePiece(Takeable* pTakeable)
{
  Assert(!m_isCarrying);

  // Not allowed if player is falling
  if (GetPMCharacter()->IsFalling())
  {
    return false; // not sure - it probably doesn't matter as we are falling.
  }

  if (pTakeable && 
      pTakeable->GetState() == UNKNOWN &&
      !CharacterIsOnPiece(pTakeable) &&
      !PieceSupportsOthers(pTakeable) )
  {
    // Get the piece height
    m_pieceHeight = pTakeable->GetCollisionVol()->GetBoundingBox().m_ymax - 
                      pTakeable->GetCollisionVol()->GetBoundingBox().m_ymin;

    m_pRotateablePiece = pTakeable->GetGameObject();

    const BoundingBox& bb = pTakeable->GetCollisionVol()->GetBoundingBox();
    m_explosion[0].Reset(VertexBase(bb.m_xmax, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmax));
    m_explosion[1].Reset(VertexBase(bb.m_xmin, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmin));
    m_explosion[2].Reset(VertexBase(bb.m_xmin, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmax));
    m_explosion[3].Reset(VertexBase(bb.m_xmax, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmin));
    
    pTakeable->SetState(TAKEN);
    RefreshShadows(m_pRotateablePiece.GetPtr());
    m_isCarrying = true;

//    EngineRunningVisGraph::RecalcVisGraph(m_pPMCharacter->GetLevel());
// Handled in AiNotifier
    m_pLastPiece = pTakeable;

    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_take_piece"));

    return true;
  }
  return false;
}

Takeable* PieceMover::DecideTakeablePiece() 
{
  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pPMCharacter->GetLevel()->GetId();
  int roomId = m_pPMCharacter->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  CharacterGameObject* pPlayer = GetPMCharacter(); 

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    // Take the piece if it's a Takeable and intersects the player.
    Takeable* pTakeable = dynamic_cast<Takeable*>(pGo.GetPtr());
    if (pTakeable && 
        pTakeable->GetState() == UNKNOWN &&
        pGo->Intersects(*pPlayer) &&
        !CharacterIsOnPiece(pTakeable) &&
        !PieceSupportsOthers(pTakeable) )
    {
      return pTakeable;
    }
  }
  return 0;
}

bool PieceMover::ActionTakePiece()
{
  // Not allowed if player is falling
  if (GetPMCharacter()->IsFalling())
  {
    return false; // not sure - it probably doesn't matter as we are falling.
  }

  // If player is intersecting one or more takeables, we should pick up.
  // How to decide which one to take if we are intersecting more than one ?
  //  - don't take if underneath player
  //  - don't take if supporting something else
  //  - take the piece the player is closest to - or facing ?
  // The piece which will be taken should be highlighted already so the
  // player won't be surprised.
  // TODO Collision/interaction manager should mark takeables which can be
  // picked up.
  // For now, iterate over game objects in room. Pick up first takeable
  // which intersects player.

  // Iterate over the game objects which are in the current level/room.
  int levelId = m_pPMCharacter->GetLevel()->GetId();
  int roomId = m_pPMCharacter->GetLevel()->GetRoomId();

  // Get the game objects which are in the same room as the player.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  CharacterGameObject* pPlayer = GetPMCharacter(); 

  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    // Take the piece if it's a Takeable and intersects the player.
    Takeable* pTakeable = dynamic_cast<Takeable*>(pGo.GetPtr());
    if (pTakeable && 
        pTakeable->GetState() == UNKNOWN &&
        pGo->Intersects(*pPlayer) &&
        !CharacterIsOnPiece(pTakeable) &&
        !PieceSupportsOthers(pTakeable) )
    {
      // Get the piece height
      m_pieceHeight = pTakeable->GetCollisionVol()->GetBoundingBox().m_ymax - 
                      pTakeable->GetCollisionVol()->GetBoundingBox().m_ymin;

      m_pRotateablePiece = pTakeable->GetGameObject();

      const BoundingBox& bb = pTakeable->GetCollisionVol()->GetBoundingBox();
      m_explosion[0].Reset(VertexBase(bb.m_xmax, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmax));
      m_explosion[1].Reset(VertexBase(bb.m_xmin, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmin));
      m_explosion[2].Reset(VertexBase(bb.m_xmin, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmax));
      m_explosion[3].Reset(VertexBase(bb.m_xmax, bb.m_ymin + EXP_Y_OFFSET, bb.m_zmin));

      pTakeable->SetState(TAKEN);
      RefreshShadows(pGo.GetPtr());
      m_isCarrying = true;

//      EngineRunningVisGraph::RecalcVisGraph(m_pPMCharacter->GetLevel());
// Handled in AiNotifier
      m_pLastPiece = pTakeable;

      Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_take_piece"));

      return true;
    }
  }

  return false;
}

PieceMover::Action PieceMover::GetCurrentAction()
{
  // TODO why is this commented out ?
  //if (GetPMCharacter()->IsFalling())
  //{
  // return NONE; 
  //}

  if (!m_isCarrying && m_canRotatePiece)
  {
    return ROTATE;
  }

  if (m_isCarrying) 
  {
    if (m_cantDrop)
    {
      return NONE;
    }
    return DROP;
  }

  // TODO get the currently takeable piece, if any.
  // If no piece is takeable, return NONE.
  // Otherwise we should highlight the takeable piece (in EngineRunning)
  
  if (DecideTakeablePiece())
  {
    return TAKE;
  }
  return NONE;
}

Takeable* PieceMover::GetCurrentPiece()
{
  Takeable* pTakeable = dynamic_cast<Takeable*>(m_pRotateablePiece.GetPtr());
  Assert(pTakeable);
  return pTakeable;
}

CooperationGroup* PieceMover::GetCooperationGroup()
{
  return m_pCoopGroup.GetPtr();
}

void PieceMover::SetCooperationGroup(CooperationGroup* p)
{
  m_pCoopGroup = p;
}

Takeable* PieceMover::GetLastPiecePlaced()
{
  return m_pLastPiece;
}

#if defined(_DEBUG)
void PieceMover::SetLastPiecePlaced(Takeable* t)
{
  m_pLastPiece = t;
}
#endif

}

