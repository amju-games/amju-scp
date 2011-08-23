/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: SpecialRecipient.h,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#if !defined(SPECIAL_RECIPIENT_H_INCLUDED)
#define SPECIAL_RECIPIENT_H_INCLUDED

#include "MessageRecipient.h"

namespace Amju
{
class File;

// A server owned by the Engine, which recieves messages.
class SpecialRecipient : public MessageRecipient
{
public:
  virtual void Draw() {}
  virtual void Update() {}
  virtual void DrawOverlays() {}
  virtual bool Load() { return true; }
};

typedef SharedPtr<SpecialRecipient> PSpecialRecipient;
typedef std::map<int, PSpecialRecipient> SpecialRecipientMap;
}

#endif

