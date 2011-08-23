/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineState.cpp,v $
Revision 1.2  2004/09/15 09:17:08  Administrator
Removed some confusing old code

Revision 1.1  2004/09/08 15:42:48  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineState.h"
#include "Engine.h"
#include "TextWriter.h"
#include "SchAssert.h"

namespace Amju
{
Engine* EngineState::GetEngine()
{
  return Engine::Instance();
}

EngineState::EngineState() : m_showControls(false)
{
  m_func = 0;
  m_mouseDown = false;
}

void EngineState::ClearScrollText()
{
  GetEngine()->GetTextWriter()->ClearScrollingItems();
}

void EngineState::ReceiveMessage(const Message& m)
{
  PGameObject pGo = GetEngine()->GetGameObject(m.GetRecipientId());
  if (pGo.GetPtr())
  {
    pGo->ReceiveMessage(m);
  }
  else
  {
#if defined(_DEBUG)
    GetEngine()->ReportError("Bad recipient ID in message '" + m.GetName() + "'.");
#endif
  }
}

void EngineState::GameOver()
{
}

void EngineState::OnKey(char key, bool down)
{
  GetEngine()->GetKeymap()->OnKey(key, down);
}

bool EngineState::ShowControls() const
{
  return m_showControls;
}
/*
void EngineState::GoToNextState()
{
  GetEngine()->ChangeState(GetEngine()->GetNextStateName(), Engine::IMMEDIATE);

  // Call a function if one is specified.
  if (m_func)
  {
    (m_func)();
    m_func = 0;
  }
}

void EngineState::SetNextStateFunction(Func f)
{
  m_func = f;
}
*/
void EngineState::SetMaxTime(float m)
{
  m_maxTime = m;
}

void EngineState::SetActive(bool active)
{
  m_time = 0;
  m_mouseDown = false;
}

void EngineState::Update() 
{
  float dt = GetEngine()->GetDeltaTime();
  m_time += dt;

  if (m_time > m_maxTime)
  {
    TimerExpired();
  }
}

void EngineState::MouseButton(bool down, bool ctrl, bool shift)
{
  if (down)
  {
    m_mouseDown = true;
  }
  
  if (m_mouseDown && !down)
  {
    Red(true); 
  }

  if (!down)
  {
    m_mouseDown = false;
  }
}
}
