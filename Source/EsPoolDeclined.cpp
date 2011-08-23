/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EsPoolDeclined.h"
#include "Engine.h"
#include "PoolOnline.h"
#include "GameState.h"
#include "EngineStatePoolOnlineTable.h"

namespace Amju
{
static float TOP = 2.3f;
static float LEFT = 8.0f;
static float WIDTH = 8.0f;
static float HEIGHT = 2.0f;
static float HSPACE = 2.25f;

const char* EsPoolDeclined::Name = "pool-declined";

static const bool registered= Engine::Instance()->
  RegisterEngineState(EsPoolDeclined::Name,
  new EsPoolDeclined);

EsPoolDeclined::EsPoolDeclined()
{
  m_yesFilename = "pool-ok-button.txt";
  m_noFilename = "pool-ok-button.txt";
}

void EsPoolDeclined::SetActive(bool active)
{
  if (active)
  {
    std::string opponent = ThePoolOnlineManager::Instance()->GetOpponentName(); 
    if (opponent.size() == 0)
    {
      opponent = "your opponent";
    }
    else
    {
      opponent[0] = toupper(opponent[0]);
    }
    m_question.clear();
    std::string s = "Sorry, " + opponent + " cannot play with you.";
    AddQ(s);  

    // Hide No button
    m_pNoButton->SetVisible(false);

    m_pYesButton->SetSize(4.0f, 2.0f); // width, height
    m_pYesButton->SetRelPos(TOP + 2.0f * HSPACE, LEFT + 2.0f); // top, left

  }
  EsPoolYesNo::SetActive(active);
}

void EsPoolDeclined::OnYes()
{
  ThePoolOnlineManager::Instance()->SendGameFinished();

  Engine::Instance()->ChangeState(EngineStatePoolOnlineTable::Name,
    Engine::IMMEDIATE);
}
}

