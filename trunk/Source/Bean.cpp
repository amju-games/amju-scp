/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Bean.cpp,v $
Revision 1.1  2004/09/08 15:42:33  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Bean.h"
#include "SolidComponent.h"
#include "File.h"
#include "Common.h"
#include "Engine.h"
#include "GameState.h"
#include "GameObjectFactory.h"
#include "Stupid.h"
#include "Level.h"
#include "Controller.h"

using namespace std;

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateBean() { return new Bean; }
static const bool isBeanRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("bean", CreateBean);
*/

Bean::Bean()
{
  m_pExplosion = new ExplosionEffectMulticolour;
  // Default explosion. TODO this should be explicitly set.
  m_pExplosion->Load("melonexplode.txt"); // TODO CONFIG

#if defined(SCENE_EDITOR)
#endif
}

const char* Bean::GetTypeName() const
{
  return "bean";
}
/*
bool Bean::Save(File* pf)
{
  pf->WriteComment("// Bean solid name");
  pf->Write(m_pSolid->GetName());

  pf->WriteComment("// Bean orientation");
  m_pSolid->GetOrientation()->Save(pf);

  pf->WriteComment("// Explosion file");
  pf->Write("melonexplode.txt"); // TODO TEMP TEST

  return true; 
}
*/
#if defined(SCENE_EDITOR)
static const std::string SOLID_NAME = "solid name";
static const std::string VALUE = "value";

GameObject::PropertyMap Bean::GetProperties() const
{
  PropertyMap pm;
  pm[SOLID_NAME] = GameObject::Property(m_pSolid->GetName());
  return pm;
}

void Bean::SetProperties(const GameObject::PropertyMap& pm)
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

void Bean::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
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
  e->PlayWav(e->GetConfigValue("wav_turn_dir"));
   // The bonus goes out of play, but we must allow a delay for the particle 
  // effect to be seen.
  e->SendGameMessage(new Message("bonus explode", 0, GetId(), GetId(), BONUS_EXPLODE));
  // TODO m_explodeTime
  e->SendGameMessage(new Message("bonus dead", 5, GetId(), GetId(), OUT_OF_PLAY));
  // Send a message to ourselves to reappear some time later.
  e->SendGameMessage(new Message("bonus reappear", 20, // TODO CONFIG
      GetId(), GetId(), UNKNOWN));
  
  // particle effect
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());

  DoBeanAction();

}
}

