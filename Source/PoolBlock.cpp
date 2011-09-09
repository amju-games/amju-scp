/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolBlock.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:18  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:21  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:37  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.7  2005/09/30 13:02:25  jay
Minor tweaks - SHOULD cause no difference in behaviour.

Revision 1.1.2.6  2005/09/23 19:34:37  jay
Remove dead code; tried to fix shadow but still not working

Revision 1.1.2.5  2005/09/14 10:07:56  jay
Added separate collision solid. This is mainly to stop blocks falling
through each other, but will also give better performance.

Revision 1.1.2.4  2005/09/11 20:50:07  jay
Fix the scene ed fix :-(

Revision 1.1.2.3  2005/09/11 20:18:33  Administrator
Scene Ed fixes

Revision 1.1.2.2  2005/09/11 19:03:20  jay
Added SCENE_EDITOR functions

Revision 1.1.2.1  2005/09/08 20:02:37  jay
Added destroyable block for CRAZY POOL

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolBlock.h"
#include "PoolMisc.h"
#include "File.h"
#include "Level.h"
#include "Engine.h"
#include "FallDownController.h"
#include "BallController.h"
#include "TextureServer.h"
#include "SolidComposite.h"

namespace Amju
{
PoolGameObject* CreatePoolBlock()
{
  return new PoolBlock;
}

PoolBlock::PoolBlock()
{
  SetController(new FallDownController);

#ifdef SCENE_EDITOR
  m_pSolid = SolidComponent::LoadSolid("cube8.comp");
  m_pCollisionSolid = SolidComponent::LoadSolid("cube8.comp");
#endif
}

const char* PoolBlock::GetTypeName() const
{
  return "pool-block";
}

void PoolBlock::SetOrientation(const Orientation& o)
{
  FreeMovingSolidGameObject::SetOrientation(o);
}

void PoolBlock::Draw()
{
  if (GetState() == UNKNOWN)
  {
    FreeMovingSolidGameObject::Draw();
#ifdef HS_DEBUG
    GetCollisionVol()->DrawHeights();
    GetCollisionVol()->DrawWalls();
#endif
  }
}

void PoolBlock::Update()
{
  Assert(m_pController.GetPtr());
  FreeMovingSolidGameObject::Update();
}

bool PoolBlock::Load(File* pf)
{
  std::string solidName;
  if (!pf->GetDataLine(&solidName))
  {
    pf->ReportError("Expected pool block solid name.");
    return false;
  }
  PSolidComponent pComp = SolidComponent::LoadSolid(solidName);
  if (!pComp.GetPtr())
  {
    pf->ReportError("Bad solid for pool block.");
    return false;
  }
  m_pSolid = pComp;

  // Now load the Collision Volume
  if (!pf->GetDataLine(&solidName))
  {
    pf->ReportError("Expected pool block collision volume name.");
    return false;
  }
  pComp = SolidComponent::LoadSolid(solidName);
  if (!pComp.GetPtr())
  {
    pf->ReportError("Bad collision volume for pool block.");
    return false;
  }
  m_pCollisionSolid = pComp;
  Matrix m;
  m.SetIdentity();
  m_pCollisionSolid->RecalculateAbsoluteCoords(m);

  if (!m_startOrientation.Load(pf))
  {
    pf->ReportError("Failed to load pool block orientation.\n");
    return false;
  }
  // Required for Scene Ed
  SetOrientation(m_startOrientation);

  std::string strTex = Engine::Instance()->GetConfigValue("player_bw_texture");
  std::string strAlpha = Engine::Instance()->GetConfigValue("player_bw_alpha");
  PoolTexture* pBwTex = TextureServer::Instance()->Get(strTex, strAlpha);
  if (!pBwTex)
  {
    pf->ReportError("Failed to load blast wave texture.");
    return false;
  }
  pBwTex->CreateBinding(PoolTexture::REGULAR);
  float speed = Engine::Instance()->GetConfigFloat("player_bw_speed");
  float time = Engine::Instance()->GetConfigFloat("player_bw_time");
  m_blastWave.Init(pBwTex, speed, time, 0, 0);

  return true;
}

void PoolBlock::OnRoomEntry()
{
  // Can't do this in Load(), m_pLevel not set. This seems to be a serious
  // oversight!
  SetSolid(m_pSolid);
  SetOrientation(m_startOrientation);

  // Reset block if it has exploded.
  SetState(UNKNOWN);

  // TODO TEMP TEST
  SetShadowSize(3.0f); // radius ?
  CreateShadow();
}

void PoolBlock::CullDraw(Frustum* pf)
{
  FreeMovingSolidGameObject::CullDraw(pf);
  m_blastWave.Draw();
}

const HeightServer* PoolBlock::GetCollisionVol() const
{
  if (GetState() == DEAD)
  {
    return 0;
  }
  return FreeMovingSolidGameObject::GetCollisionVol();
}

void PoolBlock::RecalcCollisionVol()
{
  Matrix m;
  m.SetIdentity();
  BoundingSphere bs(Vec3f(0, 0, 0), 1000); // big B.S.
  m_collisionVolume.Clear();

  GetOrientation()->TransformMatrix(&m);

  m_pCollisionSolid->CreateBoundingSphere(m);
  m_pCollisionSolid->StoreHeights(m);
  m_pCollisionSolid->AddHeights(&m_collisionVolume, bs);
}

void PoolBlock::HandleObjectCollision(PoolGameObject* pObj)
{
  if (GetState() == DEAD ||
      GetState() == FADING ||
      GetState() == OUT_OF_PLAY)
  {
    return; 
  }

  // This should have no effect - the only thing that blows up heads
  // is the cue ball.
/* 
  if (pObj->GetState() != UNKNOWN)
  {
    return;
  }
*/

  // If hit by cue ball, explode; any objects on top should drop down.
  // CAN'T make it any ball as a hack to fix block-ball collision response.
  // - no good as pool balls rest on top of blocks!
  if (IsCueBall(pObj))
  {
#ifdef _DEBUG
std::cout << "Pool block " << GetId() << ": collision with ball: " << pObj->GetId() << " (" << pObj->GetTypeName() << ")\n";
#endif

    GetRules(m_pLevel)->BallHitsObject((PoolBall*)pObj, this);

    // Cue ball is in B.Sphere - check for collision with actual solid.
    const BoundingSphere& bs = *(pObj->GetBoundingSphere());
    const Vec3f& v = bs.GetCentre();

    m_blastWave.Reset(v);

#ifdef _DEBUG
std::cout << "COLLISION - Cue Ball with PoolBlock!\n";
#endif
    Engine::Instance()->PlayWav(
      Engine::Instance()->GetConfigValue("golf_wav_9")); // explosion

    float severity = Engine::Instance()->GetConfigFloat("quake_severity");
    Engine::Instance()->GetEarthquake().SetActive(0.5f, severity);

    SetState(DEAD);
    // Send a message to put us completely out of play in a short time.
    Engine::Instance()->SendGameMessage(new Message("block o.o.p.", 5,
      GetId(), GetId(), OUT_OF_PLAY));

    // All other objects should recalc their HeightServers in case they were
    // on top of this one..?
    int levelId = m_pLevel->GetId(); 
    int roomId = m_pLevel->GetRoomId();
    GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
    // Iterate through map of Game Objects.  
    for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
    {
      //GameObjectId gameObjId = it->first;
      PPoolGameObject pGo = it->second;
      VisibleGameObject* pVis = dynamic_cast<VisibleGameObject*>(pGo.GetPtr());
      if (pVis)
      {
        pVis->RefreshHeightServer();
      }
    } 
  }
}

#if defined(SCENE_EDITOR)
bool PoolBlock::Save(File* pf)
{
  if (!pf->WriteComment("// Solid name"))
  {
    return false;
  }
  pf->Write(m_pSolid->GetName());
  pf->WriteComment("// Collision solid");
  pf->Write(m_pCollisionSolid->GetName());
  pf->WriteComment("// Orientation");
  m_pSolid->GetOrientation()->Save(pf);

  return true;
}

static const char* COMP_FILE = "Solid comp file";
static const char* COLLISION_FILE = "Collision comp file";

GameObject::PropertyMap PoolBlock::GetProperties() const
{
  PropertyMap pm;
  pm[COMP_FILE] = m_pSolid->GetName();
  pm[COLLISION_FILE] = m_pCollisionSolid->GetName();

  return pm;
}

void PoolBlock::SetProperties(const PropertyMap& pm)
{
  std::string compFile =
    const_cast<GameObject::PropertyMap&>(pm)[COMP_FILE].GetString();
  PSolidComponent pComp = SolidComponent::LoadSolid(compFile);
  if (pComp.GetPtr())
  {
    m_pSolid = pComp;
  }

  compFile = const_cast<GameObject::PropertyMap&>(pm)[COLLISION_FILE].GetString();
  pComp = SolidComponent::LoadSolid(compFile);
  if (pComp.GetPtr())
  {
    m_pCollisionSolid = pComp;
  }
}
#endif
}


