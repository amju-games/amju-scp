/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EarthquakeServer.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(EARTHQUAKE_SERVER_H_INCLUDED)
#define EARTHQUAKE_SERVER_H_INCLUDED

#include "SpecialRecipient.h"

namespace Amju
{
// This special recipient triggers an earthquake.
class EarthquakeServer : public SpecialRecipient
{
public:
  virtual void ReceiveMessage(const Message& m);
};
}

#endif


