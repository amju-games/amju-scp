/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LaunchPad.cpp,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LaunchPad.h"
#include "CharacterGameObject.h"
#include "Engine.h"
#include "Message.h"
#include "File.h"

namespace Amju
{
LaunchPad::LaunchPad() : m_upVel(10.0f), m_restoreTime(5.0f)
{
}
  
const char* LaunchPad::GetTypeName() const
{
  return "launchpad";
}

bool LaunchPad::Load(File* pf)
{
  if (!Target::Load(pf))
  {
    return false;
  }
  if (!pf->GetFloat(&m_upVel))
  {
    pf->ReportError("Expected launchpad up vel.");
    return false;
  }
  if (!pf->GetFloat(&m_restoreTime))
  {
    pf->ReportError("Expected restore time.");
    return false;
  }

  return true;
}

#if defined(SCENE_EDITOR)
bool LaunchPad::Save(File* pf)
{
  if (!Target::Save(pf))
  {
    return false;
  }
  pf->WriteComment("// Up vel");
  pf->WriteFloat(m_upVel);

  pf->WriteComment("// Restore time");
  pf->WriteFloat(m_restoreTime);

  return true;
}

static const std::string UP_VEL = "up vel";
static const std::string RESTORE_TIME = "restore time";

GameObject::PropertyMap LaunchPad::GetProperties() const
{
  PropertyMap pm = Target::GetProperties();
  pm[UP_VEL] = GameObject::Property(m_upVel);
  pm[RESTORE_TIME] = GameObject::Property(m_restoreTime);
  return pm;
}

void LaunchPad::SetProperties(const GameObject::PropertyMap& pm)
{
  Target::SetProperties(pm);
  m_upVel = (const_cast<GameObject::PropertyMap&>(pm)[UP_VEL].GetFloat());
  m_restoreTime = (const_cast<GameObject::PropertyMap&>(pm)[RESTORE_TIME].GetFloat());
}
#endif

void LaunchPad::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (GetState() == OUT_OF_PLAY)
  {
    return;
  }
 
  if (!pPlayer)
  {
    return;
  }

  // change state so a Watcher can do something.
  // Also prevents things from helplessly bouncing up and down indefinitely.
  SetState(OUT_OF_PLAY);
  // Restore launchpad after a delay
  Engine::Instance()->SendGameMessage(new Message("reset launchpad", m_restoreTime,
    GetId(), GetId(), UNKNOWN));

  pPlayer->SetVerticalVel(m_upVel); 
  pPlayer->SetFalling(true); // (falling upwards!)
}

void LaunchPad::HandleObjectCollision(GameObject* pObj)
{
  if (GetState() == OUT_OF_PLAY)
  {
    return;
  }

  if (!pObj)
  {
    return;
  }

  // TODO This is total bollocks, only Characters have downward vel!?!
  CharacterGameObject* pChar = dynamic_cast<CharacterGameObject*>(pObj);
  if (pChar)
  {
    HandlePlayerCollision(pChar);
  }
}

}

