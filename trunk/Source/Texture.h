/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Texture.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

// As you can see, this was once a nice class written by someone
// called Pierre, which I have horribly butchered over the course
// of several years. Sorry Pierre.

//********************************************
// Texture.h
//********************************************
// class Texture
//********************************************
// This object stores a bitmap image used as
// an OpenGL texture.
// Files are stored as .bmp format. 
// Depth are currently 24 or 32 bits,
// Modes are : RGB, RGBA (alpha layer).
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 18/02/98
//********************************************
// Modified 23/4/00 J Colman. Ctor is now private, but TextureServer
//  has been made a friend. All requests for a Texture must go through the
//  texture server, which ensures that there is only one copy of any texture.

#ifndef _TEXTURE_
#define _TEXTURE_

#include "Common.h"
#include <string>

namespace Amju
{
class TextureLoader;

class PoolTexture 
{
  // Textures are constructed by the Texture Server only.
  friend class TextureServer;  // j.c. 23/4/00

private :
  PoolTexture();
  ~PoolTexture();

public: 

  enum BindType 
  {
    REGULAR,
    AUTO_GENERATE,
    ENV_MAP
  };

  int ReadFile(const char *filename, int width=-1, int height=-1, int depth=-1);
    
  unsigned char *GetData() const { return m_pData; }
  unsigned int GetWidth() const { return m_Width; }
  unsigned int GetHeight() const { return m_Height;}
  unsigned int GetDepth() const { return m_Depth; }

  std::string GetFileName() const { return m_FileName; }

  void SetFileName(const std::string& name) { m_FileName = name; }

  // j.c. 8/10/2000 - Remember the filename of any alpha layer.
  // This is required for Saving from SceneEditor.
  std::string GetAlphaFilename() const { return m_alphaFilename; }
  void SetAlphaFilename(const std::string& s) { m_alphaFilename = s; } 

  // glBindTexture functions
  // Called once to set up binding for this texture.
  void CreateBinding(BindType bt, bool wrap = true); 
  // Called to bind texture operations to this texture.
  void Bind(); 

  // Use the texture without binding. This is called by CreateBinding once
  // glBindTexture() has been called.
  void DrawNoBinding(BindType bt, bool wrap = true); 

  // Misc
  int IsValid();
  int SameSize(PoolTexture *pTexture);
  int BGRtoRGB(void);
  int HigherPowerOfTwo(int value);
  int LowerPowerOfTwo(int value);

  // Alpha
  bool HasAlpha() { return (m_Depth == 32); }
  int AddAlphaLayer(unsigned char alpha);
  int SetAlphaLayer(unsigned char alpha);
  int PutAlpha(PoolTexture *pTexture);   // Put an alpha layer from grey scale

  // Buffer
  int ReadBuffer(unsigned char *buffer, int width, int height, int depth);
  int Grey(unsigned int x,unsigned int y);

  // Bitmask operations
  // Perform bitwise-and with argument texture, modifying the contents
  // of this texture.
  void AndMask(PoolTexture*);  

  // Set this texture to a rectangle of pTex. 
  // The new size of this texture will be (w, h).
  // The top left corner of this texture is set to texel(x, y) of pTex.
  void SetFromTexture(PoolTexture* pTex, int x, int y, int w, int h);

  // Force the width and height of the texture to powers of two.
  void MakePowerOfTwoSize();

private:
    // Get the data from a .bmp file.
    int ReadFileBMP(const char *filename);

    void UpdateWidthByte32();
    unsigned int WidthByte32(unsigned int width,unsigned int depth);

    int Alloc(unsigned int width,unsigned int height,unsigned int depth);
    void Free(void);

    unsigned char *m_pData;    // datas
    unsigned int   m_Width;    // width (pixels)
    unsigned int   m_Height;   // height (pixels)
    unsigned int   m_Depth;    // bits per pixel 
    std::string m_FileName; // texture image file name
    std::string m_alphaFilename; // alpha layer file - j.c. 8/10/00

    unsigned int     m_WidthByte32; // width (in bytes, and 32 bits aligned)

    unsigned int m_binding; // For glBindTexture().
    bool m_bindingset; // set to true once CreateBinding() has been called.

    // Not assignable or copyable.
    PoolTexture(const PoolTexture&);
    PoolTexture& operator=(const PoolTexture&);
};
}
#endif // _TEXTURE_

