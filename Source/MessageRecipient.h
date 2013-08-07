/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageRecipient.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(MESSAGE_RECIP_H_INCLUDED)
#define MESSAGE_RECIP_H_INCLUDED

#include "RCPtr.h"
#include "States.h"
#include <map>

namespace Amju
{
class Message;
class File;

// Can receive messages, and have state. 
class MessageRecipient : public RefCounted
{
public:
  MessageRecipient();
  virtual ~MessageRecipient();

  // Load object from a currently open file. Default implementation does nothing,
  // so you can create objects in code.
  virtual bool Load(File*) { return false; }

  virtual void ReceiveMessage(const Message& m);

  virtual void SetState(State newState);
  State GetState() const { return m_state; }

  // Each message receiver should have a unique ID. 
  void SetId(int id);
  int GetId() const { return m_id; }

  // Subclasses may have a location.
  virtual bool HasLocation() const { return false; }

protected:
  // ID, used to map game objects in levels with the GameState record of
  // each object. 
  int m_id;

private:
  // State of the object.
  // Must be set using SetState(), so Game state is kept in sync.
  State m_state;
};

typedef RCPtr<MessageRecipient> PMessageRecipient;
}
#endif

