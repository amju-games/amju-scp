/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddCharacters.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_ADD_CHARACTERS_H_INCLUDED)
#define LEVEL_WORKER_ADD_CHARACTERS_H_INCLUDED

#include <string>
#include <vector>
#include "LevelWorker.h"

namespace Amju
{
class LevelWorkerAddCharacters : public LevelWorker
{
public:
  static const char* TypeName;

  LevelWorkerAddCharacters();
  virtual bool AddFeature(Level*, CubeMap* );
  virtual bool Load(File* );
#ifdef SCENE_EDITOR
  virtual bool Save(File* );
#endif
  virtual const char* GetTypeName();

  void AddCharacterName(const std::string& name);

protected:
  typedef std::vector<std::string> Characters;
  // GOLF: Names of golf-playing characters.
  Characters m_charNames;

  // TODO spectator characters too.
};
}

#endif

