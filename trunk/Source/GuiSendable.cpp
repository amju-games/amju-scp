/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GuiSendable.h"
#include "Engine.h"

namespace Amju
{
GuiSendable::GuiSendable()
{
  m_bounceState = 0;
  m_animateTimeRemaining = 0;
  m_newX = 0;
  m_newY = 0;
  m_xVel = 0;
  m_yVel = 0;
}

void GuiSendable::SendToNewPos(float newX, float newY, float time)
{
  m_bounceState = 0;
  m_animateTimeRemaining = time;
  m_newX = newX;
  m_newY = newY;
    
  float x = 0, y = 0;
  GetRelPos(&y, &x); // top, left

  m_xVel = (m_newX - x) / time;
  m_yVel = (m_newY - y) / time;

#ifdef TILE_DEBUG
//std::cout << "Tile " << m_letter.c_str() << " send to new pos: "
//  << m_newX << " y: " << m_newY << "\n";
#endif
}

void GuiSendable::Update()
{
  if (m_animateTimeRemaining > 0 && m_bounceState < 3)
  {
    float dt = Engine::Instance()->GetDeltaTime();

    float x = 0, y = 0;
    GetRelPos(&y, &x); // top, left

    m_animateTimeRemaining -= dt;
    x += m_xVel * dt;
    y += m_yVel * dt;

    // Make sure tile is accurately positioned at the end of the
    // animation.
    if (m_animateTimeRemaining <= 0)
    {
      // Animate little bounce at the end of the animation
      ++m_bounceState;

      static const float BOUNCE_VEL = Engine::Instance()->
        GetConfigFloat("tile-bounce-vel");
      static const float BOUNCE_TIME = Engine::Instance()->
        GetConfigFloat("tile-bounce-time");

      switch (m_bounceState)
      {
      case 1:
        if (m_yVel == 0)
        {
          m_bounceState = 2; // Don't bounce as tile only moved horizontally
        }
        else
        {
          // Bounce up a little
          m_yVel = -BOUNCE_VEL;
          m_xVel = 0;
          float r = (float)rand() / (float)RAND_MAX;
          m_bounceTime = BOUNCE_TIME + BOUNCE_TIME * 0.5f * r;
          m_animateTimeRemaining = m_bounceTime;
        }
        break;
      case 2:
        // Go back down
        m_yVel = BOUNCE_VEL;
        m_xVel = 0;
        m_animateTimeRemaining = m_bounceTime;
        break;
      case 3:
        x = m_newX;
        y = m_newY;
        m_animateTimeRemaining = 0;

/*
        // Play tile sound as it lands
        // Do this through Sound FX manager because we may shift up the whole
        // grid and want to limit the number of times the sound if played.
        std::string wav = "click.wav"; // TODO good sound effect
        SoundFxManager::Instance()->PlayWav(wav.c_str());
*/
      }
    }

    SetRelPos(y, x); // top, left
  }
}

void GuiSendable::DrawImpl()
{
  Update();

  GuiComposite::DrawImpl();
}

}


