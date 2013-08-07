/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterManager.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Engine.h"
#include "CharacterManager.h"
#include "Character.h"
#include "CharacterMd2Regular.h"
#include "CharacterMd2Chicken.h" // need all types for factory function
#include "File.h"
#include "TextureServer.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
CharacterManager* CharacterManager::Instance()
{
  static CharacterManager cm;
  return &cm;
}

CharacterManager::CharacterManager()
{
  Clear();
}

void CharacterManager::Clear()
{
  m_md2Models.clear();
  m_characterMap.clear();
  m_characterNames.clear();
}

RCPtr<Md2Model> CharacterManager::GetMd2Mesh(const std::string meshName) const
{
  ModelMap::const_iterator it = m_md2Models.find(meshName);
  if (it != m_md2Models.end())
  {
    return it->second;
  }
  return 0;
}

Character* CharacterManager::Create(string name)
{
  if (name == "md2")
  {
    return new CharacterMd2Regular;
  }

  if (name == "md2blinkeyes")
  {
    return new CharacterMd2Chicken;
  }

  return 0;
}

bool CharacterManager::LoadMd2Meshes(File* pf)
{
  // Get number of meshes.
  int numMeshes = 0;
  if (!pf->GetInteger(&numMeshes))
  {
    pf->ReportError("Expected number of MD2 meshes.");
    return false;
  }

  // For each mesh..
  for (int i = 0; i < numMeshes; i++)
  {
    // Get the mesh name
    string meshName;
    if (!pf->GetDataLine(&meshName))
    {
      pf->ReportError("Expected MD2 mesh filename.");
      return false;
    }
   
    Md2Model* pMesh = new Md2Model; 
    // Load the mesh
    if (!pMesh->Load(meshName))
    {
      pf->ReportError("Failed to load MD2 mesh.");
      return false;
    }

    // Put the mesh in the Map of Md2 meshes.
    m_md2Models[meshName] = pMesh;

    // Check for extra flags for repeat/frozen animations.
    int extra = 0;
    if (!pf->GetInteger(&extra))
    {
      pf->ReportError("Expected extra info flag.");
      return false;
    }
    if (extra == 1)
    {
      // Reset all flags for this mesh
      int na = pMesh->GetNumAnimations();
      for (int n = 0; n < na; n++)
      {
        pMesh->SetDoesRepeat((Md2Model::Animation)n, false);
        pMesh->SetDoesFreeze((Md2Model::Animation)n, false);
      }

      // Load repeat flags
      int numRepeats = 0;
      if (!pf->GetInteger(&numRepeats))
      {
        pf->ReportError("Expected number of repeat flags.");  
        return false;
      }
      std::string s;     
      for (int j = 0; j < numRepeats; j++)
      {
        if (!pf->GetDataLine(&s))
        {
          pf->ReportError("Expected animation name for repeat flag.");
          return false;
        }
        pMesh->SetDoesRepeat(pMesh->GetAnimationFromName(s), true);
      }

      // Load freeze flags
      int numFreezes = 0;
      if (!pf->GetInteger(&numFreezes)) 
      {
        pf->ReportError("Expected number of freeze flags.");
        return false;
      }
      for (int k = 0; k < numFreezes; k++)
      {
        if (!pf->GetDataLine(&s))
        {
          pf->ReportError("Expected animation name for freeze flag.");
          return false;
        }
        pMesh->SetDoesFreeze(pMesh->GetAnimationFromName(s), true);
      }
 
    }
#if defined _DEBUG && defined CHARACTER_MANAGER_SHOW_INFO
    std::cout << "Animation info:\n";
    int na = pMesh->GetNumAnimations(); 
    for (int i = 0; i < na; i++)
    {
      std::cout << pMesh->GetAnimationName((Md2Model::Animation)i);
      std::cout << "\t";
      if (pMesh->GetDoesActionRepeat((Md2Model::Animation)i))
      {
        std::cout << "REPEATS\t";
      }
      if (pMesh->GetDoesActionFreeze((Md2Model::Animation)i))
      {
        std::cout << "FREEZES";
      }
      std::cout << "\n"; 
    }
#endif
  }
  return true;
}

bool CharacterManager::LoadCharacters(const string& charList)
{
  Clear();
    
  File f;
  if (!f.OpenRead(charList))
  {
    f.ReportError("Couldn't open character list file.");
    return false;
  }

  // Step 1: Load all meshes. These are stored in a map, and Characters point to
  // the single instance of a particular mesh. (Or a Character subclass could 
  // actually point to lots of meshes.)
  if (!LoadMd2Meshes(&f))
  {
    f.ReportError("Failed to load character meshes.");
    return false;
  }

  // (TODO more functions as more file formats are supported.)

  // Step 2: load each character.
  // These can be different subtypes of Character, so use factory function.

  // Get number of characters
  int numCharacters = 0;
  if (!f.GetInteger(&numCharacters))
  {
    f.ReportError("Expected number of characters.");
    return false;
  }

  // For each character..
  for (int i = 0; i < numCharacters; i++)
  {
    // Get the ID name and type (MD2 etc)
    string id;
    if (!f.GetDataLine(&id))
    {
      f.ReportError("Expected character ID.");
      return false;
    }

    if (m_characterMap.find(id) != m_characterMap.end())
    {
      string error = "Non-unique character ID " + id;
      f.ReportError(error);
      return false;
    }

    string charType;
    if (!f.GetDataLine(&charType))
    {
      f.ReportError("Expected character type.");
      return false;
    }

    // Create new Character subtype 
    RCPtr<Character> pCharacter = Create(charType);
    if (!pCharacter.GetPtr())
    {
      f.ReportError("Bad character type.");
      return false;
    }

    if (!pCharacter->Load(&f))
    {
      string error = "Failed to load character " + id;
      f.ReportError(error);
      return false;
    }

    pCharacter->SetName(id);
    m_characterMap[id] = pCharacter;
    m_characterNames.push_back(id);
  }
  return true;
}

Character* CharacterManager::GetCharacter(const string& characterName)
{
  RCPtr<Character> pCharacter = m_characterMap[characterName];
  if (pCharacter.GetPtr())
  {
    return pCharacter->Clone();
  }
  // Oops, character name not in map.
  return 0;
}

CharacterManager::Iterator CharacterManager::Begin()
{
  Assert(m_characterNames.size() == m_characterMap.size());
  return m_characterNames.begin();
}

CharacterManager::Iterator CharacterManager::End()
{
  Assert(m_characterNames.size() == m_characterMap.size());
  return m_characterNames.end();
}

int CharacterManager::NumCharacters() const
{
  Assert(m_characterNames.size() == m_characterMap.size());
  return m_characterNames.size();
}

}

