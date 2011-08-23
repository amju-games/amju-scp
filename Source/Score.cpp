/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Score.cpp,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "Score.h"
#include "Engine.h"
#include "TextWriter.h"
#include "GameState.h"
#include "Text3dLeaf.h"

using namespace std;
namespace Amju
{
void Score::Init()
{
  m_left = atof(Engine::Instance()->GetConfigValue("score_left").c_str());
  m_top = atof(Engine::Instance()->GetConfigValue("score_top").c_str());
  m_z = atof(Engine::Instance()->GetConfigValue("score_z").c_str());
  m_scale = atof(Engine::Instance()->GetConfigValue("score_scale").c_str());

  m_pHiText = new Text3dLeaf("hi"); // TODO CONFIG ?
}

Score::Score() 
{
  m_score = 0;
  m_num.SetJustify(Number::LEFT);
  m_hiScore = 0;
  m_numHi.SetJustify(Number::LEFT);
}

Score::~Score() 
{
}

void Score::Set(int score)
{
  m_num.SetNumber(score);
  
  m_score = score;
  if (m_score > m_hiScore)
  {
    m_hiScore = m_score; 
    Engine::Instance()->GetGameState()->Set("hiscore", m_hiScore);
    m_numHi.SetNumber(m_hiScore);
  }
}

int Score::Get() const
{
  return m_score;
}

void Score::Reset()
{
  m_score = 0;
  m_num.SetNumber(0);

  m_hiScore = (int)Engine::Instance()->GetGameState()->GetFloat("hiscore");
  m_numHi.SetNumber(m_hiScore);
}

void Score::Draw()
{
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_left, m_top, m_z);
  AmjuGL::Scale(m_scale, m_scale, m_scale);
  m_num.Draw();

  // TODO CONFIG
  AmjuGL::Translate(0, -1.3f, 0);
  AmjuGL::Scale(0.5f, 0.5f, 0.5f);
  m_pHiText->Draw();
  AmjuGL::Translate(1.5f, 0.3f, 0);
  m_numHi.Draw();

  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}
}

