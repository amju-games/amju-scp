/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Takeable.cpp,v $
Revision 1.1.10.1  2005/05/08 08:19:11  jay
Fix call to non-const-returning GameObject::GetOrientation()

Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Takeable.h"
#include "SolidComponent.h"
#include "File.h"
#include "Common.h"
#include "Engine.h"
#include "GameState.h"
#include "GameObjectFactory.h"
#include "Player.h"
#include "SolidGameObject.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreateTakeable() { return new Takeable; }
static const bool isTakeableRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("takeable", CreateTakeable);
*/

Takeable::Takeable(VisibleGameObject* pVis) : 
  m_collideWithPlayer(false), 
  m_heightAboveGround(0)
{
  SetGameObject(pVis);
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

/*
const char* Takeable::GetTypeName() const
{
  return "takeable";
}

bool Takeable::Save(File* pf)
{
  pf->WriteComment("// Takeable solid name");
  pf->Write(m_pSolid->GetName());
  pf->WriteComment("// Takeable initial orientation.");
  m_pSolid->GetOrientation()->Save(pf);
  return true;
}


bool Takeable::Load(File* pf)
{
  // Load the Solid 
  string name;
  if (!pf->GetDataLine(&name))
  {
    pf->ReportError("Expected Solid name.");
    return false;
  }
  PSolidComponent pSolid = SolidComponent::LoadSolid(name); 
  if (!pSolid.GetPtr())
  {
    pf->ReportError("Failed to load Takeable Solid.");
    return false;
  }
  // Load the orientation/position 
  if (!pSolid->LoadOrientation(pf))
  {
    pf->ReportError("Failed to load Takeable orientation.");
    return false;
  }

  SetSolid(pSolid);

  // Save initial orientation for Reset().
  m_startOrientation = *(m_pSolid->GetOrientation());
  return true;
}
*/

/*
const HeightServer* Takeable::GetCollisionVol() const
{
  if (GetState() == TAKEN || GetState() == OUT_OF_PLAY)
  {
    return 0;
  }

  return &m_collisionVolume;
}
*/
void Takeable::SetStartOrientation(const Orientation& o)
{
  m_startOrientation = o;
}


const BoundingSphere* Takeable::GetBoundingSphere()
{
  return GetGameObject()->GetBoundingSphere();
}

void Takeable::SetState(State s)
{
  GetGameObject()->SetState(s);
}

PSolidComponent Takeable::GetSolid()
{
  return ((SolidGameObject*)GetGameObject())->GetSolid();
}

State Takeable::GetState() 
{
  return GetGameObject()->GetState();
}

const HeightServer* Takeable::GetCollisionVol() 
{
  return GetGameObject()->GetCollisionVol();
}

void Takeable::ResetPosition()
{
  GetGameObject()->SetOrientation(m_startOrientation);
}

int Takeable::GetId()
{
  return GetGameObject()->GetId();
}

Orientation* Takeable::GetOrientation()
{
  return const_cast<Orientation*>(GetGameObject()->GetOrientation());
}

Level* Takeable::GetLevel()
{ 
  return GetGameObject()->GetLevel();
}

void Takeable::DrawTaken()
{
}

VisibleGameObject* Takeable::GetGameObject()
{ 
  return m_pVis;
}

void Takeable::SetGameObject(VisibleGameObject* pVis)
{
  m_pVis = pVis;
}
}

