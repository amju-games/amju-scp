/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ProgressCaller.cpp,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#include <iostream>
#include "ProgressCaller.h"

namespace Amju
{
ProgressCaller::ProgressCaller()
{
  m_callback = 0;
}

void ProgressCaller::SetProgressCallback(Callback c)
{
  m_callback = c;
}

void ProgressCaller::IncProgress()
{
  if (m_callback)
  {
    (m_callback)();
  }
  else
  {
    std::cout << "No callback for progress bar.\n";
  }
}



}


