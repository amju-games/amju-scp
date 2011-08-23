/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Keymap.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(KEYMAP_H_INCLUDED)
#define KEYMAP_H_INCLUDED

#include "ConfigFile.h"
#include <map>
#include <string>

namespace Amju
{
// Maps keybaord keys to functions.
class Keymap
{
public:
  // List of functions which may be mapped to keys.
  // (The functions correspond to the buttons on an N64 controller.)
  enum FunctionName
  {
    PLUS_UP = 0,
    PLUS_DOWN,
    PLUS_LEFT,
    PLUS_RIGHT,
    RED,
    BLUE,
    GREEN,
    YELLOW_UP,
    YELLOW_DOWN,
    YELLOW_LEFT,
    YELLOW_RIGHT,
    Z
  };

  bool Load(const std::string& filename);
  bool Save(const std::string& filename);

  bool OnKey(char key, bool down);

  bool SetKey(char key, FunctionName fn);

  char GetChar(FunctionName f) const;

protected:
  // Map characters to members functions of Engine.
  //typedef void(Engine::*KeyFunc)(bool);
  typedef std::map<char, FunctionName> KeyFuncMap;
  KeyFuncMap m_functions;

  ConfigFile m_config;

  // Map of keys to repeat flags, to prevent auto-repeat getting through to Engine.
  std::map<char, bool> m_repeat;

  std::string m_filename;
};
}

#endif

