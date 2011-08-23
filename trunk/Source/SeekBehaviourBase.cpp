/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SeekBehaviourBase.cpp,v $
Revision 1.1.10.1  2005/05/08 08:18:22  jay
Fixed calls to non-const-returning GameObject::GetOrientation

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "SeekBehaviourBase.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "SchAssert.h"
#include "Engine.h"
#include "AStarPathFinder.h"
#include "Level.h"
#include <iostream>

using namespace std;
namespace Amju
{
SeekBehaviourBase::SeekBehaviourBase() :
  m_isStillActivating(false)
{
}

void SeekBehaviourBase::Draw()
{
#if defined(_DEBUG)
  DrawPath();
#endif
}

void SeekBehaviourBase::Update()
{
  if (m_isStillActivating)
  {
    // Not ready yet, must continue to activate.
    // Likely case for this is when we must complete pathfinding.
    ContinueActivating();
    return;
  }

  // If a path has been set up, head towards the first point. 
  // If no path, just head straight for the target.
  VertexBase v;
  if (m_path.empty())
  {
    v = m_pTarget->GetBoundingSphere()->GetCentre();
  }
  else
  {
    v = m_path.front();
  }
 
  // HeadTowards() alters heading of Character towards vertex arg.
  // Returns distance to vertex on x-z plane.
  if (HeadTowards(v) < 1.0f) // TODO CONFIG
  {
    // We've reached the first point along path to target.

    if (m_path.empty())
    {
      // No more points, so we have reached the target.
      ReachedTarget();
    }
    else
    {
      m_path.pop_front();
    }
  }

  AvoidObstacles();
}

void DrawPathSeg(const VertexBase& v1, const VertexBase& v2)
{
  //glLineWidth(5.0f);
/*
  glBegin(GL_LINES);
  glVertex3f(v1.x, v1.y, v1.z);
  glVertex3f(v2.x, v2.y, v2.z);
  glEnd();
*/
}

void SeekBehaviourBase::DrawPath()
{
  //Engine::Instance()->PushColour(0, 0, 0, 1.0f);
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  if (!m_pCharacter || !m_pTarget.GetPtr())
  {
    return;
  }

  VertexBase begin = m_pCharacter->GetPosition();
  VertexBase end = m_pTarget->GetBoundingSphere()->GetCentre();
  VertexBase current = begin;

  for (PathPoints::iterator it = m_path.begin(); it != m_path.end(); ++it)
  { 
    DrawPathSeg(current, *it);
    current = *it;
  }
  DrawPathSeg(current, end);

  //AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  //Engine::Instance()->PopColour();

}

void SeekBehaviourBase::AddPathPoint(const VertexBase& v)
{
  m_path.push_back(v);
}

float SeekBehaviourBase::HeadTowards(const VertexBase& v)
{
  float yrot = m_pCharacter->GetOrientation()->GetYRot(); // NPC angle

  float xdiff = m_pCharacter->GetOrientation()->GetX() - v.x;

  float zdiff = m_pCharacter->GetOrientation()->GetZ() - v.z;

  // Get angle we _should_ be facing in to seek target.
  float requiredRot = atan2(xdiff, zdiff); // TODO do we need to check if z = 0 ?
  float requiredDegs = requiredRot / pi * 180.0f; // convert to degs
  requiredDegs += 180.0f; // need this to fix z direction 

  // Distance squared to target
  float d = xdiff * xdiff + zdiff * zdiff;

  if (m_isAvoiding)
  {
    // Slow down if avoiding
    m_pCharacter->SetForwardVel(m_vel * 0.5f); 
  }
  else
  {
    // TODO This old code just forces the character to face the required
    // direction. This looks pretty bad.
///*
    m_pCharacter->SetForwardVel(m_vel); 
    // (Hack.. super accurate instead of curving around to face target.)
    m_pCharacter->SetYRotateVel(0); 
    Orientation o = *(m_pCharacter->GetOrientation());
    o.SetYRot(requiredDegs);
    m_pCharacter->SetOrientation(o);

    return d;
//*/
    // TODO try this new code to rotate smoothly.

    // Get the difference between the required direction and the actual 
    // direction the character is facing.
    float actualDegs = m_pCharacter->GetOrientation()->GetYRot();

    // Get the change in direction we need.
    float changeDegs = requiredDegs - actualDegs;

    // But this can be wrong. We want the smallest change. 
    // So if the change is not in the range (-180, 180) subtract 360 degs,
    // or add 360 degs.
    while (changeDegs > 180.0f)
    {
      changeDegs -= 360.0f;
    }
    while (changeDegs < -180.0f)
    {
      changeDegs += 360.0f;
    }
    Assert(changeDegs >= -180.0f);
    Assert(changeDegs <= 180.0f);

    // TODO we might be able to get away with not doing this sqrt.
    float distToTarget = sqrt(d);

    // The rotate vel for the character is proportional to the required change.
    // (Bigger change needs faster rotate vel.)
    // Also inv. proportional to the distance to the target (less time/space to
    // change needs faster vel.)
    float rotateVel = changeDegs;
    float MAGIC_CONSTANT = 100.0f;
    if (distToTarget > SMALLEST)
    {
      float rotateVel = MAGIC_CONSTANT * changeDegs / distToTarget;
    }

    m_pCharacter->SetYRotateVel(rotateVel);

    // Slow down if we're turning
    float velMult = fabs(changeDegs) / 90.0f;
    if (velMult > 1.0f)
    {
      velMult = 1.0f;
    }
    float vel = m_vel * (1.0f - velMult);
    m_pCharacter->SetForwardVel(vel); 
  }

  // Return distance squared to target.
  return d;
}

PSolidComponent SeekBehaviourBase::GetScene()
{
  return m_pCharacter->GetLevel()->GetScene();
}

PVisibilityGraph SeekBehaviourBase::GetVisibilityGraph()
{
  float radius = m_pCharacter->GetBoundingSphere()->GetRadius();
  return m_pCharacter->GetLevel()->GetRoom()->GetVisibilityGraph(radius);
}

AStarPathFinder* SeekBehaviourBase::GetPathFinder()
{
  PVisibilityGraph pVg = GetVisibilityGraph();

  if (!pVg.GetPtr())
  {
    return 0;
  }

  // TDO don't create a new one every time!
  AStarPathFinder* pPf = new AStarPathFinder(pVg);

  // Line of sight tests used by the pathfinder must take into account the width
  // (bounding sphere radius) of the character.
  float radius = m_pCharacter->GetBoundingSphere()->GetRadius();
  pPf->SetLineOfSightWidth(radius);
  return pPf;
}

SearchResult SeekBehaviourBase::CreatePath()
{ 
  // Create a path from the NPC we control to the target.

  SharedPtr<AStarPathFinder> pPf = GetPathFinder();
  if (!pPf.GetPtr())
  {
    return SearchFail;
  }

  VertexBase begin = m_pCharacter->GetPosition();
  // Raise y-value, it's at floor level.
  begin.y += 1.0f;
  VertexBase end = m_pTarget->GetBoundingSphere()->GetCentre();

  // Target height should be configured to be the same height as visibility 
  // points (1.0)
  //end.y += 1.0f;

  SearchResult sr = pPf->FindPath(begin, end);
  if (sr == SearchFail)
  {
    // Can't find a path. Change state ?
#if defined(_DEBUG)
/*
    cout << "CREATEPATH: Can't find path\n";
*/
#endif
    return SearchFail;
  }
  if (sr == SearchPending)
  {
    return SearchPending;
  }

  // Add path points
  m_path.clear();
  for (AStarPathFinder::iterator it = pPf->Begin(); it != pPf->End(); ++it)
  {
    AddPathPoint(*it);
  }

#if defined(_DEBUG)
/*
  cout << "CREATEPATH: created path!\n";
*/
#endif

  return SearchOk;
}

void SeekBehaviourBase::ContinueActivating()
{
  // Override this function if you need to continue the activation process over
  // a number of frames. E.g. a pathfind may take too long for one call.

  // Reset the m_isStillActivating flag once activation is complete.
  m_isStillActivating = false;
}

}
