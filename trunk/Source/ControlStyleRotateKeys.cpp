/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleRotateKeys.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyleRotateKeys.h"
#include "Engine.h"
#include "PieceMover.h"
#include "EnginePaused.h"
#include "Takeable.h"
#include "AiNotifier.h"
#include "Player.h"
#include "EngineRunning.h"

#include "SchAssert.h"

namespace Amju
{
void ControlStyleRotateKeys::Green(bool) {}
void ControlStyleRotateKeys::YellowUp(bool down) {}
void ControlStyleRotateKeys::YellowDown(bool) {}
void ControlStyleRotateKeys::YellowLeft(bool) {}
void ControlStyleRotateKeys::YellowRight(bool) {}


ControlStyleRotateKeys::ControlStyleRotateKeys()
{
  Reset();
}

void ControlStyleRotateKeys::Reset()
{
  m_isWalking = false;
  m_walkTime = 0;
  m_plusTime = 0;
  m_plusFlipTime = 0;
  // z-button timer
  //m_zTime = 0;
  //m_zDown = false;
  m_maxWalkTime = 0;

  m_shuffle = Engine::Instance()->GetConfigFloat("joystick_shuffle");
  m_shuffleBase = m_shuffle;

  m_plusFlipTime = Engine::Instance()->GetConfigFloat("player_flip_time");
  // Get z-timer trigger time.
  // This is the time for which the z-button must be held down before it switches
  // view to first-person.
  m_zTriggerTime = Engine::Instance()->GetConfigFloat("z_trigger_time");
  // Time before player breaks from a walk to a run
  m_maxWalkTime = Engine::Instance()->GetConfigFloat("player_walk_time");

  ControlStyleMovePlayerBase::Reset();
}

void ControlStyleRotateKeys::Red(bool down) 
{
}

void ControlStyleRotateKeys::Blue(bool down) 
{
  DoAction(down);
}

void ControlStyleRotateKeys::PlusUp(bool b) 
{
  Engine::Instance()->GetPlayer()->ResetPieceCanRotate();

  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  if (b)
  {
    // Start at walking speed. Start timer, so we break into a run after a delay.
    Engine::Instance()->GetPlayer()->SetForwardVel(m_walkForwardVel);
    m_isWalking = true;
    m_walkTime = 0; // we start running when this reaches m_maxWalkTime 
  }
  else
  {
    // Slow to stop.
    Engine::Instance()->GetPlayer()->SetForwardAccel(SLOW_TO_STOP);
    m_isWalking = false;
  }
}

void ControlStyleRotateKeys::PlusDown(bool b) 
{
  Engine::Instance()->GetPlayer()->ResetPieceCanRotate();

  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  // Move backwards.
  if (b)
  {
    Engine::Instance()->GetPlayer()->SetForwardVel(-m_walkForwardVel / 2.0f);
    Engine::Instance()->GetPlayer()->SetForwardAccel(0);
    // TODO CONFIG
    m_isWalking = true;
    m_walkTime = 0; 
  }
  else
  {
    // Slow to stop.
    Engine::Instance()->GetPlayer()->SetForwardAccel(SLOW_TO_STOP);
    m_isWalking = false;
  }

  /*
  // Flip player orientation.
  if (!b)
  {
    m_flippedY = false;
    return;
  }

  if (!m_flippedY)
  {
    Orientation* pO = Engine::Instance()->GetPlayer()->GetOrientation();
    float yRot = pO->GetYRot();
    yRot += 180.0f;
    pO->SetYRot(yRot);
    Engine::Instance()->GetPlayer()->SetOrientation(*pO);
    // TODO problem here: flipping may stick the player b.sphere into a wall!
    //ExtractPlayerFromWall();

    m_flippedY = true;
  }
  */
}

void ControlStyleRotateKeys::PlusLeft(bool b) 
{
  Engine::Instance()->GetPlayer()->ResetPieceCanRotate();

  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  // If player is moving forward, rotate. 
  // If idle, and button down time is very short, flip player. Otherwise 
  // shuffle around.
  /*
  if (!Engine::Instance()->GetPlayer()->IsIdle())
  {
    if (b)
    {
      RotatePlayer(m_playerRotateVel); 
    }
    else
    {
      RotatePlayer(0); 
    }
    return;
  }
  */

  if (b)
  {
    RotatePlayer(m_playerRotateVel);
    m_plusTime = Engine::Instance()->GetElapsedTime();
  }
  else
  {
    m_shuffle = m_shuffleBase;
    RotatePlayer(0);
    
    //float newTime = Engine::Instance()->GetElapsedTime();
    //if ((newTime - m_plusTime) < m_plusFlipTime)
    //{
    //  FlipPlayer(90.0f); // Flip 90 degrees  
    //}
  }

}

void ControlStyleRotateKeys::PlusRight(bool b) 
{
  Engine::Instance()->GetPlayer()->ResetPieceCanRotate();

  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  /*
  if (!Engine::Instance()->GetPlayer()->IsIdle())
  {
    if (b)
    {
      RotatePlayer(-m_playerRotateVel); 
    }
    else
    {
      RotatePlayer(0); 
    }
    return;
  }
  */

  if (b)
  {
    RotatePlayer(-m_playerRotateVel); 
    m_plusTime = Engine::Instance()->GetElapsedTime();
  }
  else
  {
    m_shuffle = m_shuffleBase;
    RotatePlayer(0);
    //float newTime = Engine::Instance()->GetElapsedTime();
    //if ((newTime - m_plusTime) < m_plusFlipTime)
    //{
    //  FlipPlayer(-90.0f); // Flip 90 degrees  
    //}
  }

}

void ControlStyleRotateKeys::JoyX(float f) {}
void ControlStyleRotateKeys::JoyY(float f) {}
void ControlStyleRotateKeys::MousePos(int x, int y) {}
void ControlStyleRotateKeys::MouseButton(bool down, bool ctrl, bool shift) {}

void ControlStyleRotateKeys::Update()
{
  if (m_isWalking)
  {
    m_walkTime += Engine::Instance()->GetDeltaTime();
    if (m_walkTime > m_maxWalkTime)
    {
      // Accelerate player character from walk to run.
      Engine::Instance()->GetPlayer()->SetForwardAccel(m_playerAccel);
      // Cap player vel. 
      float vel = Engine::Instance()->GetPlayer()->GetForwardVel();
      if (vel > m_currentMaxForwardVel)
      {
        m_isWalking = false; // we're running now
        Engine::Instance()->GetPlayer()->SetForwardVel(m_currentMaxForwardVel);
        Engine::Instance()->GetPlayer()->SetForwardAccel(0);
      }
    }
  }

  // If the player is walking, check to see if she should break into a run.
  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    // This stops you after a running jump
    m_isWalking = false;
  }

}
}


