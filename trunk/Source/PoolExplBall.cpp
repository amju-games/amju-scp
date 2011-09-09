/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolExplBall.cpp,v $
Revision 1.1.2.4  2007/07/15 22:02:07  Administrator
Fix assert

Revision 1.1.2.3  2006/08/30 21:12:20  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.4  2005/10/01 22:02:25  Administrator
Bug fix: replaced check for DETONATING with m_isDetonating

Revision 1.1.2.3  2005/09/30 13:03:20  jay
Added new member, m_isDetonating; don't use DETONATING state. When in this
state collisions are not detected.
TODO find out why this is.

Revision 1.1.2.2  2005/09/29 19:11:25  jay
Flash out behaviour for exploding balls. There are still some quirks
with collision.

Revision 1.1.2.1  2005/09/23 19:59:42  jay
Add exploding balls for crazy pool

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolExplBall.h"
#include "Engine.h"
#include "TextureServer.h"
#include "Frustum.h"
#include "File.h"
#include "PoolMisc.h"
#include "StateStrings.h"

namespace Amju
{
PoolGameObject* CreatePoolExplBall()
{
  return new PoolExplBall;
}

const char* PoolExplBall::GetTypeName() const
{
  return "pool-expl-ball";
}

void PoolExplBall::OnRoomEntry()
{
  PoolBall::OnRoomEntry();
  m_isDetonating = false;
}

void PoolExplBall::SetState(State s)
{
#ifdef BOMB_DEBUG
std::cout << "PoolExplBall: Setting state now: " << StateStrings[s] << "\n";
#endif

  if (s == DETONATE)
  {
    if (GetState() != UNKNOWN)
    {
#ifdef _DEBUG
std::cout << "PoolExplBall: Got DETONATE message, but current state is " 
  << StateStrings[GetState()] << "\n";
#endif

      return;
    }

    if (!m_isDetonating)
    {
#ifdef _DEBUG
std::cout << "PoolExplBall: Got DETONATE msg, but m_isDetonating is false.\n";
#endif
      Assert(0);
    }

    if (IsGameOver())
    {
#ifdef _DEBUG
std::cout << "PoolExplBall: Got DETONATE message, but Game Is Over\n"; 
#endif
    
      PoolBall::SetState(UNKNOWN);
      return;
    }

#ifdef _DEBUG
std::cout << "Bomb " << GetId() << " exploding now!\n";
#endif

    const BoundingSphere& bs = *(GetBoundingSphere());
    const Vec3f& v = bs.GetCentre();
    m_blastWave.Reset(v);
    m_explosion.Reset(v);

    Engine::Instance()->PlayWav("battle003.wav");
    // NB When Engine State changes earthquake was reset. This has been
    // turned off in Engine so we see the bomb earthquake. 
    Engine::Instance()->GetEarthquake().SetActive(0.5f, 0.5f); // TODO CONFIG

    // Now force Rules so player who made this bomb explode loses.
    if (!IsGameOver())
    {
      // Set the Winner ID to not the current player, so
      // the player who set off the bomb loses. 
      GetRules(m_pLevel)->SetGameOverForExtReason(2,
        "You set off a bomb!");
    }
  }  
  PoolBall::SetState(s);
  // Whatever the old state, we are not detonating now.
  m_isDetonating = false;
}

void PoolExplBall::HandleObjectCollision(PoolGameObject* pObj)
{
//  if (GetState() != UNKNOWN)
//  {
//    return;
//  }

  PoolBall* pBall = dynamic_cast<PoolBall*>(pObj);
  if (pBall)
  {
    PoolBall::HandleBallCollision(pBall);
  }

  if (IsCueBall(pObj))
  {
    GetRules(m_pLevel)->BallHitsObject((PoolBall*)pObj, this);

#ifdef _DEBUG
std::cout << "Cue ball hit bomb.. will explode soon!\n";
#endif

    if (GetState() == UNKNOWN)
    {
      // Get ID of current player, so if we blow up while game is still
      // active, that player Loses.

      // Set state to 'ticking' immediately
////      SetState(DETONATING);
      m_isDetonating = true;

      m_tickTimer = 100.0f; // so we get a tick sound immediately
      // Reset values for flashing
      m_flashTimer = 0;
      m_flashColour = true;
      m_detonateTimer = 0; // total time from DETONATING to DETONATE

      // Set time, blow up after some time delay.
      // TODO Should this be object data, so balls can explode at different
      // times ? Probably not requires, and would be confusing/arbitrary ?
      float timeDelay = Engine::Instance()->GetConfigFloat("pool-bomb-time");
      float timeDelay2 = timeDelay + 20.0f; // TODO CONFIG

      Engine::Instance()->SendGameMessage(
        new Message("bomb detonate", timeDelay, GetId(), GetId(), DETONATE));
      Engine::Instance()->SendGameMessage(
        new Message("bomb detonate", timeDelay2, GetId(), GetId(), OUT_OF_PLAY));
    }
  }
}

void PoolExplBall::CullDraw(Frustum* pf)
{
  PoolBall::CullDraw(pf);
  m_blastWave.Draw();
  m_explosion.Draw();
}

void PoolExplBall::Draw()
{
  if (GetState() != UNKNOWN)
  {
    return;
  }

  if (m_isDetonating)
  {
    // Bomb flashes, with the rate increasing as time increases.
    float dt = Engine::Instance()->GetDeltaTime();
    m_detonateTimer += dt;
    static const float maxDelay = Engine::Instance()->GetConfigFloat("pool-bomb-time");
    float flashMax = (1.0f - m_detonateTimer / maxDelay);
    if (flashMax < 0)
    {
      flashMax = 0;
    }
    Assert(flashMax <= 1.0f);
    static const float FLASH1 = Engine::Instance()->GetConfigFloat("pool-bomb-fl1");
    static const float FLASH2 = Engine::Instance()->GetConfigFloat("pool-bomb-fl2");
    flashMax = FLASH1 * flashMax + FLASH2; 
    
    m_flashTimer += dt;
    if (m_flashTimer > flashMax)
    {
      m_flashTimer = 0;
      m_flashColour = !m_flashColour;
    }
 
    // Flash red ?
    if (m_flashColour)
    {
      Engine::Instance()->PushColour(1.0f, 0, 0, 1.0f);  
    }
    PoolBall::Draw();
    if (m_flashColour)
    {
      Engine::Instance()->PopColour();
    }
  }
  else
  {
    PoolBall::Draw();
  }

  // Draw blast wave, explosion effect ? In CullDraw, so we don't cull
  // the effect.
}

bool PoolExplBall::Load(File* pf)
{
  if (!PoolBall::Load(pf))
  {
    return false;
  }

  // Load time from collision to explosion

  // Load blast wave
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

  m_explosion.Load("bombexplode.txt"); // TODO CONFIG

  return true;
}

void PoolExplBall::Update()
{
  PoolBall::Update();
  if (m_isDetonating)
  {
    m_tickTimer += Engine::Instance()->GetDeltaTime();
    static const float TICK_MAX = 1.0f; // TODO CONFIG
    if (m_tickTimer > TICK_MAX)
    {
      m_tickTimer = 0;
      Engine::Instance()->PlayWav("golf-tick.wav"); // TODO CONFIG
    }
  }
}

void PoolExplBall::DrawShadow() const
{
  if (GetState() != UNKNOWN && GetState() != DETONATING)
  {
    return;
  }

  // NB This relies on us calling RefreshHeightServer().
  const BoundingSphere* bs = GetBoundingSphere();
  if (!bs)
  {
    return;
  }

  // Get coords for shadow.
  const float x = bs->x();
  const float y = bs->y();
  const float z = bs->z();

  m_pShadow->Draw(x, y, z, GetShadowSize(), m_heightServerWithPlatforms);
}
}


