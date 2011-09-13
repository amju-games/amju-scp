/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Character.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Character.h"
#include "Common.h"
#include "CompositeFactory.h"
#include "Geometry.h"
#include "Texture.h"
#include "Engine.h"
#include "Md2Model.h"
#include "HeightServer.h" // HSTri

using namespace std;

namespace Amju
{
Character::Character()
{
  m_isFrozen = false;
  m_isFalling = false;
}

Character::Character(const Character& rhs) :
  m_animation(rhs.m_animation),
  m_currentAction(rhs.m_currentAction),
  m_actionQueue(rhs.m_actionQueue),
  m_isFrozen(rhs.m_isFrozen)
{
}

Character::~Character()
{
}

const std::string& Character::GetName() const
{
  return m_name;
}

void Character::SetName(const std::string& name)
{
  m_name = name;
}

void Character::SetFrozen(bool frozen)
{
  m_isFrozen = frozen;
}

void Character::Recalculate()
{
}

void Character::SetAction(int actionId)
{
  m_animation = actionId;
}

int Character::GetAction() const
{
  return m_animation;
}

void Character::QueueAction(int actionId)
{
  m_actionQueue.push(actionId);
}

string Character::GetActionName()
{
  return m_currentAction;
}

void Character::PlayCharacterWav(const std::string& wavFile)
{
  string fullpath = wavFile;
//  PrependCharacterDir(&fullpath);
  Engine::Instance()->PlayWav(fullpath);
}

void Character::SetFalling(bool falling)
{
  m_isFalling = falling;
}

}

