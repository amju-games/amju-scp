/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageSerializer.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
 
*/

#include "MessageSerializer.h"

namespace Amju
{
MessageSerializer::MessageSerializer(SharedPtr<SerialInterface> s) :
  m_pSerial(s)
{
}

PMessage MessageSerializer::ReadMessage()
{
  // Get type ID from serial stream, so we can construct a Message
  // object of the appropriate type.
  int type;
  if (!m_pSerial->GetInteger(&type))
  {
    // Expected type id.
    return 0;
  }

  // Create Message from type ID.
  // TODO
  PMessage m = new Message;
  
  // Load Message members from serial stream.
  if (!m->Load(m_pSerial))
  {
    // Load failed, stream empty or contained wrong kind of data.
  }
  return m;
}

bool MessageSerializer::WriteMessage(PMessage m)
{
  // Write the ID for the message type, so the receving end can construct
  // a Message of the appropriate subtype.
  if (!m_pSerial->WriteInteger(0))
  {
    return false;
  }
  
  // Write the Message to the serial stream.
  return m->Save(m_pSerial);
}
}
