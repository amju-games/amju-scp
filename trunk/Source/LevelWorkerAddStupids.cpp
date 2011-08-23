/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddStupids.cpp,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "LevelWorkerAddStupids.h"
#include "Stupid.h"
#include "Level.h"
#include "File.h"
#include "CharacterManager.h"
#include "Engine.h"

namespace Amju
{
const char* LevelWorkerAddStupids::TypeName = "add_stupids";
const char* LevelWorkerAddStupids::GetTypeName() { return TypeName; }

bool LevelWorkerAddStupids::Load(File* )
{
  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddStupids::Save(File* )
{
  return true;
}
#endif

bool LevelWorkerAddStupids::AddFeature(Level* pLevel, CubeMap* pCubeMap)
{
  // Assume the zeroth character held by Character Manager is reserved 
  // for the player character.

  // For each type of stupid (i.e. name recognised by CharacterManager)
  int levelNum = pLevel->GetId();

  // How many Stupids to add for this level ?
  // Start with 1, easiest.
  int numStupids = 1 + levelNum; 
  int max = (int)Engine::Instance()->GetConfigFloat("max_stupids"); 
  if (numStupids > max)
  {
    numStupids = max;
  }

  std::vector<std::string> characterNames; 
  CharacterManager::Iterator it = CharacterManager::Instance()->Begin();
  ++it; // skip player character
  for ( ;
       it != CharacterManager::Instance()->End(); 
       ++it)
  {
    characterNames.push_back(*it);
  }

  int numAvailableCharacters = characterNames.size();
  int characterIndex = levelNum;
  characterIndex %= numAvailableCharacters;
  float dir = 0;
  float turnDir = 90.0f;

  // Get a list of the character names we will use.
  // Then we want to sort the character names before adding them, to minimise texture changes.
  // This is a desperate attempt to try to speed things up on slow graphics cards.
  typedef std::vector<std::string> StringVec;
  StringVec namevec; 
  
  int i;

  for (i = 0; i < numStupids; i++)
  {
    std::string characterName = characterNames[characterIndex];
    // Work backwards from index so in level N we get the characters from level
    // N - 1 plus one new one (up until all the characters are used).
    --characterIndex;
    if (characterIndex < 0) 
    {
      characterIndex = characterNames.size() - 1;
    }
    namevec.push_back(characterName);
  }

  // Sort the list of character names. The end result of this is that characters with the
  // same texture will be drawn consecutively. This will minimise texture changes for characters.
  std::sort(namevec.begin(), namevec.end());

  for (i = 0; i < numStupids; i++)
  {
    // Make a simple cube for the character bounding box.
    float s = 2.0f; // box size
    BoundingBox bb(0, s, 0, s, 0, s);

    Orientation initialOr;
    if (!GetOrientation(pCubeMap, bb, pLevel->GetPlayAreaSize(), &initialOr))
    {
      return false; // play area too small
    }

    //float shadowSize = 1.0f; // TODO should be a Character attribute

    float thisInitialDir = dir;
    dir += 90.0f;
    if (dir >= 359.0f)
    {
      dir = 0;
    }

    float thisTurnDir = turnDir;
    turnDir = -turnDir;

    // Speed
    //float stupidSpeed = 5.0f; // TODO Should be a character attribute

    // Bounding Sphere and collision vol: use defaults loaded from file

    Stupid* pStupid = new Stupid;
    pStupid->SetLevel(pLevel);
    pStupid->SetId(GetUniqueId());
    pStupid->SetRoomId(0);
    // Choose a file to load from.
    // Load the Stupid.
    // TODO set up in ctor
    File f;
    f.OpenRead("stupid1.txt"); // TODO TEMP TEST
    pStupid->Load(&f);
    // Set the appearance delay.
    pStupid->SetAppearanceDelay((float)i * 2.0f + 2.0f); // TODO TEMP TEST

    // Set the attributes which are not loaded.
    std::string characterName = namevec[i];
    pStupid->ClearMeshes();
    pStupid->AddMesh(CharacterManager::Instance()->GetCharacter(characterName));
    pStupid->SetOrientation(initialOr);
    pStupid->SetInitialDirection(thisInitialDir);
    pStupid->SetTurnDirection(thisTurnDir);
    /*
    pStupid->SetMaxSpeed(0); // TODO Should be character attrib
    pStupid->SetShadowSize(0); // TODO Should be character attrib
    pStupid->SetTurnVel(0); // TODO Should be character attrib
    */

    // Use this function so we keep track of which LevelWorker added which object.
    AddGameObject(pLevel, pStupid);
  }

  return true;
}
}

