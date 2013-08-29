/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Stupid.cpp,v $
Revision 1.3.6.9  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.3.6.8  2006/07/26 21:25:17  jay
Use Pool/, not Pool/

Revision 1.3.6.7  2006/04/13 18:17:41  jay
Override RefreshHeightServer to ignore NonHeadAreas; update ball pos in
Update(), not Draw()

Revision 1.3.6.6  2005/10/03 08:09:33  jay
Use new Controller to stop Stupids falling down holes

Revision 1.3.6.5  2005/09/30 18:09:58  jay
Override Update() so we correctly control Stupids. The version in
CharacterGameObject is hacked for POOL, but if we change it we change
the behaviour of the player characters :-(

Revision 1.3.6.4  2005/09/30 12:57:06  jay
Set hidden object to UNKNOWN immediately on collision, don't send a timed
msg. Also minor tweaks

Revision 1.3.6.3  2005/09/25 20:18:39  Administrator
New SFX - distinguish from Foul; random speed, not working ?

Revision 1.3.6.2  2005/09/23 20:04:02  jay
Notify Rules if hit; random initial direction

Revision 1.3.6.1  2005/09/07 20:58:09  jay
POOL: Carry ball on head

Revision 1.3  2004/09/27 15:32:25  Administrator
Attempted to improve collision explosion, sfx

Revision 1.2  2004/09/23 09:07:34  Administrator
Fix CullDraw() so explosions are drawn in the right place.

Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Stupid.h"
#include "PoolStupidBehaviour.h"
#include "File.h"
#include "Engine.h"
#include "Level.h"
#include "PoolGameState.h"
#include "EngineRunning.h" // bah, hack to increase level time
#include "PoolStupidController.h"
#include "PoolMisc.h"
#include "CharacterManager.h"
#include "NonHeadArea.h"

namespace Amju
{
Stupid::Stupid()
{
  PoolStupidBehaviour* pSb = new PoolStupidBehaviour;
  SetBehaviour(pSb);
  m_appearanceDelay = 2.0f;
  m_hiddenId = -1;
  m_initialDir = 0;
  m_turnDir = 90.0f;

#if defined(SCENE_EDITOR)
  // Default mesh
  Character* pCharacter = 
    CharacterManager::Instance()->GetCharacter("bird");
  if (!pCharacter)
  {
    Engine::Instance()->ReportError("Can't set default mesh for stupid");
  }
  else
  {
    pCharacter->SetName("bird");
    AddMesh(pCharacter);
  }
#endif

  // POOL: Set Controller which ignores heights. This is so Stupids 
  // don't fall down the pockets!
  SetController(new PoolStupidController);
}

const char* Stupid::GetTypeName() const
{
  return "stupid";
}

void Stupid::SetAppearanceDelay(float delay)
{
  m_appearanceDelay = delay;
}

void Stupid::CullDraw(Frustum* pf)
{
  // Need to save matrix for explosion - bug #36. Same situation
  // with sheds.
  AmjuGL::PushMatrix();
  Npc::CullDraw(pf);
  AmjuGL::PopMatrix();
  m_explosion.Draw(); // don't frustum-cull based on Character position
}

void Stupid::Draw()
{
  if (GetState() == UNKNOWN)
  {
    AmjuGL::PushMatrix();
    Npc::Draw();
    AmjuGL::PopMatrix();
  }
}

#ifdef SCENE_EDITOR
bool Stupid::Save(File* pf)
{
  if (!Npc::Save(pf))
  {
    return false;
  }
  pf->WriteComment("// Initial direction");
  pf->WriteFloat(m_initialDir);
  pf->WriteComment("// Turn dir");
  pf->WriteFloat(m_turnDir);
  pf->WriteComment("// Appearance delay");
  pf->WriteFloat(m_appearanceDelay);
  pf->WriteComment("// Hidden object");
  pf->WriteInteger(m_hiddenId);

  return true;
}
#endif

bool Stupid::Load(File* pf)
{
  if (!Npc::Load(pf))
  {
    return false;
  }
  // Load initial direction
  if (!pf->GetFloat(&m_initialDir))
  {
    pf->ReportError("Expected initial direction.");
    return false;
  }

  // Get turn direction when forward path is blocked
  // (i.e. 90, -90 [or 180 ??])
  if (!pf->GetFloat(&m_turnDir))
  {
    pf->ReportError("Expected turn direction.");
    return false;
  }

  // Load delay before appearing in room
  if (!pf->GetFloat(&m_appearanceDelay))
  {
    pf->ReportError("Expected appearance delay.");
    return false;
  }

  // Hidden ID
  if (!pf->GetInteger(&m_hiddenId))
  {
    pf->ReportError("Expected hidden ID for stupid.");
    return false;
  }

  // Explosion effect - same code as Piece Mover
  std::string solidName = Engine::Instance()->GetConfigValue("stupid_particle");
  PSolidComponent pBb = SolidComponent::LoadSolid(solidName);
  if (!pBb.GetPtr())
  {
    pf->ReportError("Bad particle name.");
    return false;
  }
  // TODO CONFIG num particles, max vel/time 
  int numParticles = (int)atof(Engine::Instance()->GetConfigValue("stupid_num_particles").c_str());
  if (numParticles == 0)
  {
    return false;
  }
  m_explosion.Init(numParticles, pBb, 20 /* max speed */, 20 /* max time */);

  return true;
}

void Stupid::Update()
{
  if (GetState() == DEAD || GetState() == FADING)
  {
    SetForwardVel(0);
    SetYRotateVel(0);
  }
  else if (m_pCurrentBehaviour)
  {
    m_pCurrentBehaviour->SetNpc(this);
    m_pCurrentBehaviour->Update();
  }

  if (GetState() == FADING)
  {
    return;
  }

  // Copied from CharacterGameObject::Update()
  if (GetState() == FROZEN)
  {
    return;
  }

  FreeMovingGameObject::Update();

  // POOL: Draw the 'hidden' object above the character
  if (GetState() == UNKNOWN)
  {
    PPoolGameObject pGo = Engine::Instance()->GetGameObject(m_hiddenId);
    if (pGo.GetPtr())
    {
      Orientation o = *(GetOrientation());
      o.SetY(o.GetY() + 4.0f); // TODO TEMP TEST
      pGo->SetOrientation(o);      
    }
  }

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
  ControlUpdateThis();
/*
  // Recalculate is called by ControlUpdateThis so not reqd
  Recalculate();
*/

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
}

void Stupid::SetState(State newState)
{
  Npc::SetState(newState);
  if (newState == UNKNOWN)
  {
    //m_explosion.Reset(GetBoundingSphere()->GetCentre());

    // TODO TEMP TEST
    //float severity = atof(Engine::Instance()->GetConfigValue("quake_severity").c_str());
    //Engine::Instance()->GetEarthquake().SetActive(0.5f, severity); 
  }
}

void Stupid::OnRoomEntry()
{
  float r = (float)rand() / (float)RAND_MAX * 360.0f;
  SetInitialDirection(r);

  Npc::OnRoomEntry();

  // Reset time until we appear, and reset initial direction.

  Orientation o = *(GetOrientation());
  o.SetYRot(m_initialDir);
  SetOrientation(o);

  // This is no good for golf, we want the stupids to stay hidden!
/*
  // Set state to out of play, and send a message to appear after a delay.
  SetState(OUT_OF_PLAY);
  Engine::Instance()->SendGameMessage(new Message(
    "wake up stupid", m_appearanceDelay, GetId(), GetId(), UNKNOWN));
*/
  // Make sure the hidden object is hidden
  // POOL: make ball un-collidable until it falls into play
  PPoolGameObject pGo = Engine::Instance()->GetGameObject(m_hiddenId);
  if (pGo.GetPtr())
  {
    pGo->SetState(DEAD);
  }

  static const float v1 = Engine::Instance()->GetConfigFloat("pool-stupid-v1");
  static const float v2 = Engine::Instance()->GetConfigFloat("pool-stupid-v2");

  float v = (float)rand() / (float)RAND_MAX * v1 + v2;
  SetForwardVel(v); 
}

void Stupid::HandleObjectCollision(PoolGameObject* pObj)
{
  if (!pObj)
  {
    return;
  }

  // TODO replace this horrid dynamic_cast version with a double-dispatcher.

  // For golf game, stupids can be hit with golf ball.
  // When this happens the stupid is replaced by its HIDDEN OBJECT
  // if it has one.

  if (GetState() == DEAD ||
      GetState() == FADING ||
      GetState() == OUT_OF_PLAY)
  {
    return;
  }

  // POOL: Stupids carry balls. When the cue ball (or any ball ?) hits us,
  // we explode and drop the ball into play.

  // Yuck, check for ball type
  if (!IsCueBall(pObj)) //pObj->GetId() != GetBall()->GetId())
  {
    return;
  }

  // Tell Rules we have hit something
  GetRules(m_pLevel)->BallHitsObject((PoolBall*)pObj, this);

  m_explosion.Reset(GetOrientation()->GetVertex());

  Engine::Instance()->PlayWav("squeek.wav"); 
  Engine::Instance()->PlayWav(
    Engine::Instance()->GetConfigValue("golf_wav_9")); // explosion
  Engine::Instance()->PlayWav(
    Engine::Instance()->GetConfigValue("golf_wav_7")); // bonus points

  // TODO TEMP TEST
  float severity = Engine::Instance()->GetConfigFloat("quake_severity");
  Engine::Instance()->GetEarthquake().SetActive(0.5f, severity); 

  // Fade out, so the explosion will be visible.
//  SetState(UNKNOWN); // 
  SetState(FADING); //not OUT_OF_PLAY
  // Do go OUT OF PLAY after fading.
  // Send a message to put us completely out of play in a short time.
  Engine::Instance()->SendGameMessage(new Message("npc o.o.p.", 5,
    GetId(), GetId(), OUT_OF_PLAY));

  // Stop moving
  SetForwardVel(0);
  SetForwardAccel(0);

  // TODO increase score
  //AddToScore(1000); // TODO CONFIG

  // Un-hide the hidden object WITH A MESSAGE
  PPoolGameObject pGo = Engine::Instance()->GetGameObject(m_hiddenId);
  if (pGo.GetPtr())
  {
    // TODO TEMP TEST
    // Try setting the hidden object to UNKNOWN immediately
/*
    Engine::Instance()->SendGameMessage(new Message(
      "unhide stupid bonus", 2.0f, 
      GetId(), m_hiddenId, UNKNOWN));
*/
    pGo->SetState(UNKNOWN);
/*
    Orientation o = *(GetOrientation());
    o.SetY(o.GetY() + 4.0f); // TODO TEMP TEST
    pGo->SetOrientation(o);
*/
  }
}

void Stupid::SetTurnDirection(float d)
{
  m_turnDir = d;
}

float Stupid::GetTurnDirection() const
{
  return m_turnDir;
}

float Stupid::GetInitialDirection() const
{
  return m_initialDir;
}

void Stupid::SetInitialDirection(float d)
{
  m_initialDir = d;
}

#if defined(SCENE_EDITOR)
static const std::string MESH_NAME = "mesh name";
static const std::string HIDDEN_ID = "hidden object id";

Stupid::PropertyMap Stupid::GetProperties() const
{
  std::string meshname;
  if (m_meshes.empty())
  {
    // Default mesh
    Character* pCharacter = 
      CharacterManager::Instance()->GetCharacter("bird");
    if (!pCharacter)
    {
      Engine::Instance()->ReportError("Can't set default mesh for stupid");
    }
    else
    {
      pCharacter->SetName("bird");
      (const_cast<Stupid*>(this))->AddMesh(pCharacter);
      meshname = m_meshes[0]->GetName();
    }
  }
  else
  {
    meshname = m_meshes[0]->GetName();
  }

  PropertyMap pm;
  pm[MESH_NAME] = GameObject::Property(meshname);
  pm[HIDDEN_ID] = GameObject::Property(m_hiddenId);
  return pm;
}

void Stupid::SetProperties(const PropertyMap& pm)
{
  m_hiddenId = 
    const_cast<GameObject::PropertyMap&>(pm)[HIDDEN_ID].GetInt();
  
  std::string meshname = 
    const_cast<GameObject::PropertyMap&>(pm)[MESH_NAME].GetString();

  Character* pCharacter = 
    CharacterManager::Instance()->GetCharacter(meshname);
  if (!pCharacter)
  {
    Engine::Instance()->ReportError("Bad mesh name.");
  }
  else
  {
    pCharacter->SetName(meshname);
    m_meshes[0] = pCharacter;
  }
}
#endif

void Stupid::RefreshHeightServer()
{
  Assert(m_pLevel);

  RecalcCollisionVol();

  // First refresh 'static' heightserver
  m_pHeightPoly = 0; 
  VisibleGameObject::RefreshHeightServer();

/*
  // If the HS changes, reset pointer to last known floor.
  m_heightServerWithPlatforms = m_heightServer;

  int levelId = m_pLevel->GetId();
  int roomId = m_pLevel->GetRoomId();
  // Get the game objects which are in the same room as this character.
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);

  // Iterate through map of Game Objects.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    //GameObjectId gameObjId = it->first;
    PPoolGameObject pGo = it->second;

    if (pGo.GetPtr() == this)
    {
      continue; // don't collide with self
    }
    VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
    NonHeadArea* pNha = dynamic_cast<NonHeadArea*>(pVis);
    if (pVis && !pNha && pVis->GetState() != OUT_OF_PLAY)
    {
      const HeightServer* pCollisionVol = pVis->GetCollisionVol();
      if (pCollisionVol)
      {
        m_heightServerWithPlatforms.AddHeightServer(*pCollisionVol);
      }
    }
  }
*/
}

}

