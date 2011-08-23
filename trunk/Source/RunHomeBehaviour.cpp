/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: RunHomeBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:41  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "RunHomeBehaviour.h"
#include "CharacterGameObject.h"
#include "Common.h"
#include "CrocHome.h"
#include "Engine.h"
#include "Level.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
float RunHomeBehaviour::s_runHomeVel;

void RunHomeBehaviour::Init()
{
  s_runHomeVel = atof(Engine::Instance()->GetConfigValue("croc_run_home_vel").c_str());
}

RunHomeBehaviour::RunHomeBehaviour()
{
  // TODO CONFIG
  m_lookahead = 4.0f;
  m_obstructionRotateVel = 90.0f;

  m_decideBehaviour.Init();
  m_decideBehaviour.SetUpdatePeriod(3.0f); // TODO CONFIG
}

void RunHomeBehaviour::ReachedTarget()
{
  // Decide what to do next. If there are no chicks to chase, find another Home
  // to run towards.
  // To get a new Home we have to change state to RUN_HOME. But we are already
  // in that state. So change to UNKNOWN, which will do a decide in a few secs.
  const int id = m_pCharacter->GetId(); // used when sending messages
  Engine::Instance()->SendGameMessage(new Message("idle", 0, id, id, UNKNOWN));
}

void RunHomeBehaviour::OnActivated()
{
  m_vel = s_runHomeVel;
  CreatePath(); // TODO Check return value!!!
}


void RunHomeBehaviour::Update()
{
  // Copied from ChaseBehaviour

  // Call base class seeking behaviour..
  SeekBehaviourBase::Update();
  // ..then decide if we should change to another state. This is only 
  // done periodically though.
  m_decideBehaviour.SetNpc(m_pCharacter);
  m_decideBehaviour.SetTarget(m_pTarget);
  m_decideBehaviour.Update();
}
}

