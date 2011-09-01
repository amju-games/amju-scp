/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Player.cpp,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Player.h"
#include "Engine.h"
#include "GameState.h"
#include "Character.h"
#include "File.h"
#include "CharacterManager.h"

using namespace std;
namespace Amju
{
Player::Player()
{
}

void Player::Update()
{
  CharacterGameObject::Update();
}

void Player::FellFromHeight(float heightFallenFrom)
{
  if (heightFallenFrom < 4.0f) // TODO CONFIG
  {
    return;
  }

  CharacterGameObject::FellFromHeight(heightFallenFrom);

  // Decide how badly damaged we are from the fall.
  float heightDamage = atof(Engine::Instance()->GetConfigValue("player_height_damage").c_str());
  if (heightDamage == 0)
  {
    heightDamage = 1.0f;
  }

#if defined(_DEBUG)
/*
  cout << "Fallen from height " << heightFallenFrom << " taking " << damage << endl;
*/
#endif
  //Engine::Instance()->GetEngineState()->TakeDamage(damage);
}

void Player::HandleObjectCollision(GameObject* pObj)
{
}

bool Player::Load(File* pf)
{
  if (!CharacterGameObject::Load(pf))
  {
    return false;
  }

  if (!LoadCollisionVolume(pf))
  {
    return false;
  }

  // Load Bounding Sphere(s)
  if (!LoadBoundingSpheres(pf))
  {
    return false;
  }

  // Set up player bounciness (doesn't use player file :-( )
  float bounce = atof(Engine::Instance()->GetConfigValue("player_bounce").c_str());
  SetBounceDampen(bounce);

  return true;
}

bool Player::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open player file.");
    return false;
  }
  return Load(&f);
}

void Player::SetState(State newState)
{
  // Discard messages which no longer apply. 
  State prevState = GetState();

  if (newState == prevState)
  {
    return;
  }
 
  if (newState == UNKNOWN)
  {
    if (prevState == DEAD)
    {
#if defined(_DEBUG)
/*
      cout << "Player: setting state from DEAD to UNKNOWN" << endl;
*/
#endif
    }
    CharacterGameObject::SetState(newState);
  }
  else if (newState == DEAD)
  {
    CharacterGameObject::SetState(newState);
  }
  else if (newState == OUT_OF_PLAY)
  {
    CharacterGameObject::SetState(newState);
  }
  else
  {
//    Assert(0);
#if defined(_DEBUG)
/*
    cout << "Player: unhandled state: " << newState << endl;
*/
#endif
  }
}

}
