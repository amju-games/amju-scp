/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SoundServer.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SOUND_SERVER_H_INCLUDED)
#define SOUND_SERVER_H_INCLUDED

#include "SpecialRecipient.h"

namespace Amju
{
// A MessageRecipient which plays WAVs when it gets a message.
class SoundServer : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
};
}

#endif

