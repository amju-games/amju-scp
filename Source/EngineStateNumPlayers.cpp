/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateNumPlayers.cpp,v $
Revision 1.1.8.2  2006/08/14 17:38:03  jay
Rename "Pool"

Revision 1.1.8.1  2006/07/26 21:25:16  jay
Use Pool/, not Pool/

Revision 1.1  2004/09/08 15:42:36  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateNumPlayers.h"
#include "EngineStatePoolSetUpShot.h"
#include "Engine.h"
#include "TextMenu.h"
#include "StringUtils.h"
#include "Text3dLeaf.h"
#include "GameState.h"
#include "PoolMisc.h"
#include "EngineStatePoolCourseSelect.h"
#include "EngineStatePoolGameSelect.h"

namespace Amju
{
const char* EngineStateNumPlayers::Name = "num_players"; 
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStateNumPlayers::Name, new EngineStateNumPlayers);

void EngineStateNumPlayers::SetMin(int m)
{
  m_min = m;
}

void EngineStateNumPlayers::SetMax(int m)
{
  m_max = m;
}

void EngineStateNumPlayers::SetActive(bool active)
{
  TextMenu* pMenu = new TextMenu;

  pMenu->AddItem(new Text3dLeaf("back"));

  for (int i = m_min; i <= m_max; i++)
  {
    std::string s;
    if (i == 0)
    {
      s = "none"; // TODO INTL8N
    }
    else
    {
      s = ToString(i) + " player";
      if (i != 1)
      {
        s += "s";
      }
    }

    PSolidComponent p = new Text3dLeaf(s.c_str());
    pMenu->AddItem(p.GetPtr());
  }

  pMenu->SetSelected(0);
  m_pMenu = pMenu;
}

bool EngineStateNumPlayers::Load()
{
  if (!EngineMenuState::Load())
  {
    return false;
  }
  m_min = m_max = 0;
  return true;
}

void EngineStateNumPlayers::Red(bool down)
{
  if (down)
  {
    int numPlayers = m_pMenu->Choose();
    if (numPlayers == 0)
    {
      // cancel, (back)
      GetEngine()->ChangeState(EngineStatePoolGameSelect::Name, Engine::IMMEDIATE);
    }
    else
    {
      // TODO this only works for single player mode.
      numPlayers = numPlayers + m_min;

      // Create the players.
      // Set up all players except the first as AIs.
      GetEngine()->GetGameState()->ClearPlayers();

      GameState::PlayerInfo p1(1, "amju", Orientation(), false);
      p1.m_score = 0;
      p1.m_isPlaying = true;
      p1.m_strokes = 0;
      p1.m_reset = true;
      p1.m_isAi = false;
      GetEngine()->GetGameState()->AddPlayerInfo(p1);

      if (numPlayers > 1)
      {
        GameState::PlayerInfo p2(2, "henster", Orientation(), false);
        p2.m_score = 0;
        p2.m_isPlaying = true;
        p2.m_strokes = 0;
        p2.m_reset = true;
        p2.m_isAi = true;
        GetEngine()->GetGameState()->AddPlayerInfo(p2);
      }

      if (numPlayers> 2)
      {
        GameState::PlayerInfo p3(3, "pat-pat", Orientation(), false);
        p3.m_score = 0;
        p3.m_isPlaying = true;
        p3.m_strokes = 0;
        p3.m_reset = true;
        p3.m_isAi = true;
        GetEngine()->GetGameState()->AddPlayerInfo(p3);
      }

      if (numPlayers > 3)
      {
        GameState::PlayerInfo p4(4, "chicky loulou", Orientation(), false);
        p4.m_score = 0;
        p4.m_isPlaying = true;
        p4.m_strokes = 0;
        p4.m_reset = true;
        p4.m_isAi = true;
        GetEngine()->GetGameState()->AddPlayerInfo(p4);
      }
      GetEngine()->GetGameState()->SetCurrentPlayer(0);
      GetEngine()->ChangeState(EngineStatePoolCourseSelect::Name, Engine::IMMEDIATE);
    }
  }
}
}




