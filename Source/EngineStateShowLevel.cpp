/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateShowLevel.cpp,v $
Revision 1.1  2004/09/08 15:42:37  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStateShowLevel.h"
#include "Engine.h"
#include "FirstPersonCamera.h"
#include "EngineRunning.h"
#include "FunctionFactory.h"
#include "TextFactory.h"
#include "Text3dLeaf.h"
#include "CharacterManager.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
const char* EngineStateShowLevel::Name = "showlevel"; 
// Register this State with the Engine.
static const bool registeredshowlev = Engine::Instance()->
  RegisterEngineState(EngineStateShowLevel::Name, new EngineStateShowLevel);

EngineStateShowLevel::EngineStateShowLevel() 
{
}

CharacterGameObject* GetCharacter(const std::string& charname)
{
  CharacterGameObject* pChar = new CharacterGameObject;
  pChar->AddMesh(CharacterManager::Instance()->GetCharacter(charname));
  return pChar;
}

void EngineStateShowLevel::SetActive(bool active)
{
  if (active)
  {
    m_leveltext = new Text3dLeaf("oh no . out of bounds");
  }
}

void EngineStateShowLevel::Draw()
{
  AmjuGL::PushMatrix();
  //EngineMenuState::Draw();
  // Paint background
  DrawBg();
  m_lighting.DrawLight();
  //m_pMenu->Draw();

  AmjuGL::PopMatrix();
  m_pCam->Update();
  m_pCam->Draw();

  AmjuGL::PushMatrix();

    AmjuGL::Translate(0, 0, -1.0f);
    AmjuGL::PushMatrix();
      m_leveltext->Update(); // "level <n>"
      m_leveltext->Draw();
    AmjuGL::PopMatrix();
    /*
    AmjuGL::PushMatrix();
      AmjuGL::Translate(0, -1.0f, -16.0f); // TODO TEMP TEST
      //AmjuGL::Scale(0.5f, 0.5f, 0.5f);
      m_leveltext2->Update(); // please select character
      m_leveltext2->Draw();
    AmjuGL::PopMatrix();
    */
  AmjuGL::PopMatrix();
}

bool EngineStateShowLevel::Load()
{
  string bg = GetEngine()->GetConfigValue("level_bg");
  if (!m_bg.Load(bg, ""))
  {
    return false;
  }

  return true; 
}

void EngineStateShowLevel::Red(bool down)
{
  if (down)
  {
    // Get the character chosen. Set the Player character to this mesh.
    // TODO Also set other attributes, like max speed etc.
    /*
    int choice = m_pMenu->Choose();
    std::vector<std::string> characters = 
      GetEngine()->GetEngineState()->GetCharacters();
    std::string charName = characters[choice];
    */

    string startWav = GetEngine()->GetConfigValue("start_wav");
    GetEngine()->PlayWav(startWav);


    // Unambiguously tell the Running state it's a new level.
    //((EngineRunning*)GetEngine()->GetEngineState(EngineRunning::Name).GetPtr())->OnNewLevel();

  }
}

}


