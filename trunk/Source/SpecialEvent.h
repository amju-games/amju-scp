/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: SpecialEvent.h,v $
Revision 1.1.2.1  2005/10/29 10:15:10  jay
Add support for OS-specific special events.
E.g. Paste command.

*/

#ifndef SPECIAL_EVENT_H_INCLUDED
#define SPECIAL_EVENT_H_INCLUDED

#include "RCPtr.h"

namespace Amju
{
class SpecialEvent : public RefCounted
{
};
}

#endif

