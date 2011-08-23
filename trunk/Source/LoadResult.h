/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LoadResult.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#ifndef LOAD_RESULT_H_INCLUDED
#define LOAD_RESULT_H_INCLUDED

namespace Amju
{
struct LoadResult
{
  enum Result { FAIL, OK, MORE };

  LoadResult(bool b)
  {
    if (b)
    {
      m_result = OK;
    }
    else
    {
      m_result = FAIL;
    }
  }

  LoadResult(Result r) : m_result(r) {}

  bool HasMore() const { return m_result == MORE; }
  bool Failed() const { return m_result == FAIL; }
  bool IsOk() const { return m_result == OK; }
    
  Result m_result;
};
}

#endif


