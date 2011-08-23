/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolTrickShotOver.cpp,v $
Revision 1.1.2.3  2007/03/27 22:06:38  Administrator
Fixed uninitialised variables for placing m_pText

Revision 1.1.2.2  2006/08/14 17:50:18  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.5  2005/09/30 13:00:56  jay
Don't do hugh earthquake just because game ends

Revision 1.1.2.4  2005/09/29 19:06:14  jay
Save player stats so we can award extra hearts after game is over.

Revision 1.1.2.3  2005/09/25 20:16:48  Administrator
Show bonuses, so extra bonuses can accumulate

Revision 1.1.2.2  2005/09/17 16:18:11  jay
Fixes to get Trick Shots to work

Revision 1.1.2.1  2005/09/16 20:04:19  jay
Add Trick Shot rules

*/

#include "EngineStatePoolTrickShotOver.h"
#include "Engine.h"
#include "LevelServer.h"
#include "GameState.h"
#include "EngineStatePoolCourseSelect.h"
#include "TextFactory.h"
#include "PoolMisc.h"

namespace Amju
{
const char* EngineStatePoolTrickShotOver::Name = "trick-shot-over";

static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolTrickShotOver::Name, 
  new EngineStatePoolTrickShotOver);

void EngineStatePoolTrickShotOver::SetSuccess(bool success)
{
  if (success)
  {
    m_pText = TextFactory::Instance()->CreateMultiline("you did it!");
  }
  else
  {
    m_pText = TextFactory::Instance()->CreateMultiline("try again!");
  }
}

void EngineStatePoolTrickShotOver::DrawOverlays()
{
  EngineStatePoolBase::DrawOverlays();

  DrawBonusesLeft();

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(m_bihLeft, m_bihTop, m_bihZ - 14.0f); // TODO TEMP TEST
  m_pText->Draw();
  AmjuGL::PopMatrix();  
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ); 
}

void EngineStatePoolTrickShotOver::Draw()
{
  EngineStatePoolText::Draw();
}

void EngineStatePoolTrickShotOver::SetActive(bool active)
{
  m_pLevel = LevelServer::Instance()->GetCurrentLevel();

  if (active)
  {
    SetIsGameOver(true);

    m_maxTime = 30.0f; // TODO s/b in ctor ?
    m_time = 0;

    // Make player invisible, otherwise she appears on top of ball.
    // Do for both players or it looks weird.
    // Keeping both players visible entails more work... TODO
    for (int i = 0; i < 2; i++)
    {
      int id = Engine::Instance()->GetGameState()->GetPlayerInfo(i)->m_id;
      GameObject* pGo = Engine::Instance()->GetGameObject(id).GetPtr();
      Assert(pGo);
      pGo->SetState(OUT_OF_PLAY);
    }

    std::string text = Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_name;

    // Make sure player name is up to date.
    CreatePlayerName(text);

    // Play sound effect of ball in hole
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_9"));
    // "Yay!" acting
    Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("golf_wav_12"));

    Engine::Instance()->GetGameState()->GetCurrentPlayerInfo()->m_isPlaying = false;
    // Play special music.
    std::string song = GetEngine()->GetConfigValue("golf_song_ball_in_hole");
    GetEngine()->PlaySong(song);

    // TODO CONFIG
//    Engine::Instance()->GetEarthquake().SetActive(5.0f, 20.0f);
    Engine::Instance()->SetLetterbox(true);
  }
  else
  {
    Engine::Instance()->SetLetterbox(false);
  }
}

void EngineStatePoolTrickShotOver::TimerExpired()
{
  // Save both player's stats. The idea is that a player may have had extra
  // bonuses awarded while this state was active.
  for (int i = 0; i < 2; i++)
  {
    PlayerStats* p = Engine::Instance()->GetGameState()->GetPlayerInfo(i)->
      GetPlayerStats();
    if (p)
    {
      if (!p->Save())
      {
#ifdef _DEBUG
std::cout << "FAILED to save player stats for " << i << "\n";
#endif
      }
    }
  }

  GetEngine()->ChangeState(EngineStatePoolCourseSelect::Name, Engine::IMMEDIATE);
}

void EngineStatePoolTrickShotOver::Update()
{
  EngineStatePoolText::Update();
}

void EngineStatePoolTrickShotOver::MouseButton(bool down, bool, bool)
{
  if (!down && m_time > 3.0f)
  {
    TimerExpired();
  }
}

bool EngineStatePoolTrickShotOver::Load()
{
  if (!EngineStatePoolText::Load())
  {
    return false;
  }
  m_bihZ = 0;
  return true;
}
}


