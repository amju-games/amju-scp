/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HiScoreTable.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <utility>
#include "HiScoreTable.h"
#include "Engine.h"
#include "TextWriter.h"
#include "StringUtils.h"
#include "File.h"
#include "Directory.h"
#include "HttpClient.h"

namespace Amju
{
extern const char* APPLICATION_NAME;

const int SingleHiScoreTable::TABLE_SIZE = 10;

void SingleHiScoreTable::GetScore(int i, int* pScore, std::string* pName)
{
  HiScores::const_iterator it = m_hiScores.begin();
  while (i-- > 0) it++; 
  *pScore = it->first;
  *pName = it->second;
}

void SingleHiScoreTable::Draw()
{
  TextWriter* t = Engine::Instance()->GetTextWriter();
  float i = 4.0f;
  for (HiScores::reverse_iterator it = m_hiScores.rbegin(); it != m_hiScores.rend(); ++it)
  {
    std::string s = ToString(it->first);
    int sp = 9 - s.length();
    if (sp > 0)
    {
      s = std::string(sp, ' ') + s;
    }
    t->Print(5, i, s.c_str());
    t->Print(14, i, it->second.c_str());
    i += 1.0f;
  }
}

void SingleHiScoreTable::MakeDefault()
{
  m_hiScores.clear();
  int score = 0;
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    score += 1000;
    m_hiScores.insert(std::make_pair(score, std::string("amju")));
  }
}

bool SingleHiScoreTable::Load()
{
  File f(true, File::STD); // yes has version info, no to glue file.

  std::string root = File::GetRoot();
  File::SetRoot(GetSaveDir(APPLICATION_NAME), "/");
  if (!f.OpenRead("hiscores.cfg"))
  {
    // Make default hi score table and return.
    MakeDefault();   
    File::SetRoot(root, "/");
    return true;
  } 
  File::SetRoot(root, "/");

  int c = 0; 
  for (int i = 0; i < TABLE_SIZE; i++)
  {
    int score = 0;
    std::string name;
    if (!f.GetInteger(&score))
    {
      MakeDefault();   
      return true;
    }
    if (!f.GetDataLine(&name))
    {
      MakeDefault();   
      return true;
    }
    c ^= score;
    m_hiScores.insert(std::make_pair(score, name));
  }
  int check = 0;
  if (!f.GetInteger(&check))
  {
    MakeDefault();   
    return true;
  }
  if (check != c)
  {
    MakeDefault();   
    return true;
  }

  return true;
}

bool SingleHiScoreTable::Save()
{
  int check = 0;
  File f;
  std::string root = File::GetRoot();
  File::SetRoot(GetSaveDir(APPLICATION_NAME), "/");
  if (!f.OpenWrite("hiscores.cfg"))
  {
    File::SetRoot(root, "/");
    return false;
  }

  for (HiScores::iterator it = m_hiScores.begin(); it != m_hiScores.end(); ++it)
  {
    check ^= it->first;
    f.WriteInteger(it->first);
    f.Write(it->second);
  }
  f.WriteInteger(check);

  File::SetRoot(root, "/");
  
  // Send hi scores to web site. 
  SendToUrl("www.amju.com/cgi-bin/hiscores1.pl");
  return true;
}

bool SingleHiScoreTable::IsHighScore(int score) const
{
  // score bigger than lowest score ?
  int lowest = m_hiScores.begin()->first;

std::cout << "Lowest score in High Score Table is " << lowest << "\n";

  bool b = (score > lowest);
  return b;
}

int SingleHiScoreTable::GetHighest() const
{
  int highest = m_hiScores.rbegin()->first;
  return highest;
}

void SingleHiScoreTable::AddHiScore(int score, const char* name)
{
  m_hiScores.insert(std::make_pair(score, name));
  // Remove lowest item if there are more than 10 elements (or max size of table)

  while  ((int)m_hiScores.size() > TABLE_SIZE)
  {
    m_hiScores.erase(m_hiScores.begin());
  }
}

void SingleHiScoreTable::SendToUrl(const std::string& url)
{
  // Start a new thread so we can return immediately.
  std::string u(url);
  u += "?";
  u += "a=b"; // TODO identify host ?
  for (HiScores::iterator it = m_hiScores.begin(); it != m_hiScores.end(); ++it)
  {
    u += "&"; // separate args
    u += ToString(it->first);
    u += "=";
    // TODO replace any spaces (and other characters ?)
    // Should be done by util function as it's so useful.
    u += it->second;
  }

//std::cout << "URL for hi scores: " << u << "\n";

  RCPtr<HiScoreUrlThread> pThread = new HiScoreUrlThread;
  pThread->SetUrl(u);
  pThread->Start();

}

void SingleHiScoreTable::HiScoreUrlThread::Work()
{
//std::cout << "In thread: URL for hi scores: " << m_url << "\n";

  HttpClient h;
  HttpResult res;
  h.Get(m_url, HttpClient::GET, &res); 
  std::cout << "Url thread has now finished.\n";
}
}
