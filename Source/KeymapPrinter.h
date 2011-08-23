/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: KeymapPrinter.h,v $
Revision 1.1  2004/09/08 15:42:51  jay
Added to repository
  
*/

#if !defined(KEYMAP_PRINTER_H_INCLUDED)
#define KEYMAP_PRINTER_H_INCLUDED

#include "Singleton.h"
#include "TexturedQuad.h"
#include "TextLeaf.h"
#include "Effect.h"

namespace Amju
{
// Draws the keys, so the user can see which keys do what. The display is
// hard coded here. The keys are configurable though.
class KeymapPrinter
{
public:
  bool Init();

  void Draw(float x, float y);

  void SetEffect(Effect*);

  // Show effect centred on the key with character c on it.
  void ShowEffect(char c);

  enum Mode { JOYSTICK, KEYS };
  void SetMode(Mode);
  Mode GetMode() const;

protected:
  
  void DrawKeyboard(float x, float y);

  void DrawJoystick(float x, float y);

  void DrawOneKey(
    char k, // key character 
    float r, float g, float b, // key colour (R, G, B)
    bool black, // if true, letter colour is black, else white
    float x, float y, // position of key
    float size); // size of key

  // Keyboard key quad, to show which key to press, etc.
  TexturedQuad m_buttonQuad;

  TextLeaf m_textAction;
  TextLeaf m_textView;
  TextLeaf m_textMenu;

  // Joysick buttons A and B. These are shown instead of keys if j/s enabled.
  TexturedQuad m_joyAQuad;
  TexturedQuad m_joyBQuad;

  // Text for joystick buttons
  TextLeaf m_joyTextAction;
  TextLeaf m_joyTextView;

  // This effect is used to indicate when a key mapping has changed.
  SharedPtr<Effect> m_pEffect;

  Mode m_mode;
};

typedef Singleton<KeymapPrinter> TheKeymapPrinter;
}

#endif

