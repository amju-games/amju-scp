/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: SpecialEventPaste.h,v $
Revision 1.1.2.1  2005/11/13 20:08:13  jay
Added Paste handlers

*/

#ifndef SPECIAL_EVENT_PASTE_H_INCLUDED
#define SPECIAL_EVENT_PASTE_H_INCLUDED

#include "SpecialEvent.h"

namespace Amju
{
// Holds text on clipboard, which we would like to paste into application.
// Maybe it would be better to use a Variable, so we can handle text
// and also other types, e.g. pictures etc ?
class SpecialEventPaste : public SpecialEvent
{
public:
  SpecialEventPaste(const std::string& text) : m_text(text) {}

  std::string GetText() const { return m_text; }

protected:
  std::string m_text;
};
}

#endif

