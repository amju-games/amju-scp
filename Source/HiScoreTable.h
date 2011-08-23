/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HiScoreTable.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(HI_SCORE_TABLE_H_INCLUDED)
#define HI_SCORE_TABLE_H_INCLUDED

#include <map>
#include <string>
#include "Singleton.h"
#include "Thread.h"

namespace Amju
{
class File;

class SingleHiScoreTable
{
public:
  void Draw();
  bool Load();
  bool Save();
  bool IsHighScore(int score) const;
  int GetHighest() const;
  void AddHiScore(int score, const char* name);

  // Get a score and name, from 0 (best score) to TABLE_SIZE-1 (lowest score) 
  void GetScore(int i, int* pScore, std::string* pName);

  static const int TABLE_SIZE;

  // Send all hi scores to given URL. 
  void SendToUrl(const std::string& url);

protected:
  void MakeDefault();

protected:
  // Thread for sending scores to URL without blocking the main thread.
  class HiScoreUrlThread : public Thread
  {
  public:
    virtual void Work();
    void SetUrl(const std::string& s) { m_url = s; }

  protected:
    std::string m_url;
  }; 

protected:
  typedef std::multimap<int, std::string> HiScores;
  HiScores m_hiScores;
};

typedef Singleton<SingleHiScoreTable> HiScoreTable;
}

#endif
