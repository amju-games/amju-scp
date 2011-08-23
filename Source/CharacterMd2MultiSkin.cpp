/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2MultiSkin.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CharacterMd2MultiSkin.h"
#include "Texture.h"
#include "TextureServer.h"
#include "File.h"

namespace Amju
{
bool CharacterMd2MultiSkin::Load(File* pf)
{
  if (!CharacterMd2::Load(pf))
  {
    return false;
  }

  // Load skins
  // Get number of skins
  int numSkins = 0;
  if (!pf->GetInteger(&numSkins))
  {
    pf->ReportError("Expected number of skins.");
    return false;
  }

  m_skins.resize(numSkins);

  for (int i = 0; i < numSkins; i++)
  {
    Texture* pTex = TextureServer::Instance()->Get(pf);
    if (!pTex)
    {
      pf->ReportError("Failed to load skin.");
      return false;
    }  
    m_skins[i] = pTex;
  }
  // Create bindings for all textures.
  CreateSkinBinding();

  return true;
}
}
