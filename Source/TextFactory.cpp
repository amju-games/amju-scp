/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextFactory.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <ctype.h>
#include "TextFactory.h"
#include "SolidComposite.h"
#include "BoundingBox.h"
#include "TextColourizer.h"
#include "TextColourizerColourList.h"
#include "TextCurver.h"
#include "StringUtils.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
TextColourizer* SingleTextFactory::GetCurrentColourizer()
{
  return m_colourizers[m_colourizerIndex].GetPtr();
}

void SingleTextFactory::Init()
{
  m_colourizerIndex = 2;

  // TODO also set up Chrome colourizer

  TextColourizerColourList* pColourizerColourList1 = new TextColourizerColourList;
  // Minty blue colours
  pColourizerColourList1->AddColour(Colour(0,    1.0f, 1.0f, 1.0f));
  pColourizerColourList1->AddColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
  pColourizerColourList1->AddColour(Colour(0,    0,    1.0f, 1.0f));
  m_colourizers.push_back(pColourizerColourList1);

  TextColourizerColourList* pColourizerColourList2 = new TextColourizerColourList;
  // Lots of colours
  pColourizerColourList2->AddColour(Colour(1.0f, 0,    0,    1.0f)); // red
  pColourizerColourList2->AddColour(Colour(1.0f, 1.0f, 0,    1.0f)); // yel
  pColourizerColourList2->AddColour(Colour(0,    1.0f, 1.0f, 1.0f)); // cyan
  pColourizerColourList2->AddColour(Colour(0,    1.0f, 0,    1.0f)); // gr
  pColourizerColourList2->AddColour(Colour(0,    0,    1.0f, 1.0f)); // blue
  pColourizerColourList2->AddColour(Colour(1.0f, 0.5f, 0,    1.0f)); // orange ?
  pColourizerColourList2->AddColour(Colour(1.0f, 0.5f, 1.0f, 1.0f)); // pinky ?
  m_colourizers.push_back(pColourizerColourList2);

  TextColourizerColourList* pColourizerColourList3 = new TextColourizerColourList;
  pColourizerColourList3->AddColour(Colour(1.0f, 0.0f, 1.0f, 1.0f));
  m_colourizers.push_back(pColourizerColourList3);

}

float SingleTextFactory::GetCharacterWidth(char c)
{
  if (c == ' ') return 0.5f; // TODO CONFIG
 
  return 1.0f; 
}

float SingleTextFactory::GetCharacterWidth(PSolidComponent p)
{
  // Get the width of the solid passed in.
  // TODO cache results in a static map
  BoundingBox b;
  b.SetFromSolid(p.GetPtr());
  float r = b.m_xmax - b.m_xmin;
  Assert(r > 0);
  Assert(r < 100.0f); // sanity check
  return r;
}

PSolidComponent SingleTextFactory::CreateMultiline(
  const std::string& text, 
  TextColourizer* pColourizer,
  bool centre,
  float spacing /* = 1.0f */,
  TextCurver* pCurver /* = 0 */)
{
  // TODO word wrap the text if necessary.
  // Split the text into multiple lines. Create each line of text separately.
  // Join the lines into a Composite.
  // Get the max extent of the text.

  // Multi lines are split using a special character.
  static const char SPLIT_CHAR = '$';
  std::vector<std::string> choppedText = Split(text, SPLIT_CHAR);

  static const float LINE_HEIGHT = 1.6f;

  SolidComposite* pComp = new SolidComposite;

  float z = (float(choppedText.size() - 1)) / 2.0f * LINE_HEIGHT;

  for (unsigned int i = 0; i < choppedText.size(); i++)
  {
    const std::string& thisLine = choppedText[i];

    PSolidComponent lineComp = TextFactory::Instance()->Create(
      thisLine.c_str(), 
      pColourizer,
      centre,
      spacing,
      pCurver);

    lineComp->GetOrientation()->SetY(z);
    
    pComp->AddComponent(lineComp);

    z -= LINE_HEIGHT;
  }
  return pComp;
}

PSolidComponent SingleTextFactory::Create(
  const std::string& text, 
  TextColourizer* pColourizer,
  bool centre,
  float spacing /* = 1.0f */,
  TextCurver* pCurver /* = 0 */)
{
  if (!pColourizer)
  {
    // If colourizer is not overridden, use the currently set colourizer.
    pColourizer = GetCurrentColourizer();
  }

  Matrix m;
  m.SetIdentity();

  RCPtr<SolidComposite> result = new SolidComposite;
  float x = 0; // x-coord of current letter
  float z = 0; // z-coord
  float yRot = 0; // letter rotation 
  for (unsigned int i = 0; i < text.size(); i++)
  {
    // TODO this assumption should be replaced with a map of characters
    // to filenames.
    char c = text[i];
    if (isalnum((int)c) || c == '.' || c == '-' || c == '!' || c == ' ')
    {
      std::string letterfilename = "_a_.comp";
      letterfilename[1] = c;

      if (c == ' ')
      {
        letterfilename = "_space_.comp";
      }

      PSolidComponent letter = SolidComponent::LoadSolid(letterfilename);
      if (letter.GetPtr())
      {
        // Add a colour decorator around the letter.
        PSolidComponent pDecorated = pColourizer->ColourizeCharacter(letter.GetPtr());
        // Set the position of the letter. If a curver is spcified, it
        // curves the text.
        float cx = x;
        if (pCurver)
        {
          pCurver->Curve(x, &cx, &z, &yRot);
        }

        pDecorated->GetOrientation()->SetX(cx);
        pDecorated->GetOrientation()->SetZ(z);
        pDecorated->GetOrientation()->SetYRot(yRot);
	pDecorated->GetOrientation()->SetXRot(90.0f); // TODO TEMP TEST

        result->AddComponent(pDecorated); 


        // Get the width of the character.
        // Must set up abs coords for bounding box code to work.
        letter->RecalculateAbsoluteCoords(m);
        x += spacing * GetCharacterWidth(letter.GetPtr());
      }
      else
      {
        // Space or undefined character.
        x += spacing * GetCharacterWidth(c);
      }
    }
    else
    {
      // Space or undefined character.
      x += spacing * GetCharacterWidth(c);
    }
  }

  // Iterate over items again, and adjust x-coord so text is centred.
  if (centre)
  {
    float half = x / 2.0f;
    int numChildren = result->GetNumChildren();
    for (int j = 0; j < numChildren; j++)
    {
      PSolidComponent pChild = result->GetChild(j);
      pChild->GetOrientation()->SetX(pChild->GetOrientation()->GetX() - half);
    }
  }

  return result.GetPtr();
}
}

