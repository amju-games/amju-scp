/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Tokeniser.cpp,v $
Revision 1.1.10.1  2005/08/15 16:02:23  jay
Splits tokens when a Tab is found, in addition to space and colon.

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Tokeniser.h"
#include "File.h" 
#include "SchAssert.h"
#include "StringUtils.h"

using namespace std;

namespace Amju
{
bool Tokeniser::Tokenise(string* pTail, string* pHead)
{
  Assert(pTail);
  Assert(pHead);
  if (!pTail || !pHead)
  {
    return false;
  }

  Trim(pTail); 
  
  if (pTail->empty())
  {
    return false;
  }
  
  unsigned int i = 0;
  while (i < pTail->size())
  {
    char c = pTail->at(i);
    i++;
    if (c == ' ' || c == ':' || c == '\t')
    {
      if (pTail->size() > 1)
      {
        *pHead = pTail->substr(0, i);
        *pTail = pTail->substr(i);
        Trim(pHead);
        Trim(pTail);
        return true;
      }
      else
      {
        Assert(0);
      }
    }
  }
  *pHead = *pTail;
  *pTail = "";
  return true;
}
}
