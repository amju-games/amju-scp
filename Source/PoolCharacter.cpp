/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolCharacter.cpp,v $
Revision 1.1.2.5  2007/12/13 09:31:06  jay
Fix so shadows are drawn under both characters when waiting for online opponent

Revision 1.1.2.4  2007/12/12 10:24:18  jay
Increase shadow offset for characters

Revision 1.1.2.3  2007/12/10 22:49:56  jay
Player shadow size

Revision 1.1.2.2  2007/12/10 00:08:22  jay
Add character shadows - not working ?!

Revision 1.1.2.1  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:34  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.9  2005/08/26 21:42:08  jay
Turn off debug output

Revision 1.1.8.8  2005/07/30 12:20:09  jay
Distace from cue tip to body is now per-character so each character
can accurately intersect table

Revision 1.1.8.7  2005/07/05 09:38:00  jay
Still trying to get characters to lean over table

Revision 1.1.8.6  2005/06/22 21:40:59  jay
Try to fix table intersect

Revision 1.1.8.5  2005/06/19 21:32:24  jay
Try to detect player intersection with table; draw animation accordingly.

Revision 1.1.8.4  2005/06/13 22:15:59  jay
Integrate Pool animations

Revision 1.1.8.3  2005/05/15 17:17:13  jay
Added trasparency setting

Revision 1.1.8.2  2005/05/08 17:26:28  jay
Changes to integrate pool animations - not finished

Revision 1.1.8.1  2005/04/17 22:03:39  jay
Hide characters until properl implemented

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "PoolCharacter.h"
#include "Character.h"
#include "Engine.h"
#include "PoolGameState.h"
#include "PoolMisc.h"
#include "LevelServer.h"
#include "AngleCompare.h"

namespace Amju
{
PoolCharacter::PoolCharacter()
{
  m_swish = false;
  m_swishtime = 0;

  // TODO TEMP TEST
  // Radius for player characters not set elsewhere ??!
  m_radius = 1.0f;
  m_alpha = 1.0f;

  m_distCueBody = 0;
  
  // Shadow y-offset is higher than for pool balls, as shadow is further from
  // camera
  m_pShadow->SetYOffset(1.0f);
}

void PoolCharacter::DrawShadow() const
{
  // If taking the shot, the origin in centred on the ball - don't draw the
  // shadow. If idle (or walking ?) it's ok to draw the shadow as usual.

  // TODO
  if (m_alpha == 0)
  {
    // Character is invisible
    return;
  }
 
  // Don't draw shadow if player is not standing
  // TODO Draw shadow, but move so under player legs 
  int a = m_meshes[0]->GetAction("chalk"); // POOL  
  int current = (*m_meshes.begin())->GetAction();
  if (current != a)
  {
    return;
  }

  const float x = GetPosition().x;
  const float y = -9.9f; // ground height is -10
  const float z = GetPosition().z;

  m_pShadow->Draw(x, y, z, GetShadowSize(), m_heightServerWithPlatforms);
}

void PoolCharacter::SetAlpha(float a)
{
  m_alpha = a;
}

//#define SHOW_SPHERES_DEBUG

void PoolCharacter::Draw()
{
  if (m_alpha == 0)
  {
    // Character is invisible
    return;
  }

#ifdef SHOW_SPHERES_DEBUG
  static bool d = BoundingSphere::GetDraw();
  BoundingSphere::SetDraw(true);
  m_tableBs1.Draw();
  m_tableBs2.Draw();
  // Draw line between centres
  Vec3f v1 = m_tableBs1.GetCentre();
  Vec3f v2 = m_tableBs2.GetCentre();
  glBegin(GL_LINES);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
  glEnd();
  BoundingSphere::SetDraw(d);
#endif

  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, m_alpha);
  CharacterGameObject::Draw();
  Engine::Instance()->PopColour();
}

void PoolCharacter::DrawSwish()
{
  if (m_swish)
  {
    m_pSwish->Update(); // TODO
    AmjuGL::PushMatrix();
    GetOrientation()->Draw();
    m_pSwish->Draw();
    AmjuGL::PopMatrix();

    m_swishtime += Engine::Instance()->GetDeltaTime();
    static const float swishmax = Engine::Instance()->GetConfigFloat("golf_swish_max");
    if (m_swishtime > swishmax)
    {
      m_swish = false;
    }
  }
}

bool PoolCharacter::Load(File* pf)
{
  if (!CharacterGameObject::Load(pf))
  {
    return false;
  }
#ifdef CHARACTER_DEBUG
std::cout << "GOLF CHARACTER: File: " << pf->GetName().c_str() << "\n";
#endif

  // POOL v.1.1
  // Load distance of body from cue tip. This is used to check if
  // player intersects table. 
  if (!pf->GetFloat(&m_distCueBody))
  {
    pf->ReportError("Failed to load distance from cue tip to body.");
    return false;
  }

  // Load swish effect
  std::string swish = Engine::Instance()->GetConfigValue("golf_char_swish");
  m_pSwish = SolidComponent::LoadSolid(swish);
  if (!m_pSwish.GetPtr())
  {
    pf->ReportError("Bad swish effect.");
    return false;
  }
  
  m_radius = Engine::Instance()->GetConfigFloat("pool_char_radius"); 

  SetShadowSize(10.0f);

  return true;
}

void PoolCharacter::SetActionGo()
{
#ifdef CHARACTER_DEBUG
std::cout << "SET ACTION GO\n";
#endif

  Assert(m_meshes.size() > 0);

  int a = m_meshes[0]->GetAction("setupshot"); // POOL  

  // Test if the player intersects the table. If so, we use the 'lean'
  // animation.
  Level* pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  Vec3f v1 = GetOrientation()->GetVertex(); 

  // Set the y-coord so that we will intersect the table cushions.
  v1.y = 1.0f;

  // The character origin is at centred the cue tip, not on the character! 
  // Compensate - move away from the cue tip, by going in the opposite 
  // direction to the direction the player is facing.
  float yRotate = GetOrientation()->GetYRot(); 
  float dx = sin(DegToRad(yRotate));
  float dz = cos(DegToRad(yRotate));
    
  v1.x -= dx * m_distCueBody; //DIST_MULT;
  v1.z -= dz * m_distCueBody; //DIST_MULT;

  // Also we should move to the side, because the character is to the side of
  // the cue, which lies on the centre line.
  yRotate += 90.0f; // + or - ?
  float sidex = sin((double)yRotate / 180.0 * pi);
  float sidez = cos((double)yRotate / 180.0 * pi);
  static const float DIST_MULT2 = Engine::Instance()->GetConfigFloat(
    "pool_char_dist_mult2");
  v1.x += sidex * DIST_MULT2;
  v1.z += sidez * DIST_MULT2;

  // Move away more - so we don't have the situation where the player is so
  // deep in the table that we don't spot the intersect!
  Vec3f v2(v1);
  static const float FAR_DIST = 50.0f;
  v2.x -= dx * FAR_DIST;
  v2.z -= dz * FAR_DIST;
  
  // Get a radius for our capsule - don't use bounding sphere radius, as
  // we need to make that quite big so characters are not culled too soon.
  static const float r = Engine::Instance()->GetConfigFloat(
    "char_table_cap_r");

  // Store so we can draw this
//  // TODO TEMP TEST - char positon not set yet ??
//  Vec3f v3 = GetOrientation()->GetVertex(); 
  m_tableBs1.SetCentre(v1); 
  m_tableBs1.SetRadius(r);
  m_tableBs2.SetCentre(v2); 
  m_tableBs2.SetRadius(r);

  bool hitTable = pLevel->GetScene()->LineIntersects(v1, v2, r);
  if (hitTable)
  {
    a = m_meshes[0]->GetAction("tableleftshot"); // POOL  
  }

  for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
  {
    if ((*it)->GetAction() == a)
    {
      //Assert(0); // shouldn't already be swinging/putting!
      return;
    }

    (*it)->SetAction(a);
  }
}

void PoolCharacter::SetActionReady()
{
  Assert(m_meshes.size() > 0);
/*
  // TODO should be static but depends on MD2 data !?!
  const int swing_shuffle = m_meshes[0]->GetAction("swing_shuffle");
  const int putt_shuffle = m_meshes[0]->GetAction("putt_shuffle");

  int current = (*m_meshes.begin())->GetAction();
  if (current == swing_shuffle || current == putt_shuffle)
  {
    // Wait for shuffle to end
    return;
  }

  int a = 0;
  if (HighPower())
  {
    a = m_meshes[0]->GetAction("setupshot"); // POOL  swing_ready");
  }
  else
  {
    a = m_meshes[0]->GetAction("setupshot"); // POOL  golf_ready");
  }
*/

  int a = m_meshes[0]->GetAction("chalk"); // POOL  
  
  int current = (*m_meshes.begin())->GetAction();
  if (current == a)
  {
    return;
  }

  for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
  {
    if ((*it)->GetAction() == a)
    {
      return;
    }
    (*it)->SetAction(a);
  }
}

void PoolCharacter::SetActionPoolShuffle()
{
/*
  Assert(m_meshes.size() > 0);

  int a = 0;
  if (HighPower())
  {
    a = m_meshes[0]->GetAction("swing_shuffle");
  }
  else
  {
    a = m_meshes[0]->GetAction("putt_shuffle");
  }

  for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
  {
    if ((*it)->GetAction() == a)
    {
      return;
    }
    (*it)->SetAction(a);
  }
*/

  // TODO Animation for shuffling around table - or walk WITH CUE.
  int a = m_meshes[0]->GetAction("walk");
  for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
  {
    if ((*it)->GetAction() == a)
    {
      return;
    }
    (*it)->SetAction(a);
  }
}

void PoolCharacter::OnNoMove()
{
/*
  // Multi player:
  // If not the active player, simply perform Idle action.
  if (this != GetActivePlayer())
  {
    CharacterGameObject::OnNoMove();
    return;
  }
*/

  Assert(m_meshes.size() > 0);
  // Don't change animation if we are swingin or putting.
  // TOD should be static but depends on MD2 model ??
  const int swing = m_meshes[0]->GetAction("setupshot"); // POOL  swing_finish");
  const int putt = m_meshes[0]->GetAction("setupshot"); // POOL  putt");

  int current = (*m_meshes.begin())->GetAction();
  if (current == swing || current == putt)
  {
    return;
  }

  // Go to golf ready animation.
  SetActionReady();
}

void PoolCharacter::OnArcMove()
{
  // Go to golf shuffle animation.
  SetActionPoolShuffle();
}

}

