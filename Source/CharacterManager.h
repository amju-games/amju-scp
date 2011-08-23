/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterManager.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(CHARACTER_MANAGER_H_INCLUDED)
#define CHARACTER_MANAGER_H_INCLUDED

#include "Character.h"
#include "Texture.h"
#include "Md2Model.h"
#include <map>
#include <string>

namespace Amju
{
// CharacterManager
// Maintains list of available character types, and stores attributes that
// apply to a character type. Attributes for the individual characters 
// (like position) are stored in each Character object.
class CharacterManager
{
public:
  // Singleton
  static CharacterManager* Instance();

  // Create a new character. The attributes for a character type are the same
  // for each instance. E.g. the actions available to a Chicken character are
  // the same for all Chicken instances.
  // However the Character object is allocated on the heap and you _should_
  // delete it when done with it.
  // (Or put it in a SharedPtr so it is deleted for you.)

  // TODO This should return a vector of meshes.
  Character* GetCharacter(const std::string& characterName);

  // Called when loading/reloading engine data. Create all available characters.
  // charlist is a text file listing all characters.
  bool LoadCharacters(const std::string& charList);

  // Delete everything, to make sure there are no leaks.
  void Clear();

  // Get an MD2 mesh.
  // TODO Surely this shouldn't be public ??!?!?!
  // It's only used by CharacterMd2
  // TODO sort this out!
  SharedPtr<Md2Model> GetMd2Mesh(const std::string meshName) const;

  // Iterate over character names.
  typedef std::vector<std::string>::iterator Iterator;
  Iterator Begin();
  Iterator End();
  int NumCharacters() const;

private:
  CharacterManager();
  CharacterManager(const CharacterManager&);
  CharacterManager& operator=(const CharacterManager&);

  // Factory function: create Character subclass given type name.
  Character* Create(std::string name);

  // We need a new map and a new loading function like fthis one for each 
  // mesh format we support. Currently only MD2.
  bool LoadMd2Meshes(File* pf);


  // Store each character in this map. When a character is requested,
  // clone the appropriate Character object and return the clone.
  // NB More than one Character may point to the same mesh(es).
  typedef std::map<std::string, SharedPtr<Character> > CharacterMap;
  CharacterMap m_characterMap;

  // Map character ID to an MD2 mesh.
  // If we support more mesh types, we will need a map for each one.
  typedef std::map<std::string, SharedPtr<Md2Model> > ModelMap;
  ModelMap m_md2Models;

  // List of the keys in m_characterMap. This allows clients to 
  // iterate over the available characters.
  std::vector<std::string> m_characterNames;

/*
  Character* CreateMd2Character(const std::string& characterName,
                                const std::string& skinName);


  // Map character ID to a texture.
  typedef std::map<std::string, Texture*> TextureMap;
  TextureMap m_textures;

  // Map character ID to directory. More than one character can come from the
  // same directory, with the same mesh but different skins.
  typedef std::map<std::string, std::string> DirectoryMap;
  DirectoryMap m_dirs;
*/

};
}
#endif

