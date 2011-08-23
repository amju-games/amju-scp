/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AvoidBehaviourBase.cpp,v $
Revision 1.1.10.1  2005/09/29 19:01:00  jay
Turn off debug mgs

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "AvoidBehaviourBase.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "SchAssert.h"
#include "Engine.h"
#include "AStarPathFinder.h"
#include "Level.h"
#include <iostream>

namespace Amju
{
AvoidBehaviourBase::AvoidBehaviourBase() :
  m_lookahead(4.0f),
  m_obstructionRotateVel(45.0f),
  m_isAvoiding(false)
{
}

void AvoidBehaviourBase::Update()
{
  AvoidObstacles();
}

void AvoidBehaviourBase::AvoidObstacles()
{
  // Check line of sight to a point a short distance in front of the Character.
  // If there's an obstruction, set the rotate vel., else set it to zero.
  // NB This will cause the Character to avoid all other things, including
  // the player. By tuning the rotate vel which is set, and the look-ahead distance,
  // we can make Characters react too late to obstructions to avoid them, and so 
  // they have to slide around them. This means we still get collisions with the
  // player etc if required.

//  PSolidComponent pScene = m_pCharacter->GetLevel()->GetScene(); unused

  // Get Character's forward vector. Mult by look-ahead distance.
  float dx, dz;
  // Use pre-calculated forward vector for initial check.
  m_pCharacter->GetForwardVector(&dx, &dz);
  VertexBase v(m_pCharacter->GetPosition());
  v.y += 1.0f;

  VertexBase v1(v);
  v1.x += dx * m_lookahead;
  v1.z += dz * m_lookahead;

  /*
  TOOD Should go in Draw()
#if defined(_DEBUG)
  Engine::Instance()->PushColour(0, 0, 0, 1.0f);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);

  DrawPathSeg(v, v1);

  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
  Engine::Instance()->PopColour();
#endif
  */
  // Check the character's Height Server for collisions. 
  // Not the scene, as this won't include any Game Objects.
  const HeightServer* pHs = m_pCharacter->GetHeightServer();
//  float r = m_pCharacter->GetBoundingSphere()->GetRadius();
  float r = 0.1f; // radius of capsule

  if (!pHs->Intersects(v, v1, r) && 
      !pHs->Intersects(v1, v, r ))
  {
    //m_pCharacter->SetYRotateVel(0);

    if (m_isAvoiding)
    {
      // The obstacle has been sucessfully avoided. Undo any changes made.
      m_pCharacter->SetYRotateVel(0);

/*
      // If we added a new point to the front of the path, remove it.
      m_path.pop_front();
*/
    }

    m_isAvoiding = false;
    return;
  }

  if (m_isAvoiding)
  {
    // We are already doing something about the obstacle.
    return;
  }

#if defined(AVOID_DEBUG)
  std::cout << "AvoidObstacles has detected an obstacle.\n";
#endif

  ReactToObstacle();
}

void AvoidBehaviourBase::ReactToObstacle()
{
  // There's an obstruction, so avoid it.
  // TODO we have to decide whether to rotate left or right. So we need to get new
  // (dx, dz) rotated a bit in each direction, and recheck for obstructions.. then
  // iterate until a non-obstructed line is found. The rotate vel. can then be
  // related to the number of iterations, so a bigger, closer obstacle causes a more
  // pronounced turn.
  // Also we could slow down depending on how close the obstacle is.
  m_isAvoiding = true;
  m_pCharacter->SetYRotateVel(m_obstructionRotateVel);
/*
  // Create a new path point.
  VertexBase v2(m_pCharacter->GetPosition());
  // Get current y-rotation of character. Add some offset to this to get 
*/

}


}

