/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Platform.cpp,v $
Revision 1.1.10.1  2005/05/08 08:03:43  jay
Fix GetOrientation - now returns const

Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Platform.h"
#include "Common.h"
#include "CompositeFactory.h"
#include "FunctionFactory.h"
#include "Matrix.h"
#include "Engine.h"
#include "File.h"
#include "CharacterGameObject.h"
#include "Character.h"
#include "GameObjectFactory.h"
#include "StringUtils.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
/*
// Register creation function with Game Object Factory
GameObject* CreatePlatform() { return new Platform; }
static const bool isPlatformRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("platform", CreatePlatform);
*/

Platform::Platform()
{
  m_unknownIsStationary = true;
  m_pActiveObject = 0;

#if defined(SCENE_EDITOR)
  // default constructed Game Object must be saveable for Editor.
  // Assign some default solid, to make this object saveable. 
  std::string defaultComp = Engine::Instance()->GetConfigValue("default_move_name");
  if (defaultComp.empty())
  {
    defaultComp = "melon.move"; 
  }
  m_pMovingSolid = (MovingSolid*)(SolidComponent::LoadSolid(defaultComp).GetPtr());
#endif
}

Platform::~Platform()
{
}

void Platform::CullDraw(Frustum* pFrustum)
{
  Draw();
}

void Platform::RecalcCollisionVol()
{
  Matrix m;
  m.identity();
  m_pMovingSolid->GetInnerSolid()->GetOrientation()->TransformMatrix(&m);
  m_pMovingSolid->StoreHeights(m);
  BoundingSphere bs(VertexBase(0, 0, 0), 1000); // big B.S.
  m_collisionVolume.Clear();
  // Add all the polys (that fit inside bs, which should be all of them)
  // which are part of our Moving Solid to our heightserver.
  m_pMovingSolid->GetInnerSolid()->AddHeights(&m_collisionVolume, bs);
}

void Platform::HandlePlayerCollision(CharacterGameObject* pCharacter)
{
  HandleObjectCollision(pCharacter);
}

void Platform::HandleObjectCollision(GameObject* pObj)
{
  if (pObj && !dynamic_cast<FreeMovingGameObject*>(pObj))
  {
    return;
  }
  // TODO only active if object is resting on platform.

  // TODO j.c. golf, 10/5/2004 commented out the active object bit.
  // This check means the platform is only set once while the
  // object is within the platform b.sphere. 
  // There is now extra checking (in VisibleGameObject) to make
  // sure that the ball/whatever is actually on the platform,
  // so we need to check every frame the object is within the sphere.
  if (pObj) // && !m_pActiveObject)
  {
    ((FreeMovingGameObject*)pObj)->SetActivePlatform(this); 
    m_pActiveObject = (FreeMovingGameObject*)pObj;
  }

  if (!pObj && m_pActiveObject)
  {
    m_pActiveObject->SetActivePlatform(0);
    m_pActiveObject = 0;
  }
}

void Platform::GetOrientationDelta(Orientation* pOrientation)
{
  if (m_pMovingSolid.GetPtr())
  {
    m_pMovingSolid->GetOrientationDelta(pOrientation);
  }
}

void Platform::CreateBoundingSphere(const Matrix& cm)
{
  if (m_pMovingSolid.GetPtr())
  {
    Matrix m = cm;
    // Get initial transformation matrix
    //m_pMovingSolid->GetOrientation()->TransformMatrix(&m); 
    m_pMovingSolid->CreateBoundingSphere(m);
  }
}

const BoundingSphere* Platform::GetBoundingSphere() const
{
  if (m_pMovingSolid.GetPtr())
  {
    return m_pMovingSolid->GetBoundingSphere();
  }
  return 0;
}

#if defined(SCENE_EDITOR)
bool Platform::Save(File* pf)
{
  Assert(m_pMovingSolid.GetPtr());

  pf->WriteComment("// Solid name");
  pf->Write(StripPath(m_pMovingSolid->GetName()));
  pf->WriteComment("// Reset to stationary ?");
  pf->WriteInteger(m_unknownIsStationary ? 0 : 1);
  return true;
}

static const std::string SOLID_NAME = "moving solid name";
static const std::string INITIALLY_STATIONARY = "initially stationary";

GameObject::PropertyMap Platform::GetProperties() const
{
  PropertyMap pm;
  pm[SOLID_NAME] = GameObject::Property(m_pMovingSolid->GetName());
  pm[INITIALLY_STATIONARY] = GameObject::Property(m_unknownIsStationary);

  return pm;
}

void Platform::SetProperties(const GameObject::PropertyMap& pm)
{
  m_unknownIsStationary = 
    const_cast<GameObject::PropertyMap&>(pm)[INITIALLY_STATIONARY].GetBool();

  std::string solidName = const_cast<GameObject::PropertyMap&>(pm)[SOLID_NAME].GetString();
  PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
  if (dynamic_cast<MovingSolid*>(pSolid.GetPtr()))
  {
    m_pMovingSolid = (MovingSolid*)(pSolid.GetPtr());
  }
  else
  {
    // ERROR
  }

}
#endif

const char* Platform::GetTypeName() const
{
  return "platform";
}

bool Platform::Load(File* pf)
{
  Assert(pf);

  string name;
  if (!pf->GetDataLine(&name))
  {
    pf->ReportError("Expected Solid name.");
    return false;
  }

  PSolidComponent pMovingSolid = SolidComponent::LoadSolid(name);

  m_pMovingSolid = dynamic_cast<MovingSolid*>(pMovingSolid.GetPtr()); 
  if (!m_pMovingSolid.GetPtr())
  {
    pf->ReportError("Moving solid is wrong type.");
    return false;
  }

  // Get initial state: on (1) or off (0).
  // This flag is like, "When the game resets, should the
  // platform be moving or stationary".
  int state = 0;
  if (!pf->GetInteger(&state))
  {
    pf->ReportError("Expected initial platform state.");
    return false;
  }

  m_unknownIsStationary = (state == 0);
 
  return true;
}

void Platform::Draw()
{
  Matrix m;
  m.identity();
  CreateBoundingSphere(m);

  Assert(m_pMovingSolid.GetPtr());
  if (m_pMovingSolid.GetPtr())
  {
    // (Updated elsewhere)
    AmjuGL::PushMatrix();
    m_pMovingSolid->Draw();
    AmjuGL::PopMatrix();

#if defined(HS_DEBUG)
    AmjuGL::PushMatrix();
    GetCollisionVol()->DrawHeights();
    GetCollisionVol()->DrawWalls();
    AmjuGL::PopMatrix();
#endif    
  }
}

void Platform::Update()
{
  // State must be MOVING to cause update.
  if (GetState() == STATIONARY || GetState() == UNKNOWN)
  {
    return;
  }

  Assert(m_pMovingSolid.GetPtr());
  m_pMovingSolid->Update();

  RecalcCollisionVol();
}

void Platform::SetState(State s)
{
  PFunction pFunc = m_pMovingSolid->GetFunction();
  Assert(pFunc.GetPtr());

#ifdef _DEBUG
  //std::cout << "Setting platform state, current=" << GetState() << " new=" << s << " uknownIsStationary=" << m_unknownIsStationary << "\n";
#endif

  if (s == MOVING)
  {
    // Start to move. Reset the function time.
    pFunc->SetTime(0);
  }
  else if (s == UNKNOWN)
  {
    // Start from the Initial position
    pFunc->SetTime(0);
    m_pMovingSolid->Update();    

    if (!m_unknownIsStationary)
    {
      s = MOVING;
    }
  }

  VisibleGameObject::SetState(s);
}

void Platform::SetOrientation(const Orientation& o)
{
  Assert(m_pMovingSolid.GetPtr());
  *(m_pMovingSolid->GetOrientation()) = o;

  // CreateBoundSphere ?
  Matrix m;
  m.identity();
  CreateBoundingSphere(m);
}

const Orientation* Platform::GetOrientation()
{
  Assert(m_pMovingSolid.GetPtr());
  return m_pMovingSolid->GetOrientation();
}

VertexBase Platform::GetRotationCentre() const
{
  Assert(m_pMovingSolid.GetPtr());
  Assert(m_pMovingSolid->GetInnerSolid().GetPtr());

  return m_pMovingSolid->GetInnerSolid()->GetOrientation()->GetVertex();
}

void Platform::OnRoomEntry()
{
  // TODO This probably depends on a flag. 
  // We espeically don't want blockers to move until they
  // have been triggered!
  SetState(UNKNOWN); //MOVING);
}
}

