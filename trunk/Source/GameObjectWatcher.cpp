/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectWatcher.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GameObjectWatcher.h"
#include "File.h"
#include "Engine.h"

namespace Amju
{
extern void ReportError(const std::string& error);

GameObjectWatcher::GameObjectWatcher()
{
#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
#endif
}

void GameObjectWatcher::Update()
{
  // We may want to disable the watcher once its condition has been satisfied
  // once. Or we may want to start watching again later -- we don't know. 
  // So one or more of this watcher's
  // messages should be to itself to control what happens next.
  // E.g. to never watch again this game, just send OUT_OF_PLAY to self.
  // To start watching again later, send a timed message.
  if (GetState() == OUT_OF_PLAY)
  {
    return;
  }

  if (m_objective.IsSatisfied())
  {
    m_messages.SendMessages();
  }
}

bool GameObjectWatcher::Load(File* pf)
{
  if (!m_objective.Load(pf))
  {
    ReportError("Failed to load objective.");
    return false;
  }
  return m_messages.Load(pf, GetId());
}

#if defined(SCENE_EDITOR)
bool GameObjectWatcher::Save(File* pf)
{
  pf->WriteComment("// Objective");
  m_objective.Save(pf);

  pf->WriteComment("// Messages");
  m_messages.Save(pf);

  return true; 
}

GameObject::PropertyMap GameObjectWatcher::GetProperties() const
{
  // TODO
  PropertyMap pm;
  return pm;
}

void GameObjectWatcher::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO
}
#endif

const char* GameObjectWatcher::GetTypeName() const
{
  return "watcher";
}

}
