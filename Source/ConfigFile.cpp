/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ConfigFile.cpp,v $
Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ConfigFile.h"
#include "File.h"
#include "Tokeniser.h"
#include <StringUtils.h>
#include <iostream>

using namespace std;

namespace Amju
{
ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::Erase(const std::string& key)
{
  m_values.erase(key);
}

void ConfigFile::Clear()
{
  m_values.clear();
}

bool ConfigFile::Save(const std::string& filename)
{
  File f(true, File::STD);
  if (!f.OpenWrite(filename))
  {
    f.ReportError("Couldn't open config file for writing.");
    return false;
  }
  // Save each key/value pair.
  for (ConfigMap::iterator it = m_values.begin(); it != m_values.end(); ++it)
  {
    string s = it->first;
    s += " ";
    s += it->second;
    f.Write(s);
#ifdef _DEBUG
    std::cout << "Config file: saving: " << s.c_str() << std::endl;
#endif
  }
  return true;
}

bool ConfigFile::Load(const std::string& filename)
{
  // Ctor args: arg 1 (true) => has version info
  // arg2 (false) => don't use Glue File implementation.
  File f(true, File::STD);

  if (!f.OpenRead(filename))
  {
#ifdef _DEBUG
    // This is ok for a clean install, so don't complain in a release build.
    f.ReportError("Couldn't open config file.");
#endif
    return false;
  }
  // This config file simply consists of pairs of tokens. The first of each 
  // pair is the key; the second is the value.
  Tokeniser toker;

  string configLine;
  while (f.GetDataLine(&configLine))
  {
    string key;
    string value;

    if (!toker.Tokenise(&configLine, &key))
    {
      string error = "No value for " + key + " in config file."; 
      f.ReportError(error);
      return false;
    }
    // Tokeniser chops head (the key) off configLine, leaving the value tail.
    value = configLine;

    // Set value in map.
    Set(key, value);

  }
  // No more tokens. This is ok, we have finished.
  return true;
}

std::string ConfigFile::GetValue(const std::string& key) const
{
  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return "";
  }
  return it->second;
}

float ConfigFile::GetFloat(const std::string& key) const
{
  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return 0;
  }
  return ToFloat(it->second.c_str());
}

bool ConfigFile::Exists(const std::string& key) const
{
  ConfigMap::const_iterator it = m_values.find(key);
  if (it == m_values.end())
  {
    return false;
  }
  return true;
}

void ConfigFile::Set(const std::string& key, const std::string& value)
{
  if (!key.empty())
  {
    m_values[key] = value;
  }
}

int ConfigFile::GetChecksum() const
{
  // Add up all the characters in the values.
  int t = 0;
  for (ConfigMap::const_iterator it = m_values.begin(); it != m_values.end(); ++it)
  {
    const std::string& s = it->second;
    for (unsigned int i = 0; i < s.size(); i++)
    {
      t += (i + 1) * (int)s[i];
    }
  }
  return t;
}
}
