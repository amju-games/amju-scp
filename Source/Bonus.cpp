/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bonus.cpp,v $
Revision 1.1.8.2  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.1.8.1  2006/07/26 21:25:16  jay
Use Pool/, not Pool/

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Bonus.h"
#include "SolidComponent.h"
#include "File.h"
#include "Common.h"
#include "Engine.h"
#include "PoolGameState.h"
#include "Frustum.h"
#include "EngineRunning.h"
#include "PoolMisc.h"
#include "EngineStatePoolBase.h"
#include "LevelServer.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
Bonus::Bonus()
{
  m_pExplosion = new ExplosionEffect;
  // Default explosion. TODO this should be explicitly set.
  m_pExplosion->Load("melonexplode.txt"); // TODO CONFIG

  m_bonusPoints = 0;

#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  std::string bonusSolid = "heart-1.move"; // TODO CONFIG
  m_pSolid = SolidComponent::LoadSolid(bonusSolid);
#endif
}

const char* Bonus::GetTypeName() const
{
  return "bonus";
}

#if defined(SCENE_EDITOR)
bool Bonus::Save(File* pf)
{
  pf->WriteComment("// Bonus solid name");
  pf->Write(m_pSolid->GetName());

  pf->WriteComment("// Bonus orientation");
  m_pSolid->GetOrientation()->Save(pf);

  pf->WriteComment("// Bonus explosion name");
  pf->Write(m_pExplosion->GetName());

  // Save bonus points
  pf->WriteInteger(m_bonusPoints);

  return true; 
}

static const std::string SOLID_NAME = "solid name";
static const std::string EXPLOSION_NAME = "explosion name";

GameObject::PropertyMap Bonus::GetProperties() const
{
  PropertyMap pm;
  pm[SOLID_NAME] = GameObject::Property(m_pSolid->GetName());
  pm[EXPLOSION_NAME] = GameObject::Property(m_pExplosion->GetName());
  return pm;
}

void Bonus::SetProperties(const GameObject::PropertyMap& pm)
{
  std::string solidName = const_cast<GameObject::PropertyMap&>(pm)[SOLID_NAME].GetString();
  PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
  if (pSolid.GetPtr())
  {
    m_pSolid = pSolid;
  }
  else
  {
    // ERROR TODO
  }

  std::string explosionName = const_cast<GameObject::PropertyMap&>(pm)[EXPLOSION_NAME].GetString();
  if (!m_pExplosion->Load("melonexplode.txt"))
  {
    // ERROR TODO
  }
}
#endif

bool Bonus::Load(File* pf)
{
  // Load the Solid 
  string name;
  if (!pf->GetDataLine(&name))
  {
    pf->ReportError("Expected Solid name.");
    return false;
  }
  m_pSolid = SolidComponent::LoadSolid(name); 
  if (!m_pSolid.GetPtr())
  {
    pf->ReportError("Failed to load Bonus Solid.");
    return false;
  }
  // Load the orientation/position 
  Orientation o;
  if (!o.Load(pf))
  {
    pf->ReportError("Failed to load Bonus orientation.");
    return false;
  }
  m_pSolid->SetOrientation(o);
 
  // Load the explosion/particle effect
  string explosionFileName;
  if (!pf->GetDataLine(&explosionFileName))
  {
    pf->ReportError("Expected explosion file name.");
    return false;
  }

  // Get points for this bonus
  if (!pf->GetInteger(&m_bonusPoints))
  {
    pf->ReportError("Expected bonus points.");
    return false;
  }

  // TODO
/*
  string solidName = Engine::Instance()->GetConfigValue("melon_particle");
  PSolidComponent pBb = SolidComponent::LoadSolid(solidName);
  if (!pBb.GetPtr())
  {
    pf->ReportError("Bad particle name for bonus.");
    return false;
  }
  // TODO CONFIG num particles, max vel/time 
  float numParticles = atof(Engine::Instance()->GetConfigValue("melon_num_particles").c_str());
  if (numParticles == 0)
  {
    return false;
  }
  m_explosion.Init(numParticles, pBb, 10, 10);
*/

  if (!m_pExplosion->Load(explosionFileName))
  {
    pf->ReportError("Failed to load explosion effect.");
    return false;
  }

  return true;
}

void Bonus::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
/*
  // *** HACK ***
  // Instead of a general bonus type, this is full of game-specific stuff :-(
  if (!pPlayer)
  {
    return;
  }

  if (GetState() != UNKNOWN)
  {
    // Don't react if in exploding state.
    return; 
  }

  Engine* e = Engine::Instance();
  
  // Play bonus sound.. this SHOULD BE handled by a Watcher watching the bonus.
  e->PlayWav(e->GetConfigValue("wav_extra_time"));
  
  // The bonus goes out of play, but we must allow a delay for the particle 
  // effect to be seen.
  e->SendGameMessage(new Message("bonus explode", 0, GetId(), GetId(), BONUS_EXPLODE));
  // TODO m_explodeTime
  e->SendGameMessage(new Message("bonus dead", 5, GetId(), GetId(), OUT_OF_PLAY));

  // Now we expect Watchers to do the work, this would be done by the
  // Watcher.
  
  // Send a message to ourselves to reappear some time later.
  e->SendGameMessage(new Message("bonus reappear", 20, // TODO CONFIG
      GetId(), GetId(), UNKNOWN));
  
  // particle effect
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());

  // TODO total hack!
  // Increase player time
  float timeBoost = Engine::Instance()->GetConfigFloat("time_boost");
  ((EngineRunning*)Engine::Instance()->
    GetEngineState(EngineRunning::Name).GetPtr())->
    IncreaseLevelTime(timeBoost);
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_extra_time"));
*/
}

void Bonus::HandleObjectCollision(PoolGameObject* pObj)
{
  if (!pObj)
  {
    return;
  }
  if (GetState() != UNKNOWN)
  {
    // Don't react if in exploding state.
    return; 
  }

  Engine* e = Engine::Instance();
  // The bonus goes out of play, but we must allow a delay for the particle 
  // effect to be seen.
  //e->SendGameMessage(
  //  new Message("bonus explode", 0, GetId(), GetId(), BONUS_EXPLODE));
  SetState(BONUS_EXPLODE);

  // TODO m_explodeTime
  e->SendGameMessage(new Message("bonus dead", 5, GetId(), GetId(), OUT_OF_PLAY));

  // particle effect
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());

  // Increase score
  AddToScore(m_bonusPoints); // PoolMisc

  // Increase number of bonuses collected
  e->GetGameState()->GetCurrentPlayerInfo()->m_totalBonuses++;

  EngineStatePoolBase::CreateBonusesLeft();

  Engine::Instance()->PlayWav(
    Engine::Instance()->GetConfigValue("golf_wav_7"));
}

void Bonus::Update()
{
  Assert(m_pSolid.GetPtr());
  m_pSolid->Update();  
}

void Bonus::CullDraw(Frustum* pFrustum)
{
  SolidGameObject::CullDraw(pFrustum);
  // Draw the explosion here so we still see the particles even if the bonus
  // is not in the view frustum. 
  m_pExplosion->Draw();
}

void Bonus::Draw()
{
  if (GetState() == UNKNOWN)
  {
    SolidGameObject::Draw();
  }
}

void Bonus::DrawShadow() const
{
  if (GetState() == UNKNOWN)
  {
    AmjuGL::PushMatrix();
    SolidGameObject::DrawShadow();
    AmjuGL::PopMatrix();
  }
}

void Bonus::RecalcCollisionVol() 
{
  // Do nothing - bonuses don't collide with NPCs, and disappear when the player
  // intersects them.
}

void Bonus::OnRoomEntry()
{
  // Make sure bonus is resting on the ground underneath it.
  m_pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  BoundingSphere bigBs(VertexBase(0, 0, 0), 1000.0f);
  HeightServer hs;
  m_pLevel->GetHeightServer(&hs, bigBs);
  Orientation o = *(m_pSolid->GetOrientation());
  const HeightPoly* pHp = hs.GetHeightPoly(o.GetX(), o.GetY() + 2.0f, o.GetY() - 20.0f, o.GetZ());
  //  (x, ymax, ymin, z)

  if (pHp)
  {
    float newy = pHp->GetY(o.GetX(), o.GetZ());
    o.SetY(newy);
    SetOrientation(o);
  }

  SetShadowSize(1.0f); // TOCO TEMP TEST, why no shadows ?
  RefreshShadow();
}
}

