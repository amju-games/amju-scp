/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectFactory.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(GAME_OBJECT_FACTORY_H_INCLUDED)
#define GAME_OBJECT_FACTORY_H_INCLUDED

#include <string>
#include <vector>

namespace Amju
{
class PoolGameObject;
// This should simply be a typedef for Factory<GameObject>.
// But Registering functions with the Factory using statics
// isn't working using MSVC.
class PoolGameObjectFactory
{
public:
  static PoolGameObjectFactory* Instance();
  
  PoolGameObject* Create(const std::string& gameObjectName);

private:
  PoolGameObjectFactory();


#if defined(SCENE_EDITOR)
public:
  // Get all strings which are valid for Create().
  // Used by editing tool.
  const std::vector<std::string>& GetTypeNames() const;

private:
  std::vector<std::string> m_typenames;
#endif
};
}
#endif
