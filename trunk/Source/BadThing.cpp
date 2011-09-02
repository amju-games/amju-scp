/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BadThing.cpp,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "BadThing.h"
#include "File.h"
#include "CharacterGameObject.h"
#include "Engine.h"
#include "PoolGameState.h"

namespace Amju
{
BadThing::BadThing() : 
  m_playerDamage(0), 
  m_collideWithPlayer(false)
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

void BadThing::Draw()
{
  m_bsphere.Draw();
}

bool BadThing::Load(File* pf)
{
  // Get b.sphere
  if (!m_bsphere.Load(pf))
  {
    return false;
  }

  // Get player damage
  if (!pf->GetFloat(&m_playerDamage))
  {
    pf->ReportError("Expected player damage.");
    return false;
  }
  return true;
}

#if defined(SCENE_EDITOR)
bool BadThing::Save(File* pf)
{
  m_bsphere.Save(pf);
  pf->WriteComment("// Player damage");
  pf->WriteFloat(m_playerDamage);
  return true; 
}

static const std::string RADIUS = "radius";
static const std::string PLAYER_DAMAGE = "player damage";

GameObject::PropertyMap BadThing::GetProperties() const
{
  PropertyMap pm;
  pm[PLAYER_DAMAGE] = GameObject::Property(m_playerDamage);
  pm[RADIUS] = GameObject::Property(m_bsphere.GetRadius());

  return pm;
}

void BadThing::SetProperties(const GameObject::PropertyMap& pm)
{
  m_playerDamage = const_cast<GameObject::PropertyMap&>(pm)[PLAYER_DAMAGE].GetFloat();
  m_bsphere.SetRadius(const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat());
}
#endif

const char* BadThing::GetTypeName() const
{
  return "badthing";
}

void BadThing::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (!pPlayer)
  {
    m_collideWithPlayer = false;
    return;
  }

  if (!m_collideWithPlayer)
  {
    m_collideWithPlayer = true;
    pPlayer->SetActionPain(); 

    // (Don't throw player back for now)
//    Engine::Instance()->GetEngineState()->TakeDamage(m_playerDamage);
  }
}

}

