/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateWorldDemoHiScores.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:33  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2  2004/09/23 13:19:35  Administrator
Removed public domain comment

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateWorldDemoHiScores.h"
#include "Engine.h"
#include "File.h"
#include "TextWriter.h"
#include "HiScoreTable.h"
#include "EngineStateTitle.h"
#include "TextFactory.h"
#include "StringUtils.h"
#include "TextColourizerColourList.h"

namespace Amju
{
const char* EngineStateWorldDemoHiScores::Name = "world_demo_hiscores"; 
// Register this State with the Engine.
static const bool registeredtitle = Engine::Instance()->
  RegisterEngineState(EngineStateWorldDemoHiScores::Name, 
  new EngineStateWorldDemoHiScores);

void EngineStateWorldDemoHiScores::DrawOverlays()
{
  // z-offset, corresponds to text size.
  static const float z = Engine::Instance()->GetConfigFloat("hi_score_text_z");
  EngineStateText::DrawOverlays();

  AmjuGL::PushMatrix();
  m_pCam->Draw();
  m_or.Draw();
  AmjuGL::Translate(0, m_hiScoreTop, 0);
  AmjuGL::Translate(0, 0, z);
  for (int i = 0; i < (int)m_strs.size(); i++)
  {
    PSolidComponent pComp = m_strs[i];
    if (pComp.GetPtr())
    {
      // position
      AmjuGL::Translate(0, -2.0f * m_verticalPos, 0);
      pComp->Draw();
    } 
  }
  AmjuGL::PopMatrix();

  static const float UP_VEL = Engine::Instance()->GetConfigFloat("high_score_up_vel");
  m_hiScoreTop += Engine::Instance()->GetDeltaTime() * UP_VEL;
}

void EngineStateWorldDemoHiScores::SetActive(bool b)
{
  m_time = 0;
  if (b)
  {
    m_hiScoreTop = 0;
    
    TextColourizerColourList tc;
    tc.AddColour(Colour(1.0f, 1.0f, 0.0f, 1.0f));
    tc.AddColour(Colour(1.0f, 0.0f, 0.0f, 1.0f));
    tc.AddColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
    tc.AddColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
    tc.AddColour(Colour(0.0f, 1.0f, 1.0f, 1.0f));
    tc.AddColour(Colour(1.0f, 0.5f, 1.0f, 1.0f));

    // Create the 3D strings from hi scores and names.
    m_strs.clear();
    //m_strs.push_back(TextFactory::Instance()->Create("hi scores"));
    std::string name;
    int score;
    for (int i = 0; i < SingleHiScoreTable::TABLE_SIZE; i++)
    {
      HiScoreTable::Instance()->GetScore(i, &score, &name);
      std::string s;
      //s += ToString(i + 1);
      //s += " ";
      s += ToString(score);
      s += "   ";
      s += name;
      m_strs.push_back(TextFactory::Instance()->Create(s, &tc));
    }
  }
}

bool EngineStateWorldDemoHiScores::Load()
{
  HiScoreTable::Instance()->Load();

  m_bg.Load("oct_t3.png", "");

  m_maxTime = Engine::Instance()->GetConfigFloat("hi_score_max_time");
  return true;
}

void EngineStateWorldDemoHiScores::TimerExpired()
{
  Engine::Instance()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
}

void EngineStateWorldDemoHiScores::MouseButton(bool down, bool, bool)
{
  if (!down)
  {
    Engine::Instance()->ChangeState(EngineStateTitle::Name, Engine::IMMEDIATE);
  }
}
}
