/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddCharacters.cpp,v $
Revision 1.1.8.3  2006/08/14 17:38:04  jay
Rename "Pool"

Revision 1.1.8.2  2006/07/26 21:25:17  jay
Use Pool/, not Pool/

Revision 1.1.8.1  2005/07/30 12:09:19  jay
Now loads character data from a different file for each character.

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerAddCharacters.h"
#include "Level.h"
#include "File.h"
#include "CharacterManager.h"
#include "Engine.h"
#include "Player.h"
#include "PoolCharacter.h"

namespace Amju
{
const char* LevelWorkerAddCharacters::TypeName = "characters";
const char* LevelWorkerAddCharacters::GetTypeName() { return TypeName; }

LevelWorkerAddCharacters::LevelWorkerAddCharacters()
{
  m_infoString = "character";
}

bool LevelWorkerAddCharacters::AddFeature(Level* pLevel)
{
  int numChars = m_charNames.size();
  for (int i = 0; i < numChars; i++)
  {
    // Make a simple cube for the character bounding box.
    //float s = 2.0f; // box size
    //BoundingBox bb(0, s, 0, s, 0, s);

    Orientation initialOr((float)(i * 2), 13, (float)(i * 2), 0, 0, 0);

    // GOLF
    CharacterGameObject* pCharacter = new PoolCharacter;

    pCharacter->SetLevel(pLevel);
    pCharacter->SetId(1 + i); // TODO CONFIG ?
    pCharacter->SetRoomId(pLevel->GetRoomId());

    // Set the attributes which are not loaded.
    std::string characterName = m_charNames[i];

    File f;
    std::string s = characterName;
    s += ".txt";
    if (!f.OpenRead(s.c_str()))
    {
      f.ReportError("Failed to open character file.");
      return false;
    }
    pCharacter->Load(&f);

    pCharacter->SetState(UNKNOWN);
    pCharacter->ClearMeshes();
    pCharacter->AddMesh(CharacterManager::Instance()->GetCharacter(characterName));
    pCharacter->SetOrientation(initialOr);

    // Use this function so we keep track of which LevelWorker added which object.
    AddGameObject(pLevel, pCharacter);
  }

  return true;
}

bool LevelWorkerAddCharacters::Load(File* pf)
{
  int numChars = 0;
  if (!pf->GetInteger(&numChars))
  {
    pf->ReportError("Expected number of characters.");
    return false;
  }
  for (int i = 0; i < numChars; i++)
  {
    std::string s;
    if (!pf->GetDataLine(&s))
    {
      pf->ReportError("Expected character name.");
      return false;
    }
    m_charNames.push_back(s);
  }

  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerAddCharacters::Save(File* pf)
{
  int num = m_charNames.size();
  pf->WriteInteger(num);
  for (int i = 0; i < num; i++)
  {
    pf->Write(m_charNames[i]);
  }
  return true;
}
#endif

void LevelWorkerAddCharacters::AddCharacterName(const std::string& name)
{
  // TODO verify that character name is known to character manager ?
  m_charNames.push_back(name);
}
}

