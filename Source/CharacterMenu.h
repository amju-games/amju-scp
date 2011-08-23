/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMenu.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_CHARACTER_MENU_H_INCLUDED)
#define SCHMICKEN_CHARACTER_MENU_H_INCLUDED

#include <vector>
#include "Menu.h"
#include "CharacterGameObject.h"

namespace Amju
{
// Menu for choosing a character.
class CharacterMenu : public Menu
{
public:
  CharacterMenu();

  virtual int Size() const;
  virtual void Clear();
  void AddCharacter(CharacterGameObject*);

protected:
  virtual void DrawItem(int i);
  virtual void PickDrawItem(int i);

  //typedef std::vector<SharedPtr<Character> > CharVec;
  //CharVec m_chars;

  // Characters which appear under the title
  std::vector<SharedPtr<CharacterGameObject> > m_chars;

};
}

#endif

