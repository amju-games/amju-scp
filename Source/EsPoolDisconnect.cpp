/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
$Log: EsPoolDisconnect.cpp,v $
Revision 1.1.2.5  2007/07/21 10:05:44  jay
Show opponent name in message

Revision 1.1.2.4  2007/07/12 20:29:40  jay
Keep polling for opponent's connect time

Revision 1.1.2.3  2007/06/30 14:32:27  jay
Improve presentation

Revision 1.1.2.2  2007/06/28 14:35:45  jay
Reset disconnect time if player decides to wait.

Revision 1.1.2.1  2007/06/23 11:27:44  jay
Add Error message page for when online opponent goes away

*/

#if defined(WIN32) 
#pragma warning(disable: 4786)
#endif

#include "EsPoolDisconnect.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolGameSelect.h"
#include "PoolOnline.h"

namespace Amju
{
const char* EsPoolDisconnect::Name = "pool-disconnect";

static const bool registered= Engine::Instance()->
  RegisterEngineState(EsPoolDisconnect::Name,
  new EsPoolDisconnect);

EsPoolDisconnect::EsPoolDisconnect()
{
  std::string op = ThePoolOnlineManager::Instance()->GetOpponentName(); 
  Assert(!op.empty());
  op[0] = toupper(op[0]);
  std::string s = "Oh dear! ";
  s += op; 
  s += " seems to have disconnected.";
  AddQ(s);
  AddQ("Would you like to quit ?");

  m_yesFilename = "quit-yes-button.txt";
  m_noFilename = "quit-no-button.txt";
}

void EsPoolDisconnect::OnYes()
{
  // Quit back to main menu - send game finished message to server
  ThePoolOnlineManager::Instance()->SendGameFinished();

  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name, 
    Engine::IMMEDIATE);
}

void EsPoolDisconnect::OnNo()
{
  ThePoolOnlineManager::Instance()->ResetOpponentDisconnectTime();

  // Back to previous state -- assuming EsPoolSetupShot ??
  Engine::Instance()->ChangeState(EngineStatePoolSetUpShot::Name, 
    Engine::IMMEDIATE);
}

void EsPoolDisconnect::Update()
{
  EsPoolYesNo::Update();

  // Keep polling: updates our connect time on the server.
  // If opponent has re-connected, go back to the game.
  if (!ThePoolOnlineManager::Instance()->OpponentHasDisconnected())
  {
    OnNo();
  }
}
}


