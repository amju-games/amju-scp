/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ProgressCaller.h,v $
Revision 1.1  2004/09/08 15:42:55  jay
Added to repository
  
*/

#ifndef PROGRESS_CALLER_H_INCLUDED
#define PROGRESS_CALLER_H_INCLUDED

namespace Amju
{
// Mix-in class for classes which update a progress bar. 
class ProgressCaller
{
public:
  ProgressCaller();

  typedef void (*Callback)();
  void SetProgressCallback(Callback);
  void IncProgress();

protected:
  Callback m_callback;
};
}

#endif


