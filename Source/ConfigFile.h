/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ConfigFile.h,v $
Revision 1.1  2004/09/08 15:43:17  jay
Added to repository
  
*/

#if !defined(CONFIG_FILE_H_INCLUDED)
#define CONFIG_FILE_H_INCLUDED

// Designed to get game configuration, e.g. other directories, etc.
#include <map>
#include <string>

namespace Amju
{
class ConfigFile
{
public:
  ConfigFile();
  virtual ~ConfigFile();

  bool Load(const std::string& filename);
  bool Save(const std::string& filename);

  std::string GetValue(const std::string& key) const;
  float GetFloat(const std::string& key) const;

  bool Exists(const std::string& key) const;

  void Set(const std::string& key, const std::string& value);

  void Erase(const std::string& key);

  void Clear();

  int GetChecksum() const;

protected:
  typedef std::map<std::string, std::string> ConfigMap;
  ConfigMap m_values;
};
}
#endif

