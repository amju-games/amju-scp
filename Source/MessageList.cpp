/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageList.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "MessageList.h"
#include "Engine.h"
#include "File.h"

namespace Amju
{
void MessageList::SendMessages() const
{
  for (const_iterator it = begin(); it != end(); ++it)
  {
    Engine::Instance()->SendGameMessage(*it);
  }
}

bool MessageList::Load(File* pf, GameObjectId ownerId)
{
  // Get number of messages.
  int numMessages = 0;
  if (!pf->GetInteger(&numMessages))
  {
    pf->ReportError("Expected number of messages.");
    return false;
  }

  reserve(numMessages);

  // Get each message.
  for (int i = 0; i < numMessages; i++)
  {
    PMessage m = new Message;
    if (!m->Load(pf))
    {
      pf->ReportError("Failed to load message.");
      return false;
    }
    m->SetSenderId(ownerId);
    push_back(m);
  }
  return true;
}

#ifdef SCENE_EDITOR
bool MessageList::Save(File* pf)
{
  pf->WriteComment("// Message list: no of messages, then each message");
  pf->WriteInteger(size());
  for (int i = 0; i < size(); i++)
  {
    PMessage pm = (*this)[i];
    pm->Save(pf);
  }
  return true;
}
#endif

}

