/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectFire.cpp,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#include "GameObjectFire.h"
#include "Fire.h"

namespace Amju
{
GameObjectFire::GameObjectFire()
{
  m_isTranslucent = true;
}

const char* GameObjectFire::GetTypeName() const
{
  return "fire";
}
const BoundingSphere* GameObjectFire::GetBoundingSphere() const
{
  return m_pFire->GetBoundingSphere();
}

void GameObjectFire::CreateBoundingSphere(const Matrix& cm)
{
  m_pFire->CreateBoundingSphere(cm);
}

bool GameObjectFire::Load(File* pf)
{
  std::string s;
  if (!pf->GetDataLine(&s))
  {
    pf->ReportError("Expected filename for fire.");
    return false;
  }
  m_pFire = SolidComponent::LoadSolid(s);
  if (!m_pFire.GetPtr())
  {
    pf->ReportError("Failed to load fire.");
    return false;
  }
  Orientation o;
  if (!o.Load(pf))
  {
    pf->ReportError("Failed to load fire orientation.");
    return false; 
  }
  m_pFire->SetOrientation(o);
  return true;
}

void GameObjectFire::Draw()
{
  m_pFire->Draw();
}

void GameObjectFire::DrawShadow() const
{
}

void GameObjectFire::Update()
{
  m_pFire->Update();
}

void GameObjectFire::OnRoomEntry()
{
}

void GameObjectFire::SetState(State newState)
{
}

void GameObjectFire::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
}

void GameObjectFire::HandleObjectCollision(GameObject* pGo)
{
  // Game object catches fire
///  pGo->SetOnFire(true);
}

#ifdef SCENE_EDITOR
bool GameObjectFire::Save(File* pf)
{
  pf->WriteComment("// Filename for fire.");
  pf->Write(m_pFire->GetName());
  pf->WriteComment("// Fire orientation.");
  m_pFire->GetOrientation()->Save(pf);
  return true;
}

GameObjectFire::PropertyMap GameObjectFire::GetProperties() const
{
  return PropertyMap();
}

void GameObjectFire::SetProperties(const PropertyMap&)
{
}
#endif

}


