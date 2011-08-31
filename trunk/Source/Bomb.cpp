/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bomb.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Bomb.h"
#include "Engine.h"
#include "GameState.h"
#include "BlastWaveEffect.h"
#include "TextureServer.h"
#include "Level.h"
#include "ExpandingBillboard.h"
#include "Player.h"

using namespace std;
namespace Amju
{
Bomb::Bomb() 
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

const char* Bomb::GetTypeName() const
{
  return "bomb";
}

bool Bomb::Load(File* pf)
{
  if (!SolidGameObject::Load(pf))
  {
    return false;
  }
  // TODO Load Effects. Hardcoded for now..

  // Blast wave
  BlastWaveEffect* pBlastWave = new BlastWaveEffect;
  string strTex = Engine::Instance()->GetConfigValue("player_bw_texture");
  string strAlpha = Engine::Instance()->GetConfigValue("player_bw_alpha");
  Texture* pBwTex = TextureServer::Instance()->Get(strTex, strAlpha);
  pBwTex->CreateBinding(Texture::REGULAR);
  pBlastWave->Init(pBwTex, 40.0f, 3.0f, 0, 0);
  m_fx.push_back(pBlastWave); 

  m_blastRadius = 20.0f;

  m_pBillboard = new ExpandingBillboard;
  m_pBillboard->Load(Engine::Instance()->GetConfigValue("bomb_exp_bb"));
  ((ExpandingBillboard*)m_pBillboard.GetPtr())->SetExpansionVel(100.0f);

  m_detonateTime = 0;
  m_detonated = false;

  return true;
}

void Bomb::CullDraw(Frustum* pFrustum)
{
  if (GetState() == DETONATING)
  {
    Draw(); // Don't cull
  }
  else if (GetState() == TAKEN)
  {
    // Drawn above player elsewhere(?)
  }
  else
  {
    SolidGameObject::CullDraw(pFrustum);
  }
}

void Bomb::Draw()
{
  if (GetState() == DETONATING)
  {
    if (!m_detonated)
    {
      return;
    }

    if (m_detonateTime < 6.0f) // TODO CONFIG
    {
      // We draw explosion effects in here, so we don't get culled by the 
      // view frustum. 
      AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_READ);
      AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

      for (EffectVector::iterator it = m_fx.begin(); it != m_fx.end(); ++it)
      {
        (*it)->Draw();
      }

      m_pBillboard->Draw();

      AmjuGL::PopAttrib();

      m_detonateTime += Engine::Instance()->GetDeltaTime();
    }
    else
    {
      m_detonated = false;
    }
  }
  else if (GetState() == TAKEN)
  {
    // Drawn above player elsewhere
  }
  else
  {
    Draw();
  }
}

void Bomb::SetState(State newState)
{
  if (newState == DETONATE)
  {
    Detonate();
  }
  else if (newState == TAKEN)
  {
    // Old code: Player no longer holds chicks
    //((ChickHolder*)Engine::Instance()->GetPlayer())->SetBaseHeight(2.5f);
  }
  else
  {
    m_detonateTime = 0;
    m_detonated = false;
  }
  // Call base class function
  VisibleGameObject::SetState(newState);
}

void Bomb::Use()
{
  Detonate();
}

void Bomb::DrawShadow() const
{
  // Don't draw if detonated, taken, etc.
  if (SolidGameObject::GetState() == UNKNOWN)
  {
    SolidGameObject::DrawShadow(); // call base class version
  }
}

void Bomb::Detonate()
{
/*
  if (GetState() == DETONATING)
  {
    return;
  }

  SetState(DETONATING);

  // Once bomb has been used, reset the state to OUT_OF_PLAY.
  Engine::Instance()->SendGameMessage(
    new Message("bomb reset", 6.0f, GetId(), GetId(), UNKNOWN));

  // Drop chicks down to space where the bomb was.
  //((ChickHolder*)Engine::Instance()->GetPlayer())->SetBaseHeight(1.5f);

  m_detonateTime = 0; // time since detonation
  m_detonated = true;

  //VertexBase pos = Engine::Instance()->GetPlayer()->GetPosition();
  // Player y-coord is at ground level, so we must raise it. 
  //pos.y += 0.7f; // TODO CONFIG

  // Reset all effects
  for (EffectVector::iterator e_it = m_fx.begin(); e_it != m_fx.end(); ++e_it)
  {
    (*e_it)->Reset(pos);
  }

  m_pBillboard->SetSize(0);
  //m_pBillboard->SetOrientation(*(Engine::Instance()->GetPlayer()->GetOrientation()));

  // Play wav
  std::string explWav = Engine::Instance()->GetConfigValue("wav_explosion");
  Engine::Instance()->PlayWav(explWav);
 
  // Activate Earthquake effect.
  i//Engine::Instance()->GetEarthquake().SetActive(3.0f); // TODO CONFIG

  // Check all Game Objects in this room. If they are Crocs, and within
  // the blast radius, they get wasted!
  int levelId = GetLevel()->GetId();
  int roomId = GetRoomId();

  int bonus = 1;
  // Get the game objects which are in the same room..
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // ..and iterate through them.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObjectId gameObjId = it->first;
    PGameObject pGo = it->second;
    Croc* pCroc = dynamic_cast<Croc*>(pGo.GetPtr()); 
    if (!pCroc)
    {
      continue;
    }
    // It's a Croc. Get distance
    const float dx = pCroc->GetBoundingSphere()->GetCentre().x - pos.x;
    const float dz = pCroc->GetBoundingSphere()->GetCentre().z - pos.z;
    const float dsq = dx * dx + dz * dz;
    const float d = sqrt(dsq);
    if (d < m_blastRadius)
    {
      // This Croc gets creamed
      pCroc->Dead();
      //Engine::Instance()->GetGameState()->AddScore(100 * bonus);
      bonus *= 2;
    }
    else
    {
      // Jump in surprise
      pCroc->Jump();
    }
  }
*/
}

#if defined(SCENE_EDITOR)
bool Bomb::Save(File* pf)
{ 
  // TODO ?
  return SolidGameObject::Save(pf);
}

GameObject::PropertyMap Bomb::GetProperties() const
{
  return PropertyMap();
}

void Bomb::SetProperties(const GameObject::PropertyMap& pm)
{
}
#endif
}

