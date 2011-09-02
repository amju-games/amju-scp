/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BadPlatform.cpp,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "BadPlatform.h"
#include "File.h"
#include "CharacterGameObject.h"
#include "Engine.h"
#include "PoolGameState.h"
#include "GameObjectFactory.h"

namespace Amju
{
// Register creation function with Game Object Factory
/*
GameObject* CreateBadPlatform() { return new BadPlatform; }
static const bool isBpRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("badplatform", CreateBadPlatform);
*/

BadPlatform::BadPlatform() : 
  m_playerDamage(0), 
  m_isColliding(false)
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

const char* BadPlatform::GetTypeName() const
{
  return "badplatform";
}

#if defined(SCENE_EDITOR)
bool BadPlatform::Save(File* pf)
{
  if (!Platform::Save(pf))
  {
    return false;
  }
  pf->WriteComment("// Player damage");
  pf->WriteFloat(m_playerDamage);
  return true;
}

static const std::string PLAYER_DAMAGE = "player damage";

GameObject::PropertyMap BadPlatform::GetProperties() const
{
  // Get the properties for Platform. Then add the properties for this type.
  PropertyMap pm = Platform::GetProperties();
  pm[PLAYER_DAMAGE] = GameObject::Property(m_playerDamage);

  return pm;
}

void BadPlatform::SetProperties(const GameObject::PropertyMap& pm)
{
  Platform::SetProperties(pm);
  m_playerDamage = const_cast<GameObject::PropertyMap&>(pm)[PLAYER_DAMAGE].GetFloat();
}
#endif

bool BadPlatform::Load(File* pf)
{
  if (!Platform::Load(pf))
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

void BadPlatform::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (!pPlayer)
  {
    // No collision
    m_isColliding = false;
  }
  else
  {
    if (!m_isColliding)
    {
      // First-time collsion, so inflict damage.
      pPlayer->SetActionPain(); 
//      Engine::Instance()->GetEngineState()->TakeDamage(m_playerDamage);
      // TODO Throw player backwards ?
      // TODO particle effect ?
    }
    m_isColliding = true;
  }

  // Now do base class collision stuff (? optional ? )
  Platform::HandlePlayerCollision(pPlayer);
}
}
