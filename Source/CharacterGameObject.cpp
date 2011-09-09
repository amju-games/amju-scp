/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterGameObject.cpp,v $
Revision 1.1.10.5  2005/10/01 07:19:05  jay
Bigger default collision vol

Revision 1.1.10.4  2005/09/30 18:10:51  jay
Revert back to the hack before the previous fix. It affects the player
characters badly :-( Instead we override Stupid::Update().

Revision 1.1.10.3  2005/09/30 12:58:47  jay
In Update(), replace call to Recalculate() with call to ControlUpdateThis().
This was a stupid hack where it seemed like a good idea to save time as
neither of the characters moved. Now we have other NPCs of course we need
to properly control character movement. Sigh

Revision 1.1.10.2  2005/05/15 17:09:55  jay
Pool changes as characters don't move

Revision 1.1.10.1  2005/05/08 08:16:49  jay
Fix to GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <functional> // mem_fun
#include <math.h>
#include <iostream>
#include "CharacterGameObject.h"
#include "File.h"
#include "CharacterManager.h"
#include "Character.h"
#include "CharacterControl.h"
#include "Engine.h"
#include "Level.h"
#include "PoolGameState.h"
#include "PlayerCodes.h"
#include "PlayerMessage.h"
#include "Matrix.h"
#include "Player.h"
#include "RotateHelper.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
CharacterGameObject::CharacterGameObject()
{
  // These bounding sphere offsets should be overwritten in Load().
  // Also they will go totally when multiple bounding spheres are introduced!
  m_radius = 0.8f; 
  m_sphereXOffset = 0;
  m_sphereYOffset = 0.9f;
  m_sphereZOffset = 0;

  // Colour
  m_colourTimer = 0;
  m_colourR = m_colourG = m_colourB = 1.0f;


  m_leanAngle = 0;
  m_maxLeanAngle = 10.0f;
  m_leanVel = 50.0f;

  m_isNearlyFalling = false;
  m_nearlyFallingTime = 0;

  m_oldx = 0;
  m_oldy = 0;
  m_oldz = 0;

  m_pController = new CharacterControl;

#if defined(SCENE_EDITOR)
  // Default constructed Character G.O. must be saveable.
  // TODO Gahh... can't load a Solid here, leaf renderer isn't set at startup.
  // So for this Game Object type, we must hack it (worse than usual) in 
  // SaveCollisionVol().
#endif
}

CharacterGameObject::~CharacterGameObject()
{
}

bool CharacterGameObject::Load(File* pf)
{
  // In case there is a default mesh set
  m_meshes.clear();
  // Get number of meshes
  int numMeshes = 0;
  if (!pf->GetInteger(&numMeshes))
  {
    pf->ReportError("Expected number of meshes.");
    return false;
  }
  for (int i = 0; i < numMeshes; i++)
  {
    // Get mesh ID.
    string meshName;
    if (!pf->GetDataLine(&meshName))
    {
      pf->ReportError("Expected mesh name.");
      return false;
    }  
    Character* pCharacter = CharacterManager::Instance()->GetCharacter(meshName);
    if (!pCharacter)
    {
      pf->ReportError(string("Bad mesh name: ") + meshName);
      return false;
    }
    pCharacter->SetName(meshName);

    AddMesh(pCharacter);
  }
  return true;
}

void CharacterGameObject::AddMesh(Character* p)
{
  m_meshes.push_back(p);
}

void CharacterGameObject::ClearMeshes()
{
  m_meshes.clear();
}

void CharacterGameObject::ForEachMesh(void (Character::*pf)())
{
  for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
  {
    (((*it).GetPtr())->*pf)();
  }
}

void CharacterGameObject::SetNearlyFalling(bool b)
{
  m_isNearlyFalling = b;
  m_nearlyFallingTime = 0;
}

float CharacterGameObject::UpdateNearlyFallingTime(float deltaT)
{
  m_nearlyFallingTime += deltaT;
  return m_nearlyFallingTime;
}

bool CharacterGameObject::IsNearlyFalling() const
{
  return m_isNearlyFalling;
}

#ifdef SCENE_EDITOR
bool CharacterGameObject::Save(File* pf)
{
  pf->WriteComment("// no of meshes");
  pf->WriteInteger((int)m_meshes.size());
  for (unsigned int i = 0; i < m_meshes.size(); i++)
  {
    pf->Write(m_meshes[i]->GetName());
  }
  return true;
}

GameObject::PropertyMap CharacterGameObject::GetProperties() const
{
  // TODO
  PropertyMap pm;
  return pm;
}

void CharacterGameObject::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO
}
#endif

const char* CharacterGameObject::GetTypeName() const
{
  return "character";
}

void CharacterGameObject::SetSphereParams(
  float radius, float xoff, float yoff, float zoff)
{
  m_radius = radius; 
  m_sphereXOffset = xoff;
  m_sphereYOffset = yoff;
  m_sphereZOffset = zoff;
}

#if defined(SCENE_EDITOR)
bool CharacterGameObject::SaveBoundingSpheres(File* pf)
{
  pf->WriteComment("// No of bounding spheres");
  pf->WriteInteger(1); // TODO only 1 sphere currently supported
  pf->WriteComment("// Sphere radius, y offset, x/z offset");
  pf->WriteFloat(m_radius);
  pf->WriteFloat(m_sphereYOffset);
  pf->WriteFloat(m_sphereXOffset); // should be same as z-offset :-(
  return true;
}
#endif

bool CharacterGameObject::LoadBoundingSpheres(File* pf)
{
  int numBoundingSpheres = 0;
  if (!pf->GetInteger(&numBoundingSpheres))
  {
    pf->ReportError("Expected number of bounding spheres.");
    return false;
  }
  // Load each bounding sphere radius, height and forward/back offset.
  // (TODO Can't do side-to-side offsets..?) 
  for (int i = 0; i < numBoundingSpheres; i++)
  {
    float radius, yoff, xzoff;
    if (!pf->GetFloat(&radius) ||
        !pf->GetFloat(&yoff)   ||
        !pf->GetFloat(&xzoff))
    {
      pf->ReportError("Failed to load bounding sphere.");
      return false;
    }
    SetSphereParams(radius, xzoff, yoff, xzoff); // TODO only one sphere supported!
  }
  return true;
}

#if defined(SCENE_EDITOR)
bool CharacterGameObject::SaveCollisionVolume(File* pf)
{
  // Can't load Solid in ctor, so do it here, just for Editor.
  if (!m_pCollisionVol.GetPtr())
  {
    string collisionVolName = "coll_hex.leaf"; // TODO CONFIG PREFERENCES
    m_pCollisionVol = SolidComponent::LoadSolid(collisionVolName);
  }
  Assert(m_pCollisionVol.GetPtr());
  pf->WriteComment("// collision vol name");
  pf->Write(m_pCollisionVol->GetName());
  return true;
}
#endif

bool CharacterGameObject::LoadCollisionVolume(File* pf)
{
  // Get collision volume: This should be a simple leaf solid, e.g. a cube.
  // Other characters should bounce/slide off the collision volume, so stopping
  // two characters from occupying the same space.
  string collisionVolName;
  if (!pf->GetDataLine(&collisionVolName))
  {
    pf->ReportError("Expected collision volume name.");
    return false;
  }
  m_pCollisionVol = SolidComponent::LoadSolid(collisionVolName);
  if (!m_pCollisionVol.GetPtr())
  {
    pf->ReportError("Bad collision volume name.");
    return false;
  }
  
  Matrix m;
  m.SetIdentity();
  m_pCollisionVol->CreateBoundingSphere(m);

  return true;
}

static bool IsPlayerCode(int i) { return i > PLAYER_CODES_START; } 

void CharacterGameObject::ReceiveMessage(const Message& m)
{
  // If the message code is a PlayerCode, then perform whatever the player
  // action should be. Otherwise let the base class handle the message.
  const int c = m.GetCode();
  if (IsPlayerCode(c))
  {
    const PlayerMessage& pm = dynamic_cast<const PlayerMessage&>(m);
    // TODO static map of PlayerCodes to functions
    if (c == PLAYER_JUMP)
    {
      ExecuteJump(pm);
    }
    else if (c == PLAYER_FWD_VEL)
    {
      ExecuteForwardVel(pm);
    }
    else if (c == PLAYER_ROTATE_VEL)
    {
      ExecuteRotateVel(pm);
    }
  }
  else
  {
    PoolGameObject::ReceiveMessage(m);
  }
}

void CharacterGameObject::SetState(State newState)
{
  /*
  // Handle FROZEN state.
  if (m_pCharacter.GetPtr())
  {
    m_pCharacter->SetFrozen(newState == FROZEN);
  }
  */
  FreeMovingGameObject::SetState(newState);
}

void CharacterGameObject::SetColourTimer(float time, float r, float g, float b)
{
  m_colourTimer = time;
  m_colourR = r;
  m_colourG = g;
  m_colourB = b;
}

void CharacterGameObject::SetActionDead()
{
  ForEachMesh(&Character::SetActionDead);
}

void CharacterGameObject::SetActionIdle()
{
  ForEachMesh(&Character::SetActionIdle);
}

void CharacterGameObject::SetActionPain()
{
  // Set up red flashing.
  float painTime = atof(Engine::Instance()->GetConfigValue("character_pain_time").c_str());
  SetColourTimer(painTime, 1.0f, 0, 0); // time, r, g, b.

  ForEachMesh(&Character::SetActionPain);
}

void CharacterGameObject::SetActionWalk()
{
  ForEachMesh(&Character::SetActionWalk);
}

void CharacterGameObject::SetActionRun()
{
  ForEachMesh(&Character::SetActionRun);
}

void CharacterGameObject::SetActionJump()
{
  ForEachMesh(&Character::SetActionJump);
}

void CharacterGameObject::SetActionFly()
{
  ForEachMesh(&Character::SetActionFly);
}

void CharacterGameObject::SetActionFall()
{
  ForEachMesh(&Character::SetActionFall);
}

void CharacterGameObject::SetActionShuffle()
{
  ForEachMesh(&Character::SetActionShuffle);
}

void CharacterGameObject::OnNoMove()
{
  SetActionIdle();
}

void CharacterGameObject::OnArcMove()
{
  SetActionShuffle();
}

void CharacterGameObject::CalcLeanAngle()
{
  const float dt = Engine::Instance()->GetDeltaTime();

  // If character is turning at speed, lean into the turn.
  // Rotate around the z-axis to lean in the direction we are turning.
  if (//m_forwardVel > 5.0f && 
      m_yRotateVel > 5.0f) // TODO CONFIG!!!!!
  {
    if (m_leanAngle > -m_maxLeanAngle)
    {
      m_leanAngle -= m_leanVel * dt;
    }
  }
  else if (//m_forwardVel > 5.0f && 
    m_yRotateVel < -5.0f) // TODO CONFIG!!!!!
  {
    if (m_leanAngle < m_maxLeanAngle)
    {
      m_leanAngle += m_leanVel * dt;
    }
  }
  else if (m_leanAngle > 0.1f)
  {
    m_leanAngle -= m_leanVel * dt;
    if (m_leanAngle <= 0.1f)  // TODO attempt to kill "wobble"
    {
      m_leanAngle = 0;
    }
  }
  else if (m_leanAngle < -0.1f)
  {
    m_leanAngle += m_leanVel * dt;
    if (m_leanAngle >= -0.1f)
    {
      m_leanAngle = 0;
    }
  }    
}

void CharacterGameObject::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  //AmjuGL::PushMatrix();
  // Position the Model.
  //m_orientation.Draw();
  GetOrientation()->Draw();

  Engine::Instance()->PushColour(m_colourR, m_colourG, m_colourB, 1.0f);

  AmjuGL::PushMatrix();
  AmjuGL::RotateZ(m_leanAngle); //, 0, 0, 1.0f);
  // Draw each mesh in the model.
  ForEachMesh(&Character::Draw);
  AmjuGL::PopMatrix();

  Engine::Instance()->PopColour();
  
  AmjuGL::PopAttrib();

#if defined(_DEBUG)
  Engine::Instance()->PushColour(0, 0, 0, 1.0f);
  const_cast<BoundingSphere*>(GetBoundingSphere())->Draw();
  Engine::Instance()->PopColour();
#endif
}

const BoundingSphere* CharacterGameObject::GetBoundingSphere() const
{
  m_bsphere.SetRadius(m_radius); 

  Vec3f v(GetPosition().x + m_sphereXOffset * m_vectorX, 
               GetPosition().y + m_sphereYOffset,  
               GetPosition().z + m_sphereZOffset * m_vectorZ);
  m_bsphere.SetCentre(v);

  return &m_bsphere;
}

void CharacterGameObject::CreateBoundingSphere(const Matrix& cm)
{
}

void CharacterGameObject::DrawShadow() const
{
  const State s = GetState();
  if (s == DEAD || s == FADING || s == OUT_OF_PLAY)
  {
    return;
  }

  const float x = GetPosition().x;
  const float y = GetPosition().y;
  const float z = GetPosition().z;

  m_pShadow->Draw(x, y, z, GetShadowSize(), m_heightServerWithPlatforms);
}

const HeightServer* CharacterGameObject::GetCollisionVol() const
{
  if (GetState() == DEAD || GetState() == FADING)
  {
    return 0;
  }

  return &m_collisionVolume;
}

void CharacterGameObject::RecalcCollisionVol()
{
  if (!m_pCollisionVol.GetPtr())
  {
    return;
  }

  // Set orientation of collision solid so it encloses this character.
  m_pCollisionVol->SetOrientation(m_orientation);

  Matrix m;
  m.SetIdentity();
  // Get a matrix from the solid's orientation
  m_pCollisionVol->GetOrientation()->TransformMatrix(&m);
  // Use the matrix to make absolute coord height/wall info
  m_pCollisionVol->StoreHeights(m);
  BoundingSphere bs(Vec3f(0, 0, 0), 1000); // big B.S.
  m_collisionVolume.Clear();
  // Add all the polys (that fit inside bs, which should be all of them)
  // which are part of our Solid to our heightserver.
  m_pCollisionVol->AddHeights(&m_collisionVolume, bs);

}

void CharacterGameObject::Jump()
{
  // For now, don't use messages, to reduce the number of messages flying around.
  PlayerMessage m("player jump", 0, GetId(), GetId(), PLAYER_JUMP, 0);
  ExecuteJump(m);

/*
  // Send a game message signifying this action.
  PMessage m = new PlayerMessage("player jump", 0, GetId(), GetId(), PLAYER_JUMP, 0);
  Engine::Instance()->SendGameMessage(m);
*/
}

void CharacterGameObject::ExecuteJump(const PlayerMessage& m)
{
//  Assert(m_pCharacter.GetPtr());

  if (IsFalling())
  {
    return;
  }

  float jumpHeight = 1.0f; // TODO CONFIG

  // Don't jump if we would go through the ceiling.
  const BoundingSphere& bsBefore = *GetBoundingSphere();
  Vec3f v(bsBefore.GetCentre());
  v.y += jumpHeight;
  BoundingSphere bsAfter(v, bsBefore.GetRadius());
  
  CharacterControl cc;
  if (cc.IsIntersectingCeiling(this, bsBefore, bsAfter))
  {
    // Maybe a small jump
    jumpHeight = 0.5f;
    v = bsBefore.GetCentre();
    v.y += jumpHeight;
    bsAfter = BoundingSphere(v, bsBefore.GetRadius());
  
    if (cc.IsIntersectingCeiling(this, bsBefore, bsAfter))
    {
      // TODO "Can't jump" wav ?
      return;
    }
  }
  // end of ceiling test

  SetFalling(true); // "nearly falling" fix
  SetNearlyFalling(false);        // "nearly falling" fix

  SetHeight(GetHeight() + jumpHeight); 
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_jump"));
  SetActionJump();
}

void CharacterGameObject::Recalculate()
{
  FreeMovingGameObject::Recalculate();

  Assert(!m_meshes.empty());

  // Work out next action. Run/walk if we are moving forward, shuffle if
  // we are turning round, else stand. But don't
  // change if we are doing a non-repeating animation, e.g. a Jump, because
  // we want that animation to finish first.

  // Get walk/run changeover speed.
  static float walkLimit = atof(Engine::Instance()->GetConfigValue("walk_limit").c_str());

  if (m_forwardVel > SMALLEST && m_forwardVel <= walkLimit)
  {
    if (m_meshes[0]->CurrentActionRepeats()) 
    {
      SetActionWalk();
    }
  }
  else if (m_forwardVel < -SMALLEST)
  {
    // Walking backwards
    if (m_meshes[0]->CurrentActionRepeats()) 
    {
      SetActionWalk();
    }
  }
  else if (m_forwardVel > SMALLEST && m_forwardVel > walkLimit)
  {
    if (m_meshes[0]->CurrentActionRepeats())
    {
      SetActionRun();
    }
  }
  else //if (m_forwardVel <= SMALLEST)
  {
    // If we are rotating, set the current action to Shuffle.
    if (fabs(m_yRotateVel) > SMALLEST)
    {
      //SetActionShuffle();
      OnArcMove();
    }
    // Not shuffling.. stand idle unless we should finish the current animation.
    else if (m_meshes[0]->CurrentActionRepeats())
    {
      //SetActionIdle();
      OnNoMove();
    }
  }

  // Update the animation
  ForEachMesh(&Character::Recalculate);

  // Update the position.
  FreeMovingGameObject::Recalculate();

  float deltaTime = Engine::Instance()->GetDeltaTime();

  // Reduce the colour timer by deltaTime. When the timer reaches zero, we go 
  // back to white.
  // This can be used for Injury - character goes red, or power ups, etc.
  m_colourTimer -= deltaTime;
  if (m_colourTimer <= 0)
  {
    SetColourTimer(0, 1, 1, 1); // t, r, g, b - Reset colour to white
  }

  // Characters lean into turns
  CalcLeanAngle();

  for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
  {
    (*it)->SetFalling(IsFalling());
  }
}  

void CharacterGameObject::FellFromHeight(float heightFallenFrom)
{
/*
  static const float HARMFUL_HEIGHT = 
    Engine::Instance()->GetConfigFloat("harmful_height");

  if (heightFallenFrom > HARMFUL_HEIGHT)
  {
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_hitsground"));
    SetActionPain();
  }
*/
}

void CharacterGameObject::SetOrientation(const Orientation& o)
{ 
#ifdef CHARACTER_DEBUG
std::cout << "PLAYER ORIENTATION set for object " << GetId() << "\n";
#endif

  FreeMovingGameObject::SetOrientation(o); 
  m_orientation = o;
} 

const Orientation* CharacterGameObject::GetOrientation()
{
  // TODO This assert FAILS at low FPS.
//  Assert(m_orientation == *(FreeMovingGameObject::GetOrientation()));
  return FreeMovingGameObject::GetOrientation();
}

void CharacterGameObject::Update()
{
  if (GetState() == FROZEN)
  {
    return;
  }

  FreeMovingGameObject::Update();

  // Compare recalculated position with old position, so we can spot when an
  // NPC is stupidly blocked. (Ideally we shouldn't have to do this, right..)
  float oldx = GetPosition().x;
  float oldz = GetPosition().z;

  // Recalculate position now that velocity etc has been set.
  // This handles wall collision and height.
  //CharacterControl::UpdateCharacter(this, CharacterControl::s_stepHeight); 

  // TODO TEMP TEST
  // POOL
  // We don't need to update the character positions, but we do need to 
  // update the animations.
  // Oh Dear, it turns out we do need to update, because we also have
  // NPCs.
  // Revert to just Recalculate so Amju and Marin don't go through the 
  // floor - override in Stupid::Update
/*
  ControlUpdateThis(); 
*/
  Recalculate();

  float newx = GetPosition().x;
  float newz = GetPosition().z;

  // Work out how far you would expect to move given the current velocity.
  // Then compare this with the actual distance moved.
  // This is so we can detect when an NPC is stupidly trying to run into
  // a wall, etc. It's no good to simply compare the distance moved to 
  // an absolute value though.
  float dt = Engine::Instance()->GetDeltaTime();
  float theoreticalDistance = GetForwardVel() * dt;
  float dx = newx - oldx;
  float dz = newz - oldz;
  float actualDistance = sqrt(dx * dx + dz * dz);

  if (theoreticalDistance > actualDistance * 2.0f) // TODO CONFIG
  {
    SetBlocked(true);
  }
  else
  {
    SetBlocked(false);
  }

// why need this here ?  
//  RefreshHeightServer();

}

}

