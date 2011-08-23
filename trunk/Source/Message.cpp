/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Message.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Message.h"
#include "File.h"
#include "Engine.h"

using namespace std;

namespace Amju
{
int Message::s_id;

ostream& operator<<(ostream& os, const Message& m)
{
  m.Print(os);
  return os;
}

Message::Message() : 
  m_name("NULL MESSAGE"),
  m_interval(0),
  m_sendId(0),
  m_recId(0),
  m_code(0),
  m_realTime(0)
{
  m_id = s_id++;
}

Message::Message(const std::string& name, // name for debugging
        float timeDelay,  // time delay (Secs) before message will be received
        int senderId, // ID of sender
        int recId,  // ID of recipient
        int code) : // the message code.
  m_name(name),
  m_interval(timeDelay),
  m_sendId(senderId),
  m_recId(recId),
  m_code(code),
  m_realTime(0)
{
  m_id = s_id++;
}

Message::~Message()
{
}

void Message::Print(std::ostream& os) const
{
  os << m_id
     << " "
     << m_name.c_str() 
     << " from: " 
     << m_sendId 
     << " to: " 
     << m_recId 
     << " code: " 
     << m_code;
}

Message* Message::Clone()
{
  return new Message(m_name, m_interval, m_sendId, m_recId, m_code);
}

void Message::SetRealTime(float realElapsedSeconds)
{
  m_realTime = m_interval + realElapsedSeconds;
}

bool Message::Load(PSerialInterface p)
{
  if (!p->GetString(&m_name))
  {
    return false;
  }
  if (!p->GetFloat(&m_interval))
  {
    return false;
  }
  if (!p->GetInteger(&m_sendId))
  {
    return false;
  }
  if (!p->GetInteger(&m_recId))
  {
    return false;
  }
  if (!p->GetInteger(&m_code))
  {
    return false;
  }
  return true;
}

bool Message::Save(PSerialInterface p)
{
  if (!p->WriteString(m_name))
  {
    return false;
  }
  if (!p->WriteFloat(m_interval))
  {
    return false;
  }
  if (!p->WriteInteger(m_sendId))
  {
    return false;
  }
  if (!p->WriteInteger(m_recId))
  {
    return false;
  }
  if (!p->WriteInteger(m_code))
  {
    return false;
  }
  return true;
}

// TODO Deprecated, should use the SerialInterface version.
bool Message::Load(File* pf)
{
  // Get the name
  if (!pf->GetDataLine(&m_name))
  {
    pf->ReportError("Expected message name.");
    return false;
  }
  // Get the interval
  if (!pf->GetFloat(&m_interval))
  {
    pf->ReportError("Expected message time interval.");
    return false;
  }
  // Get the recip. ID
  if (!pf->GetInteger(&m_recId))
  {
    pf->ReportError("Expected message recipient ID.");
    return false;
  }

  // Get the code
  return LoadCode(pf);
}

#ifdef SCENE_EDITOR
bool Message::Save(File* pf)
{
  pf->WriteComment("// Messsage name");
  pf->Write(m_name);
  pf->WriteComment("// Message interval");
  pf->WriteFloat(m_interval);
  pf->WriteComment("// Message recipient ID");
  pf->WriteInteger(m_recId);
  // TODO should we have virtual SaveCode() ?
  pf->WriteComment("// Message code");
  pf->WriteInteger(m_code);
  return true;
}
#endif

bool Message::LoadCode(File* pf)
{
  if (!pf->GetInteger(&m_code))
  {
    pf->ReportError("Expected message code.");
    return false;
  }
  return true;
}
}
