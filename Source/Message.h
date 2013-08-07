/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Message.h,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if !defined(MESSAGE_H_INCLUDED)
#define MESSAGE_H_INCLUDED

#include "GameObject.h"
#include "RCPtr.h"
#include <iostream>
#include <string>
#include <vector>

namespace Amju
{
// Messages are sent from Game Objects to Game Objects. They may be sent
// immediately, or after some time interval.
// This idea comes from Game Programming Gems. 
class Message : public RefCounted
{
public:
  // Create a null message, used when we load the message data from file.
  Message();

  // Create a message in code.
  Message(const std::string& name, // name for debugging
          float timeDelay,  // time delay (Secs) before message will be received
          int senderId, // ID of sender
          int recId,  // ID of recipient
          int code); // the message code

  virtual ~Message();

  virtual Message* Clone();

  virtual void Print(std::ostream&) const;

  bool Load(File*); 
#ifdef SCENE_EDITOR
  bool Save(File*); 
#endif

  void SetSenderId(int id) { m_sendId = id; }

  void SetRecipientId(int id) { m_recId = id; }

  // Call to set the real time when the message should be received, in seconds 
  // elapsed since process started.
  void SetRealTime(float realElapsedSeconds);

  float GetRealTime() const { return m_realTime; }

  std::string GetName() const { return m_name; }
  int GetRecipientId() const { return m_recId; }
  int GetCode() const { return m_code; }

  // Returns true if there should be no time delay between sending and processing.
  bool NoDelay() const { return m_interval == 0; }

  // Output for debugging
  friend std::ostream& operator<<(std::ostream& os, const Message& m);

protected:
  // Load code of message. Subclasses can override this to load more exciting
  // message content.
  virtual bool LoadCode(File* pF);

  // Human-readable message name (usually used for debugging, but actually
  // used for e.g. the SoundServer).
  std::string m_name;

  // Time interval between sending and receiving of this message.
  // Zero if no interval required.
  float m_interval;

  // Real Time is set when the message is sent, by adding the current time to 
  // the interval.
  float m_realTime;

  // Id of Game Object sending the message.
  int m_sendId;

  // Id of recipient Game Object.
  int m_recId;

  // Message code.
  // For a simple Game Object with no state machine, this code can simply be
  // treated as the next state. For a Game Object with FSM, this code is the
  // input to the FSM.
  int m_code;

  // Unique message ID, for tracing/debugging.
  static int s_id;
  int m_id; 


#if defined(SCENE_EDITOR)
public:
  void SetName(const std::string& name) { m_name = name; }
  void SetDelay(float d) { m_realTime = d; }
  void SetRecipient(int r) { m_recId = r; }
  void SetCode(int code) { m_code = code; }

  float GetDelay() const { return m_interval; }

#endif
};

typedef RCPtr<Message> PMessage;
}
#endif

