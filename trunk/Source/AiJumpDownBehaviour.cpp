/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AiJumpDownBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:43:01  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "AiJumpDownBehaviour.h"
#include "Ai.h"
#include "Character.h"
#include "Geometry.h"
#include "AngleCompare.h"
#include "Engine.h"

namespace Amju
{
void AiJumpDownBehaviour::ContinueActivating()
{
  // Try to find a path to the target.
  // If there's a path, follow the path to the target.
  Assert(m_pInnerTarget.GetPtr());

  PGameObject pTemp = m_pTarget; // The piece we want
  State tempState = pTemp->GetState();
  // Temporarily disable the piece, so it won't obstruct the path to its
  // own centre.
  pTemp->SetState(OUT_OF_PLAY);

  // Target held by Behaviour is ref counted, which is exactly what we want
  // for once! The target object will be deleted once we no longer need it.
  SetTarget(m_pInnerTarget.GetPtr());
 
  SearchResult sr = CreatePath();

  // State of piece should be UNKNOWN already.. if it isn't, Update() will fail.
  pTemp->SetState(tempState);
  SetTarget(pTemp);

  if (sr == SearchOk)
  {
    m_isStillActivating = false;
    // TODO must decide whether to go into Collect or RunTo behaviour :-(
    m_pCharacter->SetState(AI_COLLECT);
    return;
  }
  
  if (sr != SearchFail)
  {
    return;
  }
  
  m_isStillActivating = false;

  // No path, we just want to head in a sensible direction. 
  // The rest is handled in Update().

#if defined(_DEBUG)
  std::cout << "Jump state: no path to next target.\n";
#endif
}

void AiJumpDownBehaviour::AvoidObstacles()
{
  SeekBehaviourBase::AvoidObstacles();
}

void AiJumpDownBehaviour::OnActivated()
{
  // Split activation over frames, as this may take some time.
  m_isStillActivating = true;

  // This is set to true when the direction to move forward in has been
  // calculated.
  m_directionIsSet = false;

  // Get the floor level: the y-value we should be standing on.
  // Set this to the next level down from the current character height.
  // Not allowed to go less than zero though.
  m_floor = m_pCharacter->GetOrientation()->GetY();
  m_floor -= 2.0f;
  if (m_floor < 0)
  {
    m_floor = 0;
  }
  m_vel = 1.0f; // TODO CONFIG

  // Choose direction
  // Is there a path to the next target ? The next target could be a piece
  // or a location. Create a target object, which we use in ContinueActivating()
  // to try to find a path.
  // NB Same as in Collect Behaviour.
  if (!m_pInnerTarget.GetPtr())
  {
    m_pInnerTarget = new Target;
  }
  BoundingSphere bs;
  const Orientation* pO = m_pTarget->GetOrientation();
  // TODO For a piece, we have to add 1. For a target we don't. Sort this out!
  bs.SetCentre(VertexBase(pO->GetX(), pO->GetY() + 1.0f, pO->GetZ()));
  bs.SetRadius(1.0f); // TODO needed ?
  m_pInnerTarget->SetSphere(bs);
  
}

void AiJumpDownBehaviour::Update()
{
  if (m_isStillActivating)
  {
    // Not ready yet, must continue to activate.
    // Likely case for this is when we must complete pathfinding.
    ContinueActivating();
    return;
  }

  if (m_pCharacter->IsFalling())
  {
    // Can't do anything until we've stopped falling.
    return;
  }

  if (!m_directionIsSet)
  {
    SetDirection();
  }

  m_pCharacter->SetForwardVel(m_vel);

  if (m_pCharacter->GetPosition().y <= m_floor)
  {
    // We're on the floor.

    //Ai* pAi = dynamic_cast<Ai*>(m_pCharacter);
    //pAi->Succeeded(); 

    m_pCharacter->SetState(AI_COLLECT);
  }
  // As we are not calling the base class version of this function, we must
  // also do obstacle avoidance ourselves.
  AvoidObstacles();
}

void AiJumpDownBehaviour::ReachedTarget()
{
}

void AiJumpDownBehaviour::SetDirection()
{
  // TODO put this in a more accessible function, as it is generally useful
  // for NPCs.


  // Check for obstructions in the direction we want to go.
  // If there's an obstruction, pick another direction.

  // Get angle to face next target
  VertexBase vgoal = m_pTarget->GetBoundingSphere()->GetCentre();
  VertexBase vplayer = m_pCharacter->GetPosition();
  float a = Geometry::GetXZAngle(vgoal, vplayer);
  Assert(!m_pCharacter->IsFalling());

  // Temporarily disable the target so it isn't seen as an obstruction.
  State oldState = m_pTarget->GetState();
  m_pTarget->SetState(OUT_OF_PLAY);

  // List of angles. We iterate over the list, adding each angle to a
  // until we find an unobstructed path.
  float olda = a;
  float b[] = {0, 30, -30, 45, -45, 60, -60, 90, -90, 120, -120, 135, -135, 150, -150, 165, -165, 180 }; 
  static const int numAngles = sizeof(b) / sizeof(float);
  int i = 0;
  do
  {
    a = olda;
    Assert(i < numAngles);
    a += b[i];
    ++i;
  } while (IsObstructed(a) &&  i < numAngles);

  m_pTarget->SetState(oldState);

  if (i == numAngles)
  {
    // Couldn't find an unobstructed path. 
    // This indicates that the AI is stuck in a wall :-(
    NoPath();
    m_directionIsSet = true; // prevent calling this function again
    return;
  }
 
  // Get the difference between the current angle the player is facing
  // and the required angle.
  float current = m_pCharacter->GetOrientation()->GetYRot();
  float diff = AngleCompare::DegDiff(current, a);

#if defined(_DEBUG)
  std::cout << "No obstruction in heading " << a 
            << " degs. Current=" << current << " diff=" << diff << "\n";
#endif

  // TODO smoothly rotate to this angle
  m_pCharacter->RotateY(diff);

  m_directionIsSet = true;
}

bool AiJumpDownBehaviour::IsObstructed(float a)
{
  // Cast a line from the character position in direction a (degrees).
  // If the ray intersects anything, there's an obstruction.
  
  static float raylen = atof(Engine::Instance()->
    GetConfigValue("ai_los_ray_len").c_str());
  
  float rads = DegToRad(a);
  float dx = sin(rads); 
  float dz = cos(rads); 

  VertexBase v(m_pCharacter->GetPosition());
  v.y += 1.0f; // height of ray w.r.t. character origin

  VertexBase v1(v);
  v1.x += dx * raylen;
  v1.z += dz * raylen;

  // Now we have got the endpoints of our ray. Check for intersection
  // with Scene and Game Objects.
  // TODO use CubeMap ?

  // Check the character's Height Server for collisions. 
  // Not the scene, as this won't include any Game Objects.
  const HeightServer* pHs = m_pCharacter->GetHeightServer();
  float capradius = 0.1f; // radius of capsule
  if (pHs->Intersects(v, v1, capradius)  || 
      pHs->Intersects(v1, v, capradius)) 
  {
#if defined(_DEBUG)
    std::cout << "Obstruction at " << a << " degrees.\n";
#endif

    return true;
  }
#if defined(_DEBUG)
    std::cout << "NO Obstruction from " << v << " to " << v1 << "\n";
#endif

  // But wait! Perhaps there's an obstruction which is too far away from the
  // player to register in the HeightServer, but is close enough to intersect
  // the ray.
  // To check this we must test the ray against the static scene.
  // TODO

  return false;
}

void AiJumpDownBehaviour::NoPath()
{
  std::cout << "AI is stuck!\n";
}


}

