/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MessageSerializer.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_MESSAGE_SERIALIZER_H_INCLUDED)
#define SCHMICKEN_MESSAGE_SERIALIZER_H_INCLUDED

#include "Message.h"
#include "SerialInterface.h"
#include "SharedPtr.h"

namespace Amju
{
class MessageSerializer : public Shareable
{
public:
  MessageSerializer(SharedPtr<SerialInterface> s);
  
  PMessage ReadMessage();
  bool WriteMessage(PMessage m);

private:
  PMessage Create(int messageType);

  SharedPtr<SerialInterface> m_pSerial;
};
}

#endif

