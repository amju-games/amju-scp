/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleJoystick.cpp,v $
Revision 1.1.8.1  2005/06/05 00:25:51  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyleJoystick.h"
#include "Common.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "Player.h"

namespace Amju
{
ControlStyleJoystick::ControlStyleJoystick()
{
  Reset();
}

void ControlStyleJoystick::Reset()
{
  m_joyX = 0;
  m_joyY = 0;
  m_joyTheta = 0;
  m_oldJoyTheta = 0;
  // Player can only flip once before Joystick returns to centre position.
  m_flip = 0.5f;
  // Get joystick config values
  m_flip = Engine::Instance()->GetConfigFloat("joystick_flip");

}

void ControlStyleJoystick::Red(bool) {}

void ControlStyleJoystick::Blue(bool down) 
{
  DoAction(down);
}

void ControlStyleJoystick::Green(bool) {}
void ControlStyleJoystick::YellowUp(bool) {}
void ControlStyleJoystick::YellowDown(bool) {}
void ControlStyleJoystick::YellowLeft(bool) {}
void ControlStyleJoystick::YellowRight(bool) {}
void ControlStyleJoystick::Z(bool) {} 
void ControlStyleJoystick::PlusUp(bool) {}
void ControlStyleJoystick::PlusDown(bool) {}
void ControlStyleJoystick::PlusLeft(bool) {}
void ControlStyleJoystick::PlusRight(bool) {}

void ControlStyleJoystick::JoyX(float f) 
{
  m_joyX = f; // work out polar coord later.
}

void ControlStyleJoystick::JoyY(float f) 
{
  m_joyY = f;
}

void ControlStyleJoystick::MousePos(int x, int y) 
{
}

void ControlStyleJoystick::MouseButton(bool down, bool ctrl, bool shift) 
{
}

void ControlStyleJoystick::Update()
{
  if (Engine::Instance()->GetPlayer()->IsFalling())
  {
    return;
  }

  // Get camera y-rotate. 
  float camRot = ER()->GetCamera()->GetOrientation()->GetYRot();
  Engine::Instance()->GetPlayer()->SetYRotateVel(0);

  // Set player forward velocity/rotate velocity/position based on x-y joystick
  // values. The player is actually moved later using the velocity values.
  if (fabs(m_joyX) < SMALLEST && fabs(m_joyY) < SMALLEST)
  {
    // Joystick is zeroed. Don't change theta.
    m_joyRadius = 0;
    //m_isWalking = false; 
    Engine::Instance()->GetPlayer()->SetForwardAccel(SLOW_TO_STOP); // decelerate and stop.

    // Set old theta..
    // after some delay.
    m_oldJoyTheta = camRot;
  }
  else
  {
    Engine::Instance()->GetPlayer()->ResetPieceCanRotate();  
    
    // Get polar coord from x-y.
    m_joyRadius = sqrt(m_joyX * m_joyX + m_joyY * m_joyY);
    float joyTheta = atan2(m_joyY, m_joyX);
    // Convert to degs
    m_joyTheta = joyTheta/pi * 180.0f;
    m_joyTheta -= 90; 

    // Stick North is the direction the camera is facing.
    m_joyTheta += camRot;

    const Orientation* pO = Engine::Instance()->GetPlayer()->GetOrientation();

    // If the radius is less than some limit, we shuffle around on the spot. 
    // In this case we just need to know which direction to shuffle in.

    if (m_joyRadius > m_flip) 
    {
      // Move player forward.
      // Start at walking speed. Start timer, so we break into a run after a delay.
      if (Engine::Instance()->GetPlayer()->GetForwardVel() < m_walkForwardVel)
      {
        Engine::Instance()->GetPlayer()->SetForwardVel(m_walkForwardVel);
        Engine::Instance()->GetPlayer()->SetForwardAccel(m_playerAccel);
        //m_isWalking = true;
        //m_walkTime = 0; // we start running when this reaches m_maxWalkTime 
      }

      // Old way: make speed directly proportional to joystick 'radius'.
      //GetPlayer()->SetForwardVel(m_joyRadius * m_currentMaxForwardVel);

      // TODO Player rotate: alter rotate vel. rather than angle directly ?
      //pO->SetYRot(m_joyTheta);
    }    
    else
    { 
      /*
      // Shuffle around.
      Engine::Instance()->GetPlayer()->SetForwardVel(0);
      if (m_joyX < 0)
      {
        RotatePlayer(m_shuffle);
      }
      else
      {
        RotatePlayer(-m_shuffle);
      }
      */
    }
  } 

}

}

