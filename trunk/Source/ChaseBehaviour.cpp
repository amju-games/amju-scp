/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChaseBehaviour.cpp,v $
Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ChaseBehaviour.h"
#include "CharacterGameObject.h"
#include "HeightServer.h"
#include "Common.h"
#include "Engine.h"
#include "EngineRunning.h"
#include "SchAssert.h"
#include "Level.h"
#include <iostream>

using namespace std;
namespace Amju
{
float ChaseBehaviour::s_runVel;

ChaseBehaviour::ChaseBehaviour() 
{
  // TODO CONFIG
  m_lookahead = 4.0f;
  m_obstructionRotateVel = 90.0f;

  // Make new-state-decision periodically. This sets the period.
  m_decideBehaviour.Init();
  m_decideBehaviour.SetUpdatePeriod(2.0f); // TODO CONFIG
}

void ChaseBehaviour::Init()
{
  s_runVel = atof(Engine::Instance()->GetConfigValue("croc_run_vel").c_str());
}

void ChaseBehaviour::OnActivated()
{
  m_vel = s_runVel;
  CreatePath(); // This assumes the target has been set already.
  // TODO Check return value!!!
}

void ChaseBehaviour::ForceDecision()
{
  m_decideBehaviour.ForceUpdate();
}

void ChaseBehaviour::ReachedTarget()
{
  // No need to do anything here; it's handled by croc-chick collision.
}

void ChaseBehaviour::Update()
{
  // Call base class seeking behaviour..
  SeekBehaviourBase::Update();
  // ..then decide if we should change to another state. This is only 
  // done periodically though.
  m_decideBehaviour.SetNpc(m_pCharacter);
  m_decideBehaviour.SetTarget(m_pTarget);
  m_decideBehaviour.Update();
}

}

