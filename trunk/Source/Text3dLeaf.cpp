/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Text3dLeaf.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Common.h"
#include "Text3dLeaf.h"
#include "TextFactory.h"
#include "File.h"
#include "Engine.h"

namespace Amju
{
Text3dLeaf::Text3dLeaf()
{
}

Text3dLeaf::Text3dLeaf(const char * text)
{
  AddComponent(TextFactory::Instance()->Create(text));
}

PSolidComponent Text3dLeaf::Clone()
{
  return new Text3dLeaf(*this);
}

/*
void Text3dLeaf::Draw()
{
  AmjuGL::PushMatrix();
  // Translate so middle of text is centre; slight y adjust.
  float s = 0;//(float)(m_text.size() ) / 2.0f;
  AmjuGL::Translate(0, 0, 0);

  // Rotate so letters are upright rather than lying down
  //AmjuGL::Rotate(90, 1, 0, 0);

  m_pSolid->Draw();
  AmjuGL::PopMatrix();
}

int Text3dLeaf::CullDraw(const Frustum& f)
{
  return m_pSolid->CullDraw(f);
}

int Text3dLeaf::GetNumberOfPolygons() const
{
  return m_pSolid->GetNumberOfPolygons();
}
*/
bool Text3dLeaf::Load(File* pf)
{
  // Get text
  std::string text;
  if (!pf->GetDataLine(&text))
  {
    pf->ReportError("Expected text.");
    return false;
  }

  // No (x, y) is expected for this.

  AddComponent(TextFactory::Instance()->Create(text));
  return true;
}

bool Text3dLeaf::Load(const std::string& filename)
{
  File f;

  // Prepend language directory.
  std::string lang = Engine::Instance()->GetConfigValue("language");
  std::string dash = "-"; //slash = Engine::Instance()->GetConfigValue("slash");
  std::string langFileName = lang + dash + filename;

  if (!f.OpenRead(langFileName))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }

  return Load(&f);
}

}
