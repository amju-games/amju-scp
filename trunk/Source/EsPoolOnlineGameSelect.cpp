/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EsPoolOnlineGameSelect.h"
#include "Engine.h"
#include "PoolOnline.h"
#include "EngineStatePoolGameSelect.h"

namespace Amju
{
const char* EsPoolOnlineGameSelect::Name = "pool-online-game-sel";
static const bool registered= Engine::Instance()->
  RegisterEngineState(EsPoolOnlineGameSelect::Name,
  new EsPoolOnlineGameSelect);

void OnlineBackToPlayerSelect()
{
#ifdef _DEBUG
std::cout << "***SENDING GAME FINISHED\n";
#endif

  ThePoolOnlineManager::Instance()->SendGameFinished();

  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE);
}

bool EsPoolOnlineGameSelect::Load()
{
  if (!EngineStatePoolCourseSelect::Load())
  {
    return false;
  }
  // Set Back button handler
  m_pSelectButton->SetCommand(&OnlineBackToPlayerSelect);

  return true;
}
}

