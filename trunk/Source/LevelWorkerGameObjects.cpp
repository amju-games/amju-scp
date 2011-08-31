/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerGameObjects.cpp,v $
Revision 1.1.10.1  2007/03/09 21:48:21  jay
Turn off debug msg

Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelWorkerGameObjects.h"
#include "Level.h"
#include "File.h"
#include "GameObjectFactory.h"
#include "StringUtils.h"
#include "Engine.h"
#include "LevelFactory.h"

namespace Amju
{
const char* LevelWorkerGameObjects::TypeName = "game_objects";
const char* LevelWorkerGameObjects::GetTypeName() { return TypeName; }

bool LevelWorkerGameObjects::Load(File* pf)
{
  int id = 0;
  if (!pf->GetInteger(&id))
  {
    pf->ReportError("Expected Game Object ID.");
    return false;
  }

  std::string name;
  if (!pf->GetDataLine(&name))
  {
    pf->ReportError("Expected game object name.");
    return false;
  }

#ifdef LOAD_OBJECT_DEBUG
  std::string info = "Info: Level Worker loading object ";
  info += ToString(id);
  info += ": ";
  info += name;
  pf->ReportError(info);
#endif

  m_pGo = GameObjectFactory::Instance()->Create(name);
  if (!m_pGo.GetPtr())
  {
    pf->ReportError("Bad type name.");
    return false;
  }

  m_pGo->SetId(id);

  int room = 0;
  if (!pf->GetInteger(&room))
  {
    pf->ReportError("Expected room ID for Game Object.");
    return false;
  }
  m_pGo->SetRoomId(room);

  if (!m_pGo->Load(pf))
  {
    pf->ReportError("Failed to load game object.");
    return false;
  }

  // We expect "end" at the end of each object
  std::string endTest;
  if (!pf->GetDataLine(&endTest))
  {
    pf->ReportError("Expected \"end\".");
    return false;
  }
  if (endTest != "end")
  {
    pf->ReportError("Expected \"end\".");
    return false;
  }

  return true;
}

#ifdef SCENE_EDITOR
bool LevelWorkerGameObjects::Save(File* pf)
{
  pf->WriteInteger(m_pGo->GetId());
  pf->Write(m_pGo->GetTypeName());
  pf->WriteInteger(m_pGo->GetRoomId());

  if (!m_pGo->Save(pf))
  {
    return false;
  }
  pf->Write("end");
  return true;
}
#endif

void LevelWorkerGameObjects::SetGameObject(PGameObject p)
{
  m_pGo = p;
}

bool LevelWorkerGameObjects::AddFeature(Level* pLevel)
{
  m_pGo->SetLevel(pLevel);
  //m_pGo->SetRoomId(pLevel->GetRoom()->GetId());
  // Use this function so we keep track of which LevelWorker added which object.
  AddGameObject(pLevel, m_pGo.GetPtr());

  return true;
}

bool LevelWorkerGameObjects::RemoveFeature()
{
  // Remove this worker from the ID->Worker map maintained by the factory.
  GetFactory()->ClearWorkerForObject(m_pGo->GetId());
  // Remove the object.
  Engine::Instance()->ClearGameObject(m_pGo->GetId());
  return true;
}

}

