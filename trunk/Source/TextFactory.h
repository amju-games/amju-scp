/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextFactory.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_TEXT_FACTORY_H_INCLUDED)
#define SCHMICKEN_TEXT_FACTORY_H_INCLUDED

#include "Singleton.h"
#include "SolidComponent.h"
#include "TextColourizer.h"

namespace Amju
{
class TextCurver;

// Creates a Solid Composite of 3D letters, given a string. 
class SingleTextFactory 
{
public:
  // Create a Composite from a text string. You can use the current colourizer
  // or override it by specifying your own.
  // spacing: spacing between characters is multiplied by this parameter.
  //  So increase it for wider spaces between characters, decrease it to
  //  scrunch characters together.
  // Curver: specify this if you want to curve the text.
  PSolidComponent Create(
    const std::string& text,
    TextColourizer* pColourizer = 0,
    bool centre = true,
    float spacing = 1.0f,
    TextCurver* pCurver = 0);

  // Create text split into multiple lines. The text is broken into lines
  // using a special character in the text.
  PSolidComponent CreateMultiline(
    const std::string& text,
    TextColourizer* pColourizer = 0,
    bool centre = true,
    float spacing = 1.0f,
    TextCurver* pCurver = 0);

  void Init();

  float GetCharacterWidth(char c);
  float GetCharacterWidth(PSolidComponent p);

  TextColourizer* GetCurrentColourizer();
  // Set the colourizer by index. The indices are set up in Init().
  void SetCurrentColourizer(int);

protected:
  std::vector<SharedPtr<TextColourizer> > m_colourizers;
  int m_colourizerIndex;
};

typedef Singleton<SingleTextFactory> TextFactory;
}

#endif

