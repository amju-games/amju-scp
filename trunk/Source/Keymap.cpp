/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Keymap.cpp,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Keymap.h"
#include "Engine.h"
#include <string>

using namespace std;

namespace Amju
{
extern void ReportError(const std::string&);

static string toStr(int i)
{
  char buf[10];
  sprintf(buf, "%d", i);
  return string(buf);
}

static char toChar(const string& s)
{
  if (s.empty())
  {
    return ' ';
  }
  if (s.length() > 1)
  {
    // Treat string as ASCII code.
    return char(atoi(s.c_str()));
  }

  return char(tolower(s[0]));
}

bool Keymap::Load(const std::string& filename)
{
  if (!m_config.Load(filename))
  {
    string error = "Failed to load key config file " + filename;
    ReportError(error);
    return false;
  }

  m_functions[toChar(m_config.GetValue(toStr(PLUS_UP)))] = PLUS_UP;
  m_functions[toChar(m_config.GetValue(toStr(PLUS_DOWN)))] = PLUS_DOWN;
  m_functions[toChar(m_config.GetValue(toStr(PLUS_LEFT)))] = PLUS_LEFT;
  m_functions[toChar(m_config.GetValue(toStr(PLUS_RIGHT)))] = PLUS_RIGHT;
  m_functions[toChar(m_config.GetValue(toStr(RED)))] = RED;
  m_functions[toChar(m_config.GetValue(toStr(BLUE)))] = BLUE;
  m_functions[toChar(m_config.GetValue(toStr(GREEN)))] = GREEN;
  m_functions[toChar(m_config.GetValue(toStr(YELLOW_UP)))] = YELLOW_UP;
  m_functions[toChar(m_config.GetValue(toStr(Z)))] = Z;

  m_filename = filename;
  return true;
}

bool Keymap::Save(const std::string& filename)
{
  bool b = m_config.Save(filename);
  if (b)
  {
    m_filename = filename;
  }
  return b;
}

bool Keymap::OnKey(char key, bool down)
{
  KeyFuncMap::iterator it = m_functions.find(char(tolower(key)));
  if (it == m_functions.end())
  {
    return false;
  }

  // Prevent auto-repeat reaching the Engine
  if (!down)
  {
    m_repeat[char(tolower(key))] = false;
  }
  else 
  {
    if (m_repeat[char(tolower(key))])
    {
      return true;
    }
    m_repeat[char(tolower(key))] = true;
  }

  FunctionName fn = it->second;

  Engine* e = Engine::Instance();

  switch (fn)
  {
  case PLUS_UP:
    e->PlusUp(down);
    break;
  case PLUS_DOWN:
    e->PlusDown(down);
    break;
  case PLUS_LEFT:
    e->PlusLeft(down);
    break;
  case PLUS_RIGHT:
    e->PlusRight(down);
    break;
  case RED:
    e->Red(down);
    break;
  case BLUE:
    e->Blue(down);
    break;
  case GREEN:
    e->Green(down);
    break;
  case YELLOW_UP:
    e->YellowUp(down);
    break;
  case YELLOW_DOWN:
    e->YellowDown(down);
    break;
  case YELLOW_LEFT:
    e->YellowLeft(down);
    break;
  case YELLOW_RIGHT:
    e->YellowRight(down);
    break;
  case Z:
    e->Z(down);
    break;
  }

  //(e->*f)(down);
  return true;
}

bool Keymap::SetKey(char key, FunctionName fn)
{
  key = tolower(key);
  if (m_functions.find(key) != m_functions.end())
  {
    // Key is already mapped.
    // Swap the already-mapped function with the current key for fn.
    return false;
  }

  // The old key for the function, which will soon have no effect
  string oldKey = m_config.GetValue(toStr(fn));
  char oldCh = atoi(oldKey.c_str());

  m_config.Set(toStr(fn), toStr(int(key)));
  // Erase old key for the function
  m_functions.erase(oldCh);

  m_functions[key] = fn;
  Save(m_filename);
  return true;
}

char Keymap::GetChar(FunctionName f) const
{
  return toChar(m_config.GetValue(toStr((int)f)));
}
}
