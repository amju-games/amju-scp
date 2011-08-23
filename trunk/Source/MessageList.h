/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageList.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_MESSAGE_LIST_H_INCLUDED)
#define SCHMICKEN_MESSAGE_LIST_H_INCLUDED

#include "Message.h"

namespace Amju
{
class MessageList : public std::vector<PMessage>
{
public:
  void SendMessages() const;

  // The ID of the object which owns this message list is used
  // as the sender ID for each message.
  bool Load(File* pf, GameObjectId ownerId);

#ifdef SCENE_EDITOR
public:
  bool Save(File*);
#endif
};
}

#endif




