/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Shed.cpp,v $
Revision 1.3.2.1.4.6  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.3.2.1.4.5  2006/07/26 21:25:17  jay
Use Pool/, not Pool/

Revision 1.3.2.1.4.4  2005/09/23 20:03:38  jay
Notify Rules if hit

Revision 1.3.2.1.4.3  2005/09/13 07:49:05  jay
Reset shed on new game

Revision 1.3.2.1.4.2  2005/09/11 19:00:00  jay
Whitespace

Revision 1.3.2.1.4.1  2005/09/06 20:11:35  Administrator
Minor tweaks

Revision 1.3.2.1  2004/10/01 09:29:40  Administrator
Make each animal set off in a different direction

Revision 1.3  2004/09/28 09:55:39  Administrator
Fix for #10, no shadow

Revision 1.2  2004/09/23 09:07:34  Administrator
Fix CullDraw() so explosions are drawn in the right place.

Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Shed.h"
#include "File.h"
#include "Engine.h"
#include "PoolBall.h"
#include "PoolMisc.h"
#include "Character.h"
#include "Stupid.h"
#include "CharacterManager.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateShed() { return new Shed; }
static const bool isShedRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("shed", CreateShed);
*/

Shed::Shed()
{
  SetShadowSize(3.0f); // TODO CONFIG
  m_isColliding = false;

#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
//  PSolidComponent pSolid = SolidComponent::LoadSolid("shed.comp");
//  SetSolid(pSolid);
  AddMesh(CharacterManager::Instance()->GetCharacter("shed"));
#endif
}

#ifdef SCENE_EDITOR
bool Shed::Save(File* pf)
{
  if (!Npc::Save(pf))
  {
    return false;
  }
  // Save hidden IDs
  pf->WriteComment("// No of hidden IDs");
  pf->WriteInteger(m_hiddenIds.size());
  for (std::vector<int>::iterator it = m_hiddenIds.begin(); it != m_hiddenIds.end(); ++it)
  {
    pf->WriteInteger(*it); 
  }
  return true;
}
#endif

void Shed::Draw()
{
  Npc::Draw();
}

#if defined(SCENE_EDITOR)
GameObject::PropertyMap Shed::GetProperties() const
{
  // TODO
  PropertyMap pm;
  return pm;
}

void Shed::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO
}
#endif

void Shed::HandleObjectCollision(GameObject* pObj)
{
  if (!pObj)
  {
    m_isColliding = false;
  }
  if (m_isColliding)
  {
    return;
  }
  if (GetState() != UNKNOWN)
  {
    return;
  }

  if (dynamic_cast<PoolBall*>(pObj))
  {
    GetRules(m_pLevel)->BallHitsObject((PoolBall*)pObj, this);

    m_isColliding = true;

    // Pool ball hit shed - collapse and enable things inside
    Dead();
    m_explosion.Reset(GetBoundingSphere()->GetCentre());

    // enable inside things
    float delay = 3.0f; // TODO CONFIG

    for (std::vector<int>::iterator it = m_hiddenIds.begin(); it != m_hiddenIds.end(); ++it)
    {
      int hiddenId = *it;
      PGameObject pGo = Engine::Instance()->GetGameObject(hiddenId);
      if (pGo.GetPtr())
      {
        Orientation o = *(GetOrientation());
        // Make each stupid face a different direction. This prevents a procession
        // of Stupids running off a cliff edge.
        static float yRot = 0;
        static const float YROT_INC = 
          Engine::Instance()->GetConfigFloat("stupid_yrot_inc");
        o.SetYRot(yRot);
        yRot += YROT_INC;
        if (yRot > 360.0f)
        {
          yRot -= 360.0f;
        }
        pGo->SetOrientation(o);

        Engine::Instance()->SendGameMessage(new Message(
          "unhide shed obj", delay, 
          GetId(), hiddenId, UNKNOWN));

        delay += 1.0f; // TODO CONFIG
      }
    }

    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_31"));

    float severity = atof(Engine::Instance()->GetConfigValue("quake_severity").c_str());
    Engine::Instance()->GetEarthquake().SetActive(0.5f, severity); 
  }

}

const char* Shed::GetTypeName() const
{
  return "shed";
}

void Shed::CullDraw(Frustum* pF)
{
// Not required, as we derive from Npc.
/*
  if (GetState() == FADING)
  {
    m_fade -= Engine::Instance()->GetDeltaTime() * 0.5f; // TODO CONFIG
    if (m_fade < 0)
    {
      m_fade = 0;
    }
  }
*/
  // We need to save the matrix, or the explosion is way off.
  AmjuGL::PushMatrix();
  Npc::CullDraw(pF);
  AmjuGL::PopMatrix();
  m_explosion.Draw();
}

bool Shed::Load(File* pf)
{
  if (!Npc::Load(pf))
  {
    return false;
  }
  // Get the hidden IDs
  int numIds = 0;
  if (!pf->GetInteger(&numIds))
  {
    pf->ReportError("Expected no of hidden IDs in shed.");
    return false;
  }
  for (int i = 0; i < numIds; i++)
  {
    int id = 0;
    if (!pf->GetInteger(&id))
    {
      pf->ReportError("Expected hidden ID in shed.");
      return false;
    }
    m_hiddenIds.push_back(id);
  }

  // Load explosion
  std::string explFile = Engine::Instance()->GetConfigValue("golf_shed_explode");
  if (!m_explosion.Load(explFile))
  {
    pf->ReportError("Failed to load shed explosion file.");
    return false;
  }
  return true;
}

void Shed::OnRoomEntry()
{
  Npc::OnRoomEntry();
  SetState(UNKNOWN);
  SetActionIdle();
  m_isColliding = false;

  for (std::vector<int>::iterator it = m_hiddenIds.begin(); it != m_hiddenIds.end(); ++it)
  {
    PGameObject pGo = Engine::Instance()->GetGameObject(*it);
    if (pGo.GetPtr())
    {
      pGo->SetOrientation(*(this->GetOrientation()));
      pGo->SetState(OUT_OF_PLAY);
    }
  }
  RefreshShadow();
}
}

