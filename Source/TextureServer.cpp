/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureServer.cpp,v $
Revision 1.1.10.2  2007/12/15 21:31:29  Administrator
Don't try to clean up in dtor, crashes on Windows ?

Revision 1.1.10.1  2005/05/08 08:19:45  jay
Need <iostream> for new gcc version

Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "TextureServer.h"
#include "File.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& error);

  
TextureServer* TextureServer::Instance()
{
  static TextureServer ts;
  return &ts;
}

TextureServer::TextureServer()
{
}

TextureServer::~TextureServer()
{
  // No point if we are exiting - and causes crash on Windows?????
  //Clear();
}

void TextureServer::DeleteTexture(PoolTexture* p)
{
  // Very dicey: any existing pointers to the texture will be pointing to
  // garbage!
  // Only use if you are sure there's only one pointer to the texture in use.
  for (TexturesMap::iterator it = m_texmap.begin();
       it != m_texmap.end(); 
       ++it)
  {
    PoolTexture* pt = (*it).second;
    if (pt == p)
    {
      delete pt;
      m_texmap.erase(it);
      return;
    }
  }

}

void TextureServer::Clear()
{
  // Delete all the textures we allocated in Get().
  for (TexturesMap::iterator it = m_texmap.begin();
       it != m_texmap.end(); 
       ++it)
  {
    PoolTexture* pt = (*it).second;
    delete pt;
  }
  m_texmap.clear();
}

void TextureServer::ReloadAll()
{
}

#ifdef SCENE_EDITOR
bool TextureServer::Save(File* pf, Texture* pTex)
{
  if (!pTex)
  {
    return false;
  }

  // This doesn't save a bitmap file. It saves the text description of a texture,
  // with alpha-layer filename if it exists.
  // Save texture data in the same format as it is loaded in Get() below.
  pf->WriteComment("// Texture file name");
  pf->Write(pTex->GetFileName());

  pf->WriteComment("// Use alpha layer (1) or no alpha layer (0)");

  bool alpha = !pTex->GetAlphaFilename().empty();
  pf->WriteInteger(alpha ? 1 : 0);
  if (alpha)
  {
    pf->WriteComment("// Alpha layer filename");
    pf->Write(pTex->GetAlphaFilename());
  }
  return true;
}
#endif

PoolTexture* TextureServer::CreateNewTexture(
  const std::string& bitmapname,
  const std::string& alphafilename)
{
  // Try to find the texture filename in the map.
  TexturesMap::iterator it = m_texmap.find(bitmapname);

  PoolTexture* pTex = 0;

  // If we have already loaded the texture, we may be changing the 
  // alpha layer.
  // Using this function links a texture filename to an alpha filename - 
  // You can't use the texture filename with a different (or no) alpha
  // name.
  if (it != m_texmap.end())
  {
    pTex = (*it).second;
  }
  else
  {
    pTex = new PoolTexture;
    pTex->SetFileName(bitmapname);
    m_texmap[bitmapname] = pTex;
  }
  Assert(pTex);
  pTex->SetAlphaFilename(alphafilename);

  return pTex;
}

PoolTexture* TextureServer::Get(const string& texturefile, const string& alphafile)
{
  string lookupname = texturefile;
  lookupname += "@"; // to separate the two filenames. See ReloadAll().
  lookupname += alphafile;

  // Try to find the texture filename in the map.
  TexturesMap::iterator it = m_texmap.find(lookupname);

  // If found, simply return pointer to texture object.
  if (it != m_texmap.end())
    return (*it).second;

  // Texture not found. We create it, and add the pair to the map.
  PoolTexture* pt = new PoolTexture;

  // No loader - but we can still have a texture object, for scene editor.
  pt->SetAlphaFilename(alphafile);
  pt->SetFileName(texturefile);
    
  if (pt->ReadFile(const_cast<char*>(texturefile.c_str())) == 0)
  {
    // Failed to read file - j.c. 28/10/00
    //delete pt;
#ifdef _DEBUG
    std::cout << "Bad texture - TextureServer::Get returning 0\n";
#endif
    return 0;
  }

  pt->BGRtoRGB();

  // If an alpha-layer file has been specified, load the data and add to 
  // texture.

  if (!alphafile.empty())
  {
    // Add alpha layer. 
    int alpha = 255; // Totally opaque.
    pt->AddAlphaLayer(alpha);
    pt->SetAlphaFilename(alphafile); // so we can save later.
    if (!LoadAlphaData(pt, alphafile))
    {
#ifdef _DEBUG
    std::cout << "Bad alpha texture - TextureServer::Get returning 0\n";
#endif
      return 0;
    }
  }

  // Add the texture to the map.
  m_texmap[lookupname] = pt;

  return pt;
}

PoolTexture* TextureServer::Get(File* pf)
{
  // Get the texture filename for this polygon.
  std::string texturefile;
  if (!pf->GetDataLine(&texturefile))
  {
    pf->ReportError("Expected texture filename.");
    return 0;
  }

  // New file version - we allow a second bitmap, which specifies transparency
  // - i.e. alpha values - for the texture. 9/5/2000
  bool bAlphaLayer = false;
  std::string alphafile;
  if (pf->GetVer() >= 3) // starting with leaf files version 3.
  {
    int al = 0;
    if (!pf->GetInteger(&al))
    {
      pf->ReportError("Expected alpha layer flag.");
      return 0;
    }

    bAlphaLayer = (al != 0); // non-zero int means alpha bitmap follows.
    if (bAlphaLayer)
    {
       if (!pf->GetDataLine(&alphafile))
       {
         pf->ReportError("Expected alpha layer filename.");
         return 0;
       }
    }
  }

  return Get(texturefile, alphafile);
}


bool TextureServer::LoadAlphaData(PoolTexture* pt, 
                                    const std::string& alphafilename)
{
  // The alpha data is stored in the same format as texture data - i.e., a
  // 24-bit/pixel bitmap file. Yes, this is wasteful but simplifies the code
  // as we can reuse Texture directly.
  // The transparency data is conceptually an 8-bit/pixel bitmap. For each
  // pixel, 0 means totally transparent, and 255 means opaque.

  // We will be adding the alpha data to *pt. pt is a pointer to a texture
  // which already has an alpha layer set - we just change the data.

  PoolTexture alphatex; // temporarily use another Texture to load the 
                     // alpha bitmap.
  int ret = alphatex.ReadFile(const_cast<char*>(alphafilename.c_str()));

  if (ret == 0) 
  {
    return false;
  }

  if (alphatex.GetHeight() != pt->GetHeight() ||
      alphatex.GetWidth() != pt->GetWidth() )
  {
    string err = "Bad texture: alpha file is wrong size: ";
    err += alphafilename;
    ReportError(err);
    return false;
  }

  // Use Texture's PutAlpha() to treat the alpha bitmap as an alpha layer.
  // This makes sure the alpha data is the same size as the texture data.
  ret = pt->PutAlpha(&alphatex);

  return (ret != 0);
}
}
