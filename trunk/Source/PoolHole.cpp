/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolHole.cpp,v $
Revision 1.1.2.3  2007/06/10 08:56:40  jay
No Shadow required

Revision 1.1.2.2  2007/03/21 10:10:48  jay
Don't show or award bonus hearts if "show path" is on. This is to try to
prevent cheating on the high score table.. but you can still line up the
shot then turn "show path" off :-(

Revision 1.1.2.1  2006/08/14 17:50:23  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:34  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.17  2005/10/01 22:04:29  jay
Increase angle for hiding bonuses

Revision 1.1.8.16  2005/09/29 19:06:43  jay
Don't draw almost-invisible hearts at all.

Revision 1.1.8.15  2005/09/06 19:59:54  jay
Turn off debug output

Revision 1.1.8.14  2005/08/31 22:43:35  Administrator
Added number of hearts above each Pocket

Revision 1.1.8.13  2005/08/31 17:30:18  jay
Allow calls to create/add bonuses

Revision 1.1.8.12  2005/08/26 21:45:16  jay
Added bonuses

Revision 1.1.8.11  2005/08/06 10:39:36  Administrator
Minor config value warning fixed

Revision 1.1.8.10  2005/08/06 09:07:44  jay
Bonus stuff for v.1.2

Revision 1.1.8.9  2005/08/03 22:32:16  Administrator
Added radius property for Scene Editor

Revision 1.1.8.8  2005/08/03 21:05:24  Administrator
Save/Load radius per object

Revision 1.1.8.7  2005/08/02 18:31:26  jay
Bah, no bonuses this release.

Revision 1.1.8.6  2005/07/30 23:35:03  jay
Re-enable bonuses

Revision 1.1.8.5  2005/07/05 09:38:35  jay
Added bonuses - disabled for now

Revision 1.1.8.4  2005/06/04 22:33:06  jay
Don't draw arrows

Revision 1.1.8.3  2005/04/17 22:02:20  jay
Added API for nominating a hole

Revision 1.1.8.2  2005/03/31 22:34:10  jay
Pool: we are reusing PoolHoles as Pool Pockets. Don't need to draw shadows.

Revision 1.1.8.1  2005/03/17 20:54:39  jay
Check for collision with PoolBall

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolHole.h"
#include "File.h"
#include "PoolMisc.h"
#include "Texture.h"
#include "TextureServer.h"
#include "Engine.h"
#include "AngleCompare.h"
#include "FreeMovingSolidGameObject.h"
#include "PoolBall.h"
#include "PoolPlayerStats.h"
#include "PoolGameState.h"
#include "EngineStatePoolBase.h"
#include "Mgc/MgcDist3DLinLin.h"
#include "Mgc/MgcDist3DVecLin.h"
#include "EngineStatePoolSetUpShot.h"

namespace Amju
{
static bool ShowTrajectory()
{
  EngineStatePoolSetUpShot* e = dynamic_cast<EngineStatePoolSetUpShot*>(
    Engine::Instance()->GetEngineState(EngineStatePoolSetUpShot::Name).GetPtr());
  if (e)
  {
    return e->GetShowTrajectory();
  }
  return true; // ?
}

PoolHole::PoolHole()
{
#if defined(SCENE_EDITOR)
  // TODO CONFIG
  PSolidComponent pSolid = SolidComponent::LoadSolid("gflag1.comp");
  SetSolid(pSolid);
#endif

  // POOL - we want to draw bonuses above the pockets, and fade them
  //  to translucent if they obscure the table.
  m_isTranslucent = true; 
  m_isNominated = false;
  m_numBonuses = 1;
}
  
const char* PoolHole::GetTypeName() const
{
  return "golf_hole";
}

void PoolHole::Draw()
{
  // NB POOL: no shadow

#if defined(SCENE_EDITOR)
  static const float radius = 2.0f;
    // NB Was:  Engine::Instance()->GetConfigFloat("golf_hole_radius");
  SetShadowSize(radius); 
#endif
}

void PoolHole::CullDraw(Frustum* pf)
{
  DrawBonuses();

  AmjuGL::PushMatrix();
  m_pSolid->GetOrientation()->Draw();
  AmjuGL::Translate(0, 8.0f, 0); // TODO CONFIG - depends on height of bonuses
  m_explosion.Draw();
  AmjuGL::PopMatrix();

  SolidGameObject::CullDraw(pf);
}

void PoolHole::DrawShadow() const
{
}

void PoolHole::CreateShadow()
{
}

void PoolHole::SetOrientation(const Orientation& o)
{
  BoundingSphere bs;
  bs.SetCentre(o.GetVertex());
  bs.SetRadius(m_target.GetBoundingSphere()->GetRadius()); 

  m_target.SetSphere(bs);
  SolidGameObject::SetOrientation(o);
}

const BoundingSphere* PoolHole::GetBoundingSphere() const
{
  return m_target.GetBoundingSphere();
}

void PoolHole::CreateBoundingSphere(const Matrix& cm)
{
}

bool PoolHole::Load(File* pf)
{
  float x, y, z;
  if (!pf->GetFloat(&x) || !pf->GetFloat(&y) || !pf->GetFloat(&z))
  {
    pf->ReportError("Failed to load golf hole vertex.");
    return false;
  }  
  float radius = 0;
  if (!pf->GetFloat(&radius))
  {
    pf->ReportError("Failed to load golf hole (pocket) radius.");
	return false;
  }

  if (!pf->GetInteger(&m_numBonuses))
  {
	pf->ReportError("Failed to get number of hearts.");
	return false;
  }

  PSolidComponent pSolid = SolidComponent::LoadSolid("gflag1.comp"); // TODO CONFIG 
  Orientation o;
  o.SetX(x);
  o.SetY(y);
  o.SetZ(z);
  pSolid->SetOrientation(o);
  SetSolid(pSolid);

  // The target is just below the top centre of the solid.
  BoundingSphere bs;
  // Assume height of hole solid is 1.0
  VertexBase v(x, y, z);
  bs.SetCentre(v);

  bs.SetRadius(radius); 
  m_target.SetSphere(bs);

  SetShadowSize(radius); 
  CreateShadow();
  // It looks like this should be called on every object once a room has an object
  // added or deleted.
  RefreshShadow();

  // Explosion Effect for Bonuses
  std::string effect = Engine::Instance()->GetConfigValue("pool_bonus_explode");
  if (!m_explosion.Load(effect))
  {
    return false;
  }

  return true;
}

void PoolHole::HandleObjectCollision(GameObject* pObj)
{
  if (!pObj)
  {
    return;
  }

  // POOL
  // The idea here is that PoolBall::HandleObjectCollision handles
  // hole-ball collision. So let the ball code deal with it.
  // The CollisionTest code only tests each pair of objects once, so
  // this doesn't cause the collision to be handled twice.
  if (dynamic_cast<PoolBall*>(pObj))
  {
    pObj->HandleObjectCollision(this);
  }
}

VertexBase PoolHole::GetTargetVertex() const
{
  return m_target.GetBoundingSphere()->GetCentre();
}

void PoolHole::OnRoomEntry()
{
  m_isNominated = false;
  SolidGameObject::OnRoomEntry();

  // POOL
  CreateBonuses();
}

void PoolHole::CreateBonuses()
{
  m_bonuses.clear();
  CreateExtraBonuses();
}

void PoolHole::CreateExtraBonuses()
{
  // No bonuses if this is a practice game 
  if (Engine::Instance()->GetGameState()->IsFriendly())
  {
    return;
  }

  PoolBonus b;
  // Get list of bonus names  e.g. melon, heart, star, etc.
  static bool gotList = false;
  static std::vector<std::string> bonuses;
  static int bonusIndex = 0;
  if (!gotList)
  {
    std::string bonusFile = Engine::Instance()->GetConfigValue("bonus_list");
    File f;
    if (!f.OpenRead(bonusFile))
    {
      f.ReportError("Failed to open bonuses file.");
      return;
    }
    int numBonuses = 0;
    if (!f.GetInteger(&numBonuses))
    {
      f.ReportError("Expected no of bonus types.");
      return;
    }
    for (int i = 0; i < numBonuses; i++)
    {
      std::string s;
      if (!f.GetDataLine(&s))
      {
        f.ReportError("Expected bonus name.");
        return;
      }
      bonuses.push_back(s);
    }
    gotList = true;
  }

  for (int i = 0; i < m_numBonuses; i++)
  {
    PoolBonus b;
    std::string s = bonuses[bonusIndex];
    ++bonusIndex;
    if (bonusIndex == (int)bonuses.size())
    {
      bonusIndex = 0;
    }
    
    b.m_pComp = SolidComponent::LoadSolid(s);
    m_bonuses.push_back(b);
  }

  m_getBonuses = false;
}

void PoolHole::PlayerGetsBonuses()
{
  // No bonus if path is on
  if (ShowTrajectory())
  {
    return;
  }

  // Make the current bonus explode.
  // When the bottom bonus finishes exploding, it disappears, and any 
  // bonuses above it drop down.
  m_getBonuses = true;
  m_fallVel = 0;
  m_bonusTime = 100.0f; // so first bonus goes off immediately
  m_fall = 0;
}

void PoolHole::Update()
{
  // Update the falling bonuses
  float dt = Engine::Instance()->GetDeltaTime();

  if (m_getBonuses && !m_bonuses.empty())
  {
    m_bonusTime += dt; 
    if (m_bonusTime > 1.0f) // TODO CONFIG
    {
      m_bonusTime = 0;
      // Start a new explosion effect
      m_explosion.Reset(m_bonuses[0].m_pComp->GetOrientation()->GetVertex());
      // Remove this bonus
      m_bonuses.erase(m_bonuses.begin()); //pop_front();

#ifdef GET_BONUS_DEBUG      
std::cout << "PLAYER GETS BONUS\n";
#endif

      PlayerStats* p = Engine::Instance()->GetGameState()->
        GetCurrentPlayerInfo()->GetPlayerStats();
  
      PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
      if (pp)
      {
        // Add bonus of the given type
        // TODO different types - currently type is always 1.
        pp->AddBonus(1);
        // Update HUD
        EngineStatePoolBase::CreateBonusesLeft();
        Engine::Instance()->PlayWav("bonus_points.wav");
      } 

      m_fall = 0;
      m_fallVel = 0;
    }
    m_fall += dt * m_fallVel; 
    static const float ACCEL = Engine::Instance()->GetConfigFloat(
      "pool_bonus_grav");
    m_fallVel += dt * ACCEL;
    if (m_fall > 2.0f) // TODO CONFIG
    {
      m_fall = 2.0f;
      m_bonusTime = 100.0f;
    }
  }

  // Update transparency: get the distance between two lines.
  // Line1 is from cue ball to camera. Line2 is a vertical line above
  // this pocket, i.e. where the bonuses are. 
  // If the distance between the lines is lower than some limit,
  // opacity is proportional to the distance.
  VertexBase v1 = GetBall()->GetBoundingSphere()->GetCentre();
  VertexBase v2 = EngineStatePoolBase::SGetCamera()->
    GetOrientation()->GetVertex();
  Mgc::Segment3 line1;
  line1.Origin() = Mgc::Vector3(v1.x, v1.y, v1.z);
  float d1x = v2.x - v1.x;
  float d1y = v2.y - v1.y;
  float d1z = v2.z - v1.z;
  line1.Direction() = Mgc::Vector3(d1x, d1y, d1z);

  VertexBase v3 = GetBoundingSphere()->GetCentre();
  Mgc::Segment3 line2;
  line2.Origin() = Mgc::Vector3(v3.x, v3.y, v3.z);
  line2.Direction() = Mgc::Vector3(0, 20.0f, 0);

  float dsq = Mgc::SqrDistance(line1, line2);
  float dist = sqrt(dsq);
 
  static const float OPACITY_LIMIT = 20.0f; // TODO CONFIG 
  if (dist < OPACITY_LIMIT)
  {
    m_alpha = dist / OPACITY_LIMIT;
  }
  else
  {
    m_alpha = 1.0f;
  }
}


void PoolHole::DrawBonuses()
{
  // Don't draw bonuses if "show path" is on
  if (ShowTrajectory())
  {
    return;
  }

  // Bonuses fade to transparent if they obscure the table.
  if (m_alpha < 0.25f) // TODO CONFIG
  {
    // Invisible bonuses create weird effects as they 'cut out' bits of the 
    // scene depending on the drawing order. So don't draw if they are (nearly)
    // invisible.
    return;
  }

  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, m_alpha);

  AmjuGL::PushMatrix();
  AmjuGL::Translate(0, 5.0f, 0); // TODO CONFIG
  // Translate to golf hole position
  m_pSolid->GetOrientation()->Draw();
  for (unsigned int i = 0; i < m_bonuses.size(); i++)
  {
    float y = 4.0f + 2.0f * i; // TODO CONFIG
    if (i == 0 && m_getBonuses)
    {
      y -= m_fall; 
    }
    AmjuGL::PushMatrix();
    AmjuGL::Translate(0, y, 0); // TODO CONFIG
    // Translate up for each bonus
    m_bonuses[i].m_pComp->Update();
    m_bonuses[i].m_pComp->Draw();
    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();
  Engine::Instance()->PopColour();
}

#if defined(SCENE_EDITOR)
bool PoolHole::Save(File* pf)
{
  pf->WriteComment("// Hole vertex");
  pf->WriteFloat(m_target.GetBoundingSphere()->GetCentre().x);
  pf->WriteFloat(m_target.GetBoundingSphere()->GetCentre().y);
  pf->WriteFloat(m_target.GetBoundingSphere()->GetCentre().z);
  pf->WriteFloat(m_target.GetBoundingSphere()->GetRadius());
  pf->WriteComment("// Num hearts");
  pf->WriteInteger(m_numBonuses);
  return true;
}

static const std::string RADIUS = "radius";
static const std::string HEARTS = "hearts";

GameObject::PropertyMap PoolHole::GetProperties() const
{
  PropertyMap pm;
  pm[RADIUS] = GameObject::Property(m_target.GetBoundingSphere()->GetRadius());
  pm[HEARTS] = GameObject::Property(m_numBonuses);
  return pm;
}

void PoolHole::SetProperties(const GameObject::PropertyMap& pm)
{
  BoundingSphere bs = *(m_target.GetBoundingSphere());
  bs.SetRadius(const_cast<GameObject::PropertyMap&>(pm)[RADIUS].GetFloat());
  m_target.SetSphere(bs);

  m_numBonuses = const_cast<GameObject::PropertyMap&>(pm)[HEARTS].GetInt();
}
#endif

}

