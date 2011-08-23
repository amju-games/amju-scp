/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SerialInterface.h,v $
Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_SERIAL_INTERFACE_H_INCLUDED)
#define SCHMICKEN_SERIAL_INTERFACE_H_INCLUDED

#include "SharedPtr.h"
#include <string>

namespace Amju
{
// Interface for classes which read and write data from a serial data source. 
// I.e. Files, sockets.
// Having this base class lets us e.g. serialize a Message to a file
// and socket using the same code.
  class SerialInterface : public Shareable
{
public:
  virtual ~SerialInterface() {}

  virtual bool GetInteger(int*) = 0;
  virtual bool WriteInteger(int i) = 0;
  virtual bool GetFloat(float*) = 0;
  virtual bool WriteFloat(float) = 0;
  virtual bool GetString(std::string*) = 0;
  virtual bool WriteString(const std::string&) = 0;
};

typedef SharedPtr<SerialInterface> PSerialInterface;
}

#endif

