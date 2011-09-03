/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolCue.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.10  2005/08/06 10:38:31  Administrator
Improved Birds Eye playability

Revision 1.1.2.9  2005/07/17 23:05:56  jay
Configurable sensitivity

Revision 1.1.2.8  2005/07/09 10:43:14  Administrator
Added flag for 'auto take shot'. When this is set the user no longer
controls the cue swing vel.

Revision 1.1.2.7  2005/07/09 05:54:12  jay
Set max cue position

Revision 1.1.2.6  2005/07/05 09:39:32  jay
Less debug output

Revision 1.1.2.5  2005/06/13 22:13:43  jay
"Pinball" control - pull back and release

Revision 1.1.2.4  2005/06/04 22:37:39  jay
Debug output change only

Revision 1.1.2.3  2005/05/24 20:44:55  jay
Return value from SetContactPos so we know if cue intersected anything.

Revision 1.1.2.2  2005/05/15 17:22:03  jay
Allow user to swing cue using mouse; show cue contact pos;
detect collision with table or balls (ongoing)

Revision 1.1.2.1  2005/05/08 08:25:28  jay
Show user the cue position

*/

#include <iostream>
#include "Common.h"
#include "PoolCue.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "PoolGameState.h"
#include "LevelServer.h"
#include "AngleCompare.h"
#include "EngineStatePoolBase.h"

namespace Amju
{
static const float MAX_SWING_POS = 5.0f; // TODO TEMP TEST

PoolCue::PoolCue()
{
  Reset();
}

float PoolCue::GetSwingPower() const
{
  float f = m_swingPos / MAX_SWING_POS;
  if (f < 0)
  {
    f = 0;
  }
  if (f > 1.0f)
  {
    f = 1.0f;
  }
  return f;
}

void PoolCue::Reset()
{
  m_swingMode = false;
  m_swingVel = 0;
  m_oldy = 0;
  m_hitBall = false;
  m_swingPos = 0;
  m_x = 0;
  m_y = 0;
  m_autoTakeShot = false;
}

void PoolCue::ResetSwingPos()
{
  m_swingPos = 0;
  m_swingVel = 0;
  m_autoTakeShot = false;
}

bool PoolCue::SetContactPos(float x, float y)
{
  float oldx = m_x;
  float oldy = m_y;
  m_x = x;
  m_y = y;
  if (Intersects())
  {
    m_x = oldx;
    m_y = oldy;
    return false;
  }
  return true;
}

bool PoolCue::Intersects()
{
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  // Get end points of cue
  m_v1 = GetBall()->GetOrientation()->GetVertex();
  // Offset this end of the cue to the cue contact pos.
  // TODO

  float yr = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke.m_yRot;
  m_v2 = m_v1;

  // Get endpoint of cue - use elev and yr.
  // NB Length is not critical
  float len = 20.0f;
  // Get (x, z);
  m_v2.x -= len * sin(DegToRad(yr));
  m_v2.z -= len * cos(DegToRad(yr));
  float elev = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke.m_cueElevationDegs;
  m_v2.y += len * sin(DegToRad(elev));

  // Radius of pool cue capsule
  float radius = 0.2f; // A rough radius of the cue section 
  bool intersectsScene = pLevel->GetScene()->LineIntersects(m_v1, m_v2, radius);
  if (intersectsScene)
  {
    return true;
  }

  // Test ball intersection - ignore cue ball
  int levelId = pLevel->GetId();
  int roomId = pLevel->GetRoomId();
  GameObjectMap& objs = Engine::Instance()->GetGameObjects(levelId, roomId);
  // Iterate through map of Game Objects.
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    PPoolGameObject pGo = it->second;
    PoolBall* pBall = dynamic_cast<PoolBall*>(pGo.GetPtr());
    if (pBall && !IsCueBall(pBall))
    {
      // Test ball-capsule intersection
      if (pBall->GetBoundingSphere()->Intersects(m_v1, m_v2, radius))
      {
#ifdef CUE_DEBUG
std::cout << "CUE INTERSECTS BALL: " << pBall->GetId() << "\n";
#endif
        return true;
      }
    }
  }
  return false;
}

void PoolCue::Update()
{
  float dt = Engine::Instance()->GetDeltaTime();

  // If cue intersects table or a ball, raise the elevation angle.
  // TODO Also left/right cue contact pos
  // TODO Limit cue contact pos

  // Only check for cue intersection if it has moved.
  // TODO

  // Don't do this in birds eye mode. It makes it impossible to aim.

  if (!EngineStatePoolBase::IsBirdsEye() && Intersects())
  {
#ifdef CUE_DEBUG
std::cout << "POOL CUE INTERSECTS!!\n";
#endif

    float elev = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke.m_cueElevationDegs;
    static const float CUE_RAISE_VEL = Engine::Instance()->GetConfigFloat("pool_cue_raise_vel");
    Assert(CUE_RAISE_VEL > 0);
    elev += dt * CUE_RAISE_VEL;
   
    static const float MAX_ELEV = Engine::Instance()->GetConfigFloat(
      "pool_max_elev");
 
    if (elev > MAX_ELEV)
    {
      elev = MAX_ELEV; 
    }
    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke.m_cueElevationDegs = elev;
  }

  if (!m_swingMode)
  {
    return;
  }

  m_swingPos += dt * m_swingVel;
  if (m_swingPos > MAX_SWING_POS)
  {
    m_swingPos = MAX_SWING_POS;
  }
  if (m_swingPos < 0 && !m_autoTakeShot)
  {
    m_swingPos = 0;
  }
  // Stop cue from moving 
  if (!m_autoTakeShot)
  {
    m_swingVel = 0; 
  }

#ifdef CUE_DEBUG
std::cout << "CUE: Swing pos: " << m_swingPos << "\n";
#endif

  // Stops at end points
  // Max forward position - also check for table hit.
  // TODO

  // If the positon passes some point, the cue ball has been hit.
  if (m_swingPos < -2.0f)
  {
    Assert(m_autoTakeShot);
    m_hitBall = true;
    m_swingMode = false; // prevent m_swingVel being overwritten
  }
}

void PoolCue::SetSwingVel(float v)
{
  m_swingVel = v;
}

float PoolCue::GetSwingVel()
{
#ifdef CUE_DEBUG
std::cout << "POOL CUE: Swing vel: " << m_swingVel << "\n";
#endif

  return m_swingVel;
}

bool PoolCue::HitsCueBall()
{
  return m_hitBall;
}

void PoolCue::MousePos(int x, int y)
{
  int ydiff = y - m_oldy;
  m_oldy = y;
  if (!m_swingMode)
  {
    return;
  }
  if (m_autoTakeShot)
  {
    return;
  }

  static const float SWING_SENSITIVITY = Engine::Instance()->GetConfigFloat(
    "pool_swing_sensitivity");
  m_swingVel = (float)ydiff * SWING_SENSITIVITY;
  // Don't let the user swing the cue too far forward. 
  // What happens is the user pulls back the cue to set the power
  // and then releases the cue.
 
#ifdef CUE_DEBUG
std::cout << "CUE: Setting m_swingVel: new val: " << m_swingVel << "\n";
#endif
}

bool PoolCue::IsSwingMode() const
{
  return m_swingMode;
}

void PoolCue::SetSwingMode(bool b)
{
  if (m_hitBall)
  {
    // Player has hit the cue ball, so now cannot change this flag
    // until a reset.
    return;
  }

  m_swingMode = b;
  m_swingVel = 0; // ??

#ifdef CUE_DEBUG
if (b)
{
  std::cout << "POOL CUE: Swing mode ON\n";
}
else
{
  std::cout << "POOL CUE: Swing mode OFF\n";
}
#endif
}

void PoolCue::Draw()
{
#ifdef CUE_DEBUG
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex3f(m_v1.x, m_v1.y, m_v1.z);
  glVertex3f(m_v2.x, m_v2.y, m_v2.z);
  glEnd();
  return;
#endif 
 
  // Position cue - take into account
  // - ball pos
  // - Y-rot of player; i.e. the shot angle
  // - cue elevation; i.e. masse
  // - cue contact pos 

  // Find ball centre.
  Orientation o = *(GetBall()->GetOrientation());
  // Move the centre depending on draw/roll/english

  // Find player shot direction.
  float yr = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke.m_yRot;
  float elev = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_golfStroke.m_cueElevationDegs;

#ifdef CUE_DEBUG
std::cout << "POOL CUE: Elev: " << elev << " degs\n";
#endif

  // If in birds eye mode, move the cue up a bit so it doesn't intersect
  // the table.
  if (EngineStatePoolBase::IsBirdsEye())
  {
    o.SetY(o.GetY() + 2.0f);
  }

  AmjuGL::PushMatrix();
  o.SetZRot(elev); 
  o.SetYRot(yr + 90.0f);
  o.Draw();
  // Set contact pos
  AmjuGL::Translate(0, m_y, m_x); 
  // Swing cue forward or back
  AmjuGL::Translate(m_swingPos, 0, 0); 
  m_pSolid->Draw();
  AmjuGL::PopMatrix();
}

bool PoolCue::Load()
{
  // Get cue solid file name
  std::string s = Engine::Instance()->GetConfigValue("pool_cue");
  m_pSolid = SolidComponent::LoadSolid(s);
  if (!m_pSolid.GetPtr())
  {
    return false;
  }
  return true;
}

void PoolCue::SetAutoTakeShot(bool b)
{
  m_autoTakeShot = b;
}

bool PoolCue::GetAutoTakeShot() const
{
  return m_autoTakeShot;
}
}


