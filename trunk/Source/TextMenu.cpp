/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextMenu.cpp,v $
Revision 1.1.10.1  2007/07/15 21:55:28  Administrator
MSVC warnings

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include "TextMenu.h"
#include "SolidComponent.h"
#include "File.h"
#include "Engine.h"
#include "Text3dLeaf.h"

using namespace std;
namespace Amju
{
bool TextMenu::Load(File* pf)
{
  // Get number of items.
  int numItems = 0;
  if (!pf->GetInteger(&numItems))
  {
    pf->ReportError("Expected number of menu items.");
    return false;
  }

  // Get filename of items. The file lives in the <language> directory.
  std::string textFileName;
  if (!pf->GetDataLine(&textFileName))
  {
    pf->ReportError("Expected menu text filename.");
    return false;
  }

  // Open the text file.
  File textFile;
  string lang = Engine::Instance()->GetConfigValue("language");
  string dash = "-"; //slash = Engine::Instance()->GetConfigValue("slash");
  string langFileName = lang + dash + textFileName;
  
  if (!textFile.OpenRead(langFileName))
  {
    textFile.ReportError("Couldn't open menu text file.");
    return false;
  }
  // Get each item. 
  for (int i = 0; i < numItems; i++)
  {
    // Get coords and text for each item.
    SharedPtr<Text3dLeaf> pLeaf1 = new Text3dLeaf; // Real type of Text leaf
    if (!pLeaf1->Load(&textFile))
    {
      textFile.ReportError("Failed to load menu text.");
      return false;
    }
    SolidComposite* pComp = new SolidComposite;
    pComp->AddComponent(pLeaf1.GetPtr());
    pComp->SetOrientation(Orientation(0, 0, 0, 90.0f, 0, 0));   
    AddItem(pComp);

  }
  return true;
}
}

