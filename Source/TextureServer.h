/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureServer.h,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if !defined(TEXTURE_SERVER_H_INCLUDED)
#define TEXTURE_SERVER_H_INCLUDED

#include "Texture.h" 
#include <map>
#include <string>

namespace Amju
{
typedef std::map<std::string, PoolTexture*> TexturesMap;

class File;

class TextureServer
{
public:
  static TextureServer* Instance();
  virtual ~TextureServer();

  // Get the required texture, by passing in the file we are currently reading.
  // The file should give us the image filename next. Only one
  // matching texture object is created; pointers to it are given to each
  // client that requests it.
  // File format v.3 and above supports alpha layer.
  PoolTexture* Get(File* pf);

  // Get the texture by giving filenames for texture filename and alpha 
  // filename.
  PoolTexture* Get(const std::string& texturename, const std::string& alphaname);

  // Create a new texture. By using this method, TextureServer keeps ownership
  // of all textures, avoiding leaks. This is designed to be used by SceneEditor.
  PoolTexture* CreateNewTexture(const std::string& bitmapname,
                             const std::string& alphafilename);

  // Load alpha layer file if required. 
  bool LoadAlphaData(PoolTexture* pt, const std::string& alphafilename);

  // Clear all textures. The scene should be reloaded after calling this.
  void Clear();

#ifdef SCENE_EDITOR
  // Save texture data. Used by SceneEditor.
  bool Save(File* pf, PoolTexture* pTex);
#endif

  // Discard all textures and reload.
  void ReloadAll();

  // To free up memory, delete a texture.
  // NB This is dangerous to use with no ref. counting!
  // Any existing pointers to the texture will be pointing to
  // garbage!
  // Only use if you are sure there's only one pointer to the texture in use.
  void DeleteTexture(PoolTexture*);

private:
  TextureServer();

protected:
  TexturesMap  m_texmap;
};
}
#endif
