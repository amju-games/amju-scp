/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterControl.cpp,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CharacterControl.h"
#include "HeightServer.h"
#include "FreeMovingGameObject.h"
#include "Character.h"
#include "Common.h"
#include "SchAssert.h"
#include "Geometry.h"
#include "Engine.h"
#include "ConfigKeys.h"
#include "Level.h"
#include <algorithm>
#include <iostream>
#include <math.h>

using namespace std;
namespace Amju
{
void CharacterControl::UpdateCharacter(
  FreeMovingGameObject* pCharacter, 
  float stepHeight)
{
  Assert(pCharacter);

  // Move character in current direction, but handle collision with walls.
  // Calls Character's Reclculate() function.
  HandleWallCollision(pCharacter, stepHeight);

  // Don't let character outside of the absolute x-z boundary.
  EnforceBoundary(pCharacter);

  // TODO This may not be the right place for this call.
  // This is to get a player out of a wall if he has got stuck there.
  ExtractCharacterFromWall(pCharacter);
}

void CharacterControl::UpdateObject(FreeMovingGameObject* pCharacter)
{
  UpdateCharacter(pCharacter, CharacterControl::s_stepHeight); 
}

}
