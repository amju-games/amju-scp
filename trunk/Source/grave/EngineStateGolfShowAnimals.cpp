/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStateGolfShowAnimals.cpp,v $
Revision 1.1.2.2  2006/08/14 17:50:10  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:29  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1  2004/09/08 15:43:05  jay
Added to repository
  
*/

#include "EngineStatePoolShowAnimals.h"
#include "Engine.h"
#include "Stupid.h"
#include "Level.h"

namespace Amju
{
const char* EngineStatePoolShowAnimals::Name = "golf_show_animals"; 
// Register this State with the Engine.
static const bool registeredgolfanimals = Engine::Instance()->
  RegisterEngineState(EngineStatePoolShowAnimals::Name, new EngineStatePoolShowAnimals);

EngineStatePoolShowAnimals::EngineStatePoolShowAnimals()
{
  m_maxTime = 10.0f;  
}

bool EngineStatePoolShowAnimals::Load()
{
  return true;
}


void EngineStatePoolShowAnimals::SetActive(bool active)
{
  if (!active)
  {
    return;
  }
  m_time = 0;
  m_animals.clear();
  // Get the animals in this level
  GameObjectMap& objs = Engine::Instance()->GetGameObjects();
  // Get the Stupids
  for (GameObjectMap::iterator it = objs.begin(); it != objs.end(); ++it)
  {
    GameObject* pObj = it->second.GetPtr();
    if (dynamic_cast<Stupid*>(pObj))
    { 
      CharacterGameObject* pChar = (CharacterGameObject*)(pObj);
      m_animals.push_back(pChar);
      pChar->SetForwardVel(5.0f); // TODO TEMP TEST

    }
  }
 
  // Set the same velocity for each one
  
  // Set position of each one, so they are in a row, following each other



}

void EngineStatePoolShowAnimals::Draw()
{
  // Draw the animals - we must do this explicitly because they may not all
  // be in the same room.
  for (Animals::iterator it = m_animals.begin(); it != m_animals.end(); ++it)
  {
    (*it)->Draw();
  }
}

void EngineStatePoolShowAnimals::Update()
{
  m_time += Engine::Instance()->GetDeltaTime();

  // Draw the animals - we must do this explicitly because they may not all
  // be in the same room.
  for (Animals::iterator it = m_animals.begin(); it != m_animals.end(); ++it)
  {
    (*it)->Update();
  }
}
}



