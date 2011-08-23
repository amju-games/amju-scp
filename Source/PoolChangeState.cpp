/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: PoolChangeState.cpp,v $
Revision 1.1.2.5  2006/08/30 21:12:19  Administrator
Get Pool to build in MSVC

Revision 1.1.2.4  2006/08/17 18:15:42  jay
Ongoing work for Online Pool

Revision 1.1.2.3  2006/08/14 17:50:22  jay
Rename "Pool"

Revision 1.1.2.2  2006/08/12 07:43:08  jay
Ongoing fixes for Online Pool: currently trying to get both clients to always
be consistent.

Revision 1.1.2.1  2006/07/24 18:20:38  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.1  2005/09/23 19:58:42  jay
Functions to change state, taking Mode into account

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "PoolChangeState.h"
#include "Engine.h"
#include "EngineStatePoolSetUpShot.h"
#include "EngineStatePoolSetUpShotOnline.h"
#include "EngineStatePoolSetUpShotNag.h"
#include "EngineStatePoolSetUpShotDemo.h"
#include "EngineStatePoolShowShot.h"
#include "EngineStatePoolShowShotNag.h"
#include "EngineStatePoolShowShotDemo.h"
#include "EngineStatePoolShotInPlay.h"
#include "EngineStatePoolShotInPlayNag.h"
#include "EngineStatePoolShotInPlayDemo.h"
#include "PoolMisc.h"

namespace Amju
{
void ChangeStateToSetUpShot()
{
  if (IsNagMode())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Nag mode: changing state to SetUpShot\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolSetUpShotNag::Name, Engine::IMMEDIATE);
  }
  else if (IsDemoMode())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Demo mode: changing state to SetUpShot\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolSetUpShotDemo::Name, Engine::IMMEDIATE);
  }
  else if (IsOnlineGame())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "ONLINE mode: changing state to SetUpShotOnline\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolSetUpShotOnline::Name, Engine::IMMEDIATE);
  }
  else
  {
    Engine::Instance()->ChangeState(EngineStatePoolSetUpShot::Name, Engine::IMMEDIATE);
  }
}

void ChangeStateToShowShot()
{
  if (IsNagMode())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Nag mode: changing state to ShowShot\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolShowShotNag::Name, Engine::IMMEDIATE);
  }
  else if (IsDemoMode())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Demo mode: changing state to ShowShot\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolShowShotDemo::Name, Engine::IMMEDIATE);
  }
  else 
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Changing state to ShowShot...\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolShowShot::Name, Engine::IMMEDIATE);
  }
}

void ChangeStateToShotInPlay()
{
  if (IsNagMode())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Nag mode: changing state to ShotInPlay\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolShotInPlayNag::Name, Engine::IMMEDIATE);
  }
  else if (IsDemoMode())
  {
#ifdef CHANGE_STATE_DEBUG
std::cout << "Demo mode: changing state to ShotInPlay\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePoolShotInPlayDemo::Name, Engine::IMMEDIATE);
  }
  else 
  {
    Engine::Instance()->ChangeState(EngineStatePoolShotInPlay::Name, Engine::IMMEDIATE);
  }
}
}


