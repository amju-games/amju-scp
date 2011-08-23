/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Button.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Button.h"
#include "File.h"
#include "SolidComponent.h"
#include "Engine.h"
#include "GameObjectFactory.h"
#include <algorithm>

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateButton() { return new Button; }
static const bool isButtonRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("button", CreateButton);
*/

Button::Button()
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
  PSolidComponent pSolid = SolidComponent::LoadSolid("cube2.comp");
  SetSolid(pSolid);
#endif
}

#if defined(SCENE_EDITOR)
bool Button::Save(File* pf)
{
  pf->WriteComment("// Button solid name");
  pf->Write(m_pSolid->GetName());

  pf->WriteComment("// Button set orientation");
  m_setPosition.Save(pf);

  pf->WriteComment("// Button unset orientation");
  m_unsetPosition.Save(pf);

  pf->WriteComment("// Button messages");
  m_setMessages.Save(pf);

  return true; 
}

static const std::string SOLID_NAME = "solid name";

GameObject::PropertyMap Button::GetProperties() const
{
  // TODO orientations and messages
  PropertyMap pm;
  pm[SOLID_NAME] = GameObject::Property(m_pSolid->GetName());
  return pm;
}

void Button::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO orientations and messages
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
}
#endif

const char* Button::GetTypeName() const
{
  return "button";
}

bool Button::Load(File* pf)
{
  // Get the Solid.
  std::string solidname;
  if (!pf->GetDataLine(&solidname))
  {
    pf->ReportError("Expected solid name.");
    return false;
  }
  m_pSolid = SolidComponent::LoadSolid(solidname);
  if (!m_pSolid.GetPtr())
  {
    pf->ReportError("Bad solid name.");
    return false;
  }

  // Get the set orientation.
  if (!m_setPosition.Load(pf))
  {
    pf->ReportError("Failed to load set position.");
    return false;
  }

  // Get the unset orientation.
  if (!m_unsetPosition.Load(pf))
  {
    pf->ReportError("Failed to load un-set position.");
    return false;
  }

  if (!m_setMessages.Load(pf, GetId())) // messages specify this button as sender 
  {
    pf->ReportError("Failed to load button messages.");
    return false;
  }

  return true;
}

void Button::DrawShadow() const
{
}

void Button::Update()
{
  
  // move smoothly from set to unset positions
  if (m_isMoving)
  {
    m_interp += Engine::Instance()->GetDeltaTime(); // TODO velocity
    if (m_interp >= 1.0f)
    {
      // Finished moving.
      m_interp = 1.0f;
      m_isMoving = false;
    }

    // Set the position: it's somewhere between the Set and Unset positions.
    Orientation pos;
    if (GetState() == SET)
    {
      // We are moving towards the Set pos.
      pos = Interpolate(m_unsetPosition, m_setPosition, m_interp);
    }
    else
    {
      // Moving towards unset pos.
      pos = Interpolate(m_setPosition, m_unsetPosition, m_interp);
    }
    SetOrientation(pos);

  }
  else
  {
    // TODO only need to do this once
    if (GetState() == SET)
    {
      SetOrientation(m_setPosition);
    }
    else
    {
      SetOrientation(m_unsetPosition);
    }
  }
}

void Button::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  // make sure we don't re-set every frame the player is touching the button!
  if (pPlayer && GetState() == UNSET)
  {
    Set(true);
  }
  /*
  // TODO TEMP TEST
  if (!pPlayer && GetState() == SET)
  {
    Set(false);
  }
  */
}

void sendMessage(PMessage m)
{
  Engine::Instance()->SendGameMessage(m);
}

void Button::Set(bool b)
{
  if (m_isMoving)
  {
    // We are already changing state - can't change again.
    return;
  }

  m_isMoving = true;
  m_interp = 0;

  if (b)
  {
    GameObject::SetState(SET);
    //SetOrientation(m_setPosition);
    std::for_each(m_setMessages.begin(), m_setMessages.end(), sendMessage);
  }
  else
  {
    GameObject::SetState(UNSET);
    //SetOrientation(m_unsetPosition);
  }
}

void Button::OnRoomEntry()
{
  m_isMoving = false;
  Set(false);
}

void Button::SetState(State newState)
{
  Set(newState == SET);
}
}

