/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TakeableCrate.cpp,v $
Revision 1.1.8.1  2005/06/05 00:26:11  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TakeableCrate.h"
#include "File.h"
#include "Engine.h"
#include "GameState.h"
#include "Player.h"
#include "BlockShadow.h"
#include "Level.h"
#include "SchAssert.h"

namespace Amju
{
const char* TakeableCrate::GetTypeName() const
{
  return "crate";
}

TakeableCrate::TakeableCrate() : Takeable(this)
{
}

std::string TakeableCrate::GetType() const
{
  // Get the type of crate - return the solid's name
  return m_crateType;
}

void TakeableCrate::Update()
{
}

bool TakeableCrate::Load(File* pf)
{
  // Sadly a total copy of Takeable::Load, but we need the solid name :-(
  // Load the Solid 
  std::string name;
  if (!pf->GetDataLine(&name))
  {
    pf->ReportError("Expected Solid name.");
    return false;
  }

  m_crateType = name;

  PSolidComponent pSolid = SolidComponent::LoadSolid(name); 
  if (!pSolid.GetPtr())
  {
    pf->ReportError("Failed to load Takeable Solid.");
    return false;
  }
  // Load the orientation/position 
  if (!pSolid->LoadOrientation(pf))
  {
    pf->ReportError("Failed to load Takeable orientation.");
    return false;
  }

  SetSolid(pSolid);

  // Save initial orientation for Reset().
  m_startOrientation = *(m_pSolid->GetOrientation());

  CreateShadow();
  m_pShadow->SetCaster(this);

  return true;
}

void TakeableCrate::Use()
{
  if (Engine::Instance()->GetPlayer()->GetActivePlatform())
  {
    // Player on platform: don't drop the Takeable.
    return;
  }

  // If the ground is flat, put the crate down on the ground.
  // Then put the player on top of the crate.
  const BoundingSphere* bs = Takeable::GetBoundingSphere();
  Assert(bs);

  // Get coords for shadow.
  const float x = bs->x();
  const float y = bs->y();
  const float z = bs->z();

  const HeightPoly* pHp = m_heightServer.GetHeightPoly(x, y, z);
  Assert(pHp);
  if (!pHp)
  {
    return;
  }
  // Is the ground flat ? TODO this doesn't seem to work
  if (pHp->b() < 0.2f)
  {
    // TODO wav ?
    return;
  }
  SetOrientation(*(Engine::Instance()->GetPlayer()->GetOrientation()));
  m_collideWithPlayer = true; // so player doesn't immediately take it again
  // TODO Takeables need to be solid. Then we can put the player on top.
  float playerY = Engine::Instance()->GetPlayer()->GetOrientation()->GetY();
  // Add the bounding box height, plus a bit more for luck
  // (actually so a shadow gets drawn.)
  float boxHeight = m_collisionVolume.GetBoundingBox().m_ymax - 
                    m_collisionVolume.GetBoundingBox().m_ymin;
  playerY += boxHeight + 0.2f; // TODO CONFIG
  //Engine::Instance()->GetPlayer()->GetOrientation()->SetY(playerY);

  // Reset state of this object, and make sure Game (Player) state is consistent.
  Engine* const e = Engine::Instance();
  Takeable::SetState(UNKNOWN);
}

void TakeableCrate::RecalcCollisionVol() 
{
  Matrix m;
  m.identity();
  m_pSolid->GetOrientation()->TransformMatrix(&m); 
  m_pSolid->StoreHeights(m);

  m_collisionVolume.Clear();
  BoundingSphere bs(VertexBase(0, 0, 0), 1000); // big B.S.
  m_pSolid->AddHeights(&m_collisionVolume, bs);
}

void TakeableCrate::CreateShadow()
{
  m_pShadow->SetCaster(this);
  return;

  //m_pShadow = new BlockShadow;
  //m_pShadow->SetCaster(this);
}

void TakeableCrate::DrawShadow() const
{
  if (SolidGameObject::GetState() != TAKEN)
  {
    SolidGameObject::DrawShadow(); // call base class version
  }
}

void TakeableCrate::Draw()
{
  if (Takeable::GetState() == TAKEN)
  {
    return;
  }
  
  SolidGameObject::Draw(); 
}

void TakeableCrate::DrawTaken()
{
  //Engine* e = Engine::Instance();
  //e->PushColour(1.0f, 1.0f, 1.0f, 0.5f);
  m_pSolid->Draw();
  //e->PopColour();
}


void TakeableCrate::RefreshHeightServer()
{
  // TODO make sure this isn't called every frame!!!!

  RecalcCollisionVol();

  VisibleGameObject::RefreshHeightServer();
/*
  // Not required unless Takeable Crates are stackable.
  // They were stackable in v.1, but not in v.2.

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  // Get the game objects which are in the same room as this character.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects. 
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;

    if (pGo.GetPtr() == this)
    {
      continue; 
    }

    TakeableCrate* pTakeable = dynamic_cast<TakeableCrate*>(pGo.GetPtr());
    if (pTakeable &&
        pTakeable->GetState() != OUT_OF_PLAY &&
        pTakeable->GetState() != TAKEN)  // TODO any other states which exclude it ?
    {
      // Increase takeable's b.sphere so we make sure to get
      // all objects we may cast a shadow on.
      BoundingSphere bs = *(pTakeable->GetBoundingSphere());
      bs.SetRadius(bs.GetRadius() + 1.0f); // TODO CONFIG or algo
      if (GetBoundingSphere()->Intersects(bs))
      {
        const HeightServer* pCollisionVol = pTakeable->GetCollisionVol();
        if (pCollisionVol)
        {
          m_heightServer.AddHeightServer(*pCollisionVol);
        }
      }
    }
  }
*/  
}

#if defined(SCENE_EDITOR)
static const std::string SOLID_NAME = "solid name";

GameObject::PropertyMap TakeableCrate::GetProperties() const
{
  PropertyMap pm;
  pm[SOLID_NAME] = GameObject::Property(m_pSolid->GetName());

  return pm;
}

void TakeableCrate::SetProperties(const GameObject::PropertyMap& pm)
{
  std::string solidName = const_cast<GameObject::PropertyMap&>(pm)[SOLID_NAME].GetString();
  PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
  if (pSolid.GetPtr())
  {
    m_pSolid = pSolid;
  }
  else
  {
    // ERROR
  }
}
#endif

void TakeableCrate::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
/*
  // TODO Put this code in a subclass if required.
  if (!pPlayer)
  {
    m_collideWithPlayer = false;
    return;
  }

  if (GetState() != UNKNOWN)
  {
    return;
  }

  if (GetState() == TAKEN)
  {
    return;
  } 

  if (m_collideWithPlayer)
  {
    // We have already processsed the collision
    return;
  }
  m_collideWithPlayer = true; // so we only do this once per player collision

  Engine* e = Engine::Instance();
  int takenId = -1; //e->GetGameState()->GetTakenObject();
  if (takenId != -1)
  { 
    // Set the previously Taken thing to the correct height at this location.
    PGameObject pTakenGo = e->GetGameObject(takenId);
    Assert(pTakenGo.GetPtr());
    Takeable* pTaken = dynamic_cast<Takeable*>(pTakenGo.GetPtr());
    Assert(pTaken);

    // We have already taken an object. Swap the currently taken object for this one.
    // Set the position of the currently taken object to the orientation of this one.
    Orientation o = *(GetOrientation()); // position of this takeable
    // Set height above ground for the previously taken thing.
    float groundY = 0;
    GetHeightServer()->GetHeight(o.GetX(), o.GetZ(), &groundY);
    o.SetY(groundY + pTaken->m_heightAboveGround);

    pTaken->SetOrientation(o); // Set previously taken thing to this position
    // NB This call also takes care of the bounding sphere and the collision vol!

    // Set the state to UNKNOWN, so it isn't taken any more.
    pTaken->SetState(UNKNOWN);

    // Set the player collision flag, so we don't immediately try to take it again.
    pTaken->m_collideWithPlayer = true;

    // Prevent Player from blundering into the thing we just dropped.
    // This can be a problem when things have different sizes.
    e->GetPlayer()->SetForwardVel(0);
    e->GetPlayer()->SetForwardAccel(0);
  }

  e->PlayWav(e->GetConfigValue("wav_taken"));

  SetState(TAKEN);
  //e->GetGameState()->SetTakenObject(m_id);
*/
}

}

