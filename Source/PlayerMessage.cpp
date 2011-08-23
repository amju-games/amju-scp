/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PlayerMessage.cpp,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#include "PlayerMessage.h"

namespace Amju
{
PlayerMessage::PlayerMessage(
  const std::string& name, // name for debugging
  float timeDelay,  // time delay (Secs) before message will be received
  int senderId, // ID of sender
  int recId,  // ID of recipient
  int code, // the message code
  float arg) :// extra argument
  Message(name, timeDelay, senderId, recId, code),
  m_arg(arg)
{
  m_id = s_id++;
}

void PlayerMessage::Print(std::ostream& os) const
{
  Message::Print(os);
  os << " Arg: "
     << m_arg;
}

}

