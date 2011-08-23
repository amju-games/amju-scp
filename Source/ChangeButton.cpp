/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChangeButton.cpp,v $
Revision 1.3.6.2  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.3.6.1  2006/07/26 21:25:16  jay
Use Pool/, not Pool/

Revision 1.3  2004/09/28 09:55:39  Administrator
Fix for #10, no shadow

Revision 1.2  2004/09/17 12:46:11  Administrator
Just added an assert

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "ChangeButton.h"
#include "File.h"
#include "Frustum.h"
#include "Engine.h"
#include "PoolMisc.h"

namespace Amju
{
static const float SHADOW_SIZE = 1.0f;
ChangeButton::ChangeButton() : m_isColliding(false)
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
  PSolidComponent pSolid = SolidComponent::LoadSolid("cube2.comp");
  Assert(pSolid.GetPtr());
  SetSolid(pSolid);
  
  // Shadow size is set in SetSolid, and uses collision vol size.
  // As there is no collision vol for this, we must reset the shadow size.
  SetShadowSize(SHADOW_SIZE);

  m_solids.push_back(pSolid);
  m_solids.push_back(pSolid);

  m_pExplosion = new ExplosionEffect;
  m_pExplosion->Load("melonexplode.txt");
#endif
}

const char* ChangeButton::GetTypeName() const
{
  return "change_button";
}
/*
bool ChangeButton::SaveMessageLists(File* pf)
{
  int size = m_messageLists.size();
  pf->WriteInteger(size);
  for (int i = 0; i < size; i++)
  {
    m_messageLists[i].Save(pf);
  }
  return true;
}

bool ChangeButton::LoadMessageLists(File* pf)
{
  int num = 0;
  if (!pf->GetInteger(&num))
  {
    pf->ReportError("Expected number of message lists.");
    return false;
  }
  for (int i = 0; i < num; i++)
  {
    MessageList ml;
    if (!ml.Load(pf, GetId()))
    {
      pf->ReportError("Error loadinbg message list.");
      return false;
    }
    m_messageLists.push_back(ml);
  }
  return true;
}
*/
#ifdef SCENE_EDITOR
bool ChangeButton::Save(File* pf)
{
  // Save solids
  pf->WriteInteger(m_solids.size());
  for (int i = 0; i < m_solids.size(); i++)
  {
    pf->Write(m_solids[i]->GetName());
    m_solids[i]->GetOrientation()->Save(pf);
  }
  pf->WriteComment("// Explosion file");
  pf->Write(m_pExplosion->GetName());

  SaveMessageLists(pf);

  return true;
}
#endif

bool ChangeButton::Load(File* pf)
{
  m_solids.clear(); // for default-added solids

  // Get the number of states. For each state, load the solid.
  // Load the message list for each state.
  // Check that number of message lists == no. of solids
  int numSolids = 0;
  if (!pf->GetInteger(&numSolids))
  {
    pf->ReportError("Expected number of solids for change button.");
    return false;
  }
  for (int i = 0; i < numSolids; i++)
  {
    std::string s;
    if (!pf->GetDataLine(&s))
    {
      pf->ReportError("Expected solid name for change button.");
      return false;
    }
    PSolidComponent pComp = SolidComponent::LoadSolid(s);
    if (!pComp.GetPtr())
    {
      pf->ReportError("Bad name for solid for change button.");
      return false;
    }
    // Load orientation
    Orientation o;
    if (!o.Load(pf))
    {
      pf->ReportError("Failed to load solid orientation for change button.");
      return false;
    }
    pComp->SetOrientation(o);
    m_solids.push_back(pComp);
  }

  SetSolid(m_solids[0]);
  // Shadow size is set in SetSolid, and uses collision vol size.
  // As there is no collision vol for this, we must reset the shadow size.
  SetShadowSize(SHADOW_SIZE);
  
  // Get explosion filename
  std::string s;
  if (!pf->GetDataLine(&s))
  {
    pf->ReportError("Expected explosion filename for change button.");
    return false;
  }
  m_pExplosion = new ExplosionEffect;
  if (!m_pExplosion->Load(s))
  {
    pf->ReportError("Failed to load explosion for change button.");
    return false;
  }

  if (!LoadMessageLists(pf))
  {
    pf->ReportError("Failed to load messsage lists for change button.");
    return false;
  }
  if (m_solids.size() != m_lists.size())
  {
    pf->ReportError("Change button size mismatch.");
    return false;
  }

  return true;
}

void ChangeButton::SetOrientation(const Orientation& o)
{
  ButtonBase::SetOrientation(o);
  for (int i = 0; i < m_solids.size(); i++)
  {
    m_solids[i]->SetOrientation(o);
  }
}

void ChangeButton::DrawShadow() const
{
  ButtonBase::DrawShadow(); 
}

void ChangeButton::Update()
{
  m_solids[m_buttonState]->Update();
}

void ChangeButton::Draw()
{
  // DON'T push/pop matrix - we need the orientation in CullDraw().
  m_solids[m_buttonState]->GetOrientation()->Draw();
  m_solids[m_buttonState]->Draw();
}

void ChangeButton::CullDraw(Frustum* pFrustum)
{
  if (pFrustum->Contains(*(GetBoundingSphere())))
  {
    Draw();
  }
  // Draw the explosion here so we still see the particles even if the bonus
  // is not in the view frustum. 
  m_pExplosion->Draw();
}

void ChangeButton::OnRoomEntry()
{
  // Reset state.
  m_buttonState = 0;
  SetSolid(m_solids[0]);
  m_isColliding = false;

  // Shadow size is set in SetSolid, and uses collision vol size.
  // As there is no collision vol for this, we must reset the shadow size.
  SetShadowSize(SHADOW_SIZE);
  RefreshShadow();
}

void ChangeButton::SetState(State newState)
{
  // Treat the state as an integer.
  // NB The state could be set to OUT_OF_PLAY (expecially if this is a hidden item),
  // so we must handle this correctly.
  // TODO

  ButtonBase::SetState(newState);
}

void ChangeButton::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
}

void ChangeButton::HandleObjectCollision(GameObject* pGo)
{
  if (!pGo)
  {
    m_isColliding = false;
    return;
  }

  if (m_isColliding)
  {
    return;
  }

  if (pGo->GetId() != GetBall()->GetId())
  {
    return;
  }

  m_lists[m_buttonState].SendMessages();
/*
  // *** HACK ***
  // Point camera at recipient of message
  if (!m_lists[m_buttonState].empty())
  {
    PMessage pM = m_lists[m_buttonState][0];
    int id = pM->GetRecipientId();
    PGameObject pGo = Engine::Instance()->GetGameObject(id);
    Engine::Instance()->GetEngineState()->GetCamera()->
      SetPlayer((FreeMovingGameObject*)pGo.GetPtr());
    Engine::Instance()->GetEngineState()->GetCamera()->
      Reset();
  }
*/
  // Go to next state. Wrap if final state has been reached.
  if (m_buttonState == (m_solids.size() - 1))
  {
    // This is for buttons which can only be turned on once.
    return;
  }
  m_buttonState++;
  SetSolid(m_solids[m_buttonState]); 

  // Shadow size is set in SetSolid, and uses collision vol size.
  // As there is no collision vol for this, we must reset the shadow size.
  SetShadowSize(SHADOW_SIZE);
  
  m_pExplosion->Reset(GetBoundingSphere()->GetCentre());
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_change_button"));

  // TODO CONFIG
  Engine::Instance()->GetEarthquake().SetActive(0.5f, 0.1f);

  m_isColliding = true;
}

#if defined(SCENE_EDITOR)  
static const std::string SOLID_NAME_1 = "solid 1";
static const std::string SOLID_NAME_2 = "solid 2";

ChangeButton::PropertyMap ChangeButton::GetProperties() const
{
  PropertyMap pm;
  pm[SOLID_NAME_1] = GameObject::Property(m_solids[0]->GetName());
  pm[SOLID_NAME_2] = GameObject::Property(m_solids[1]->GetName());
  return pm;
}

void ChangeButton::SetProperties(const PropertyMap& pm)
{
  std::string solidName1 = const_cast<GameObject::PropertyMap&>(pm)[SOLID_NAME_1].GetString();
  PSolidComponent pSolid1 = SolidComponent::LoadSolid(solidName1);
  if (pSolid1.GetPtr())
  {
    m_solids[0] = pSolid1;
  }
  else
  {
    // ERROR TODO
  }

  std::string solidName2 = const_cast<GameObject::PropertyMap&>(pm)[SOLID_NAME_2].GetString();
  PSolidComponent pSolid2 = SolidComponent::LoadSolid(solidName2);
  if (pSolid2.GetPtr())
  {
    m_solids[1] = pSolid2;
  }
  else
  {
    // ERROR TODO
  }

}

#endif  

}



