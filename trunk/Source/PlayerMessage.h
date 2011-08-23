/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PlayerMessage.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_PLAYER_MESSAGE_H_INCLUDED)
#define SCHMICKEN_PLAYER_MESSAGE_H_INCLUDED

#include "Message.h"

namespace Amju
{
class PlayerMessage : public Message
{
public:
  PlayerMessage(const std::string& name, // name for debugging
                float timeDelay,  // time delay (Secs) before message will be received
                int senderId, // ID of sender
                int recId,  // ID of recipient
                int code, // the message code
                float arg); // extra argument

  virtual void Print(std::ostream& os) const;

  float GetArg() const { return m_arg; }

protected:
  float m_arg;

};
}

#endif

