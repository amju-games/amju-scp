/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextColourizer.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#include "TextColourizer.h"

namespace Amju
{
SolidComposite* TextColourizer::ColourizeCharacter(
  SolidComponent* pChar)
{
  SolidComposite* pCompositeChar = dynamic_cast<SolidComposite*>(pChar);
  if (pCompositeChar)
  {
    SolidComposite* pResult = new SolidComposite;
    pResult->SetOrientation(*(pChar->GetOrientation()));
    int numChars = pCompositeChar->GetNumChildren();
    for (int i = 0; i < numChars; i++)
    {
      PSolidComponent pChild = pCompositeChar->GetChild(i);
      PSolidComponent pDecorated = ColourizeElement(pChild);
      pResult->AddComponent(pDecorated);
    }
    return pResult;
  }

  SolidComposite* pResult = new SolidComposite;
  pResult->SetOrientation(*(pChar->GetOrientation()));
  PSolidComponent pDecorated = ColourizeElement(pChar);
  pResult->AddComponent(pDecorated);
  return pResult;
}
}
