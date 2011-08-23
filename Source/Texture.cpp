/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Texture.cpp,v $
Revision 1.1.10.2  2006/05/04 09:44:05  jay
If AMJU_GL_NONE is #defined, no calls to OpenGL are made. This lets us
build command-line tools where there is no GL context.
In the future, this #define should be one option for the graphics lib.

Revision 1.1.10.1  2005/08/15 18:28:35  jay
Improve mipmap appearance hopefully

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

// As you can see, this was once a nice class written by someone
// called Pierre, which I have horribly butchered over the course
// of several years. Sorry Pierre.

//********************************************
// Texture.cpp
//********************************************
// class Texture
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 19/12/97
//********************************************

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include <string>
#include "Common.h"
#include "Texture.h"
#include "Bitmap.h" 
#include "File.h" // Get directory root
#include "StringUtils.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& msg);

Texture::Texture()
{
    m_pData = NULL;
    m_Width = 0;
    m_WidthByte32 = 0;
    m_Height = 0;
    m_Depth = 0;
    m_FileName = "";
    m_binding = 0;

//#ifndef AMJU_GL_NONE
//    glGenTextures(1, (GLuint*)&m_binding);
//#endif
    m_bindingset = false;
}

Texture::~Texture()
{
//#ifndef AMJU_GL_NONE
//    glDeleteTextures(1, (GLuint*)&m_binding);
//#endif
    Free();
}

void Texture::CreateBinding(Texture::BindType bt, bool wrap /*=true*/)
{
  if (!GetData())
  { 
    return;
  } 

  if (m_bindingset)
  {
    return; // We have already created binding for this texture.
  }

  m_bindingset = true;
//#ifndef AMJU_GL_NONE
//  glBindTexture(AmjuGL::AMJU_TEXTURE_2D, m_binding);
//#endif

  DrawNoBinding(bt, wrap);
  
  // New code: set up mipmaps
  // Build mipmaps for 24- and 32-bit textures
/*
  if (m_Depth == 24)
  {
#ifndef AMJU_GL_NONE
    gluBuild2DMipmaps(
      AmjuGL::AMJU_TEXTURE_2D, 
      GL_RGB, 
      GetWidth(), 
      GetHeight(), 
      GL_RGB, 
      GL_UNSIGNED_BYTE, 
      GetData());
#endif
  }
  else
  {
#ifndef AMJU_GL_NONE
    gluBuild2DMipmaps(
      AmjuGL::AMJU_TEXTURE_2D, 
      GL_RGBA, 
      GetWidth(), 
      GetHeight(), 
      GL_RGBA, 
      GL_UNSIGNED_BYTE, 
      GetData());
#endif
  }
  // End of new mipmap code
*/
}

void Texture::DrawNoBinding(Texture::BindType bt, bool wrap)
{
/*
  Assert(GetData());
  Assert(GetWidth() > 0);
  Assert(GetHeight() > 0);

  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);


  int wrapmode = GL_REPEAT;
  if (!wrap)
  {
    wrapmode = GL_CLAMP; 
  }

  glTexParameterf(AmjuGL::AMJU_TEXTURE_2D, AmjuGL::AMJU_TEXTURE_MATRIX_WRAP_S, wrapmode);
  glTexParameterf(AmjuGL::AMJU_TEXTURE_2D, AmjuGL::AMJU_TEXTURE_MATRIX_WRAP_T, wrapmode);
  // New code: changed to use mipmaps
  //  GL_LINEAR_MIPMAP_LINEAR is supposedly the smoothest-looking.
  glTexParameterf(AmjuGL::AMJU_TEXTURE_2D, AmjuGL::AMJU_TEXTURE_MATRIX_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
  glTexParameterf(AmjuGL::AMJU_TEXTURE_2D, AmjuGL::AMJU_TEXTURE_MATRIX_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Allowed values for min filter:
  // GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, 
  // GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR

  if (bt == AUTO_GENERATE)
  {
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
    glTexGeni(GL_S, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_OBJECT_LINEAR); 
    glTexGeni(GL_T, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_OBJECT_LINEAR);
  }
  else if (bt == ENV_MAP)
  {
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
    AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
    glTexGeni(GL_S, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_EYE_LINEAR); //GL_SPHERE_MAP);
    glTexGeni(GL_T, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_EYE_LINEAR); //GL_SPHERE_MAP);
  }
  else if (bt == REGULAR)
  {
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
    AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
  }

  glTexEnvf(AmjuGL::AMJU_TEXTURE_MATRIX_ENV, AmjuGL::AMJU_TEXTURE_MATRIX_ENV_MODE, GL_MODULATE);

  // Different depending on RGB or RGBA.
  if (m_Depth ==24)
  {
      glTexImage2D(AmjuGL::AMJU_TEXTURE_2D,
        0,
        3, 
        GetWidth(),
        GetHeight(),
        0,
        GL_RGB, 
        GL_UNSIGNED_BYTE,
        GetData());
  }
  else if (m_Depth == 32)
  {
      glTexImage2D(AmjuGL::AMJU_TEXTURE_2D,
        0,
        4, // four components, not 3 - i.e., RGBA.
        GetWidth(),
        GetHeight(),
        0,
        GL_RGBA, // not just RGB.
        GL_UNSIGNED_BYTE,
        GetData());
  }
  // Delete the data - the driver will keep a copy.
  //delete [] m_pData;
  //m_pData = 0;
*/

  if (m_Depth ==24)
  {
    AmjuGL::SetTexture(&m_binding, AmjuGL::AMJU_TEXTURE_REGULAR, AmjuGL::AMJU_RGB, 
    GetWidth(), GetHeight(), GetData());
  }
  else if (m_Depth == 32)
  {
    AmjuGL::SetTexture(&m_binding, AmjuGL::AMJU_TEXTURE_REGULAR, AmjuGL::AMJU_RGBA, 
    GetWidth(), GetHeight(), GetData());
  }
  else
  {
    Assert(0);
  }

}

void Texture::Bind()
{
//#ifndef AMJU_GL_NONE
//  glBindTexture(AmjuGL::AMJU_TEXTURE_2D, m_binding);
//#endif
  
  AmjuGL::UseTexture(m_binding);
}

int Texture::Alloc(unsigned int width,
                    unsigned int height,
                    unsigned int depth)
{
    Free();

    unsigned int BytePerPixel = (unsigned int)(depth / 8);
    unsigned int Width32 = WidthByte32(width,depth);

    // Only rgb and rgba modes
    Assert(BytePerPixel == 3 || 
             BytePerPixel == 4);

    m_pData = new unsigned char [Width32 * height];
    
    if (!m_pData)
    {
        return 0;
    }

    // Set members variables
    m_Width = width;
    m_WidthByte32 = Width32;
    m_Height = height;
    m_Depth = depth;

    return 1;
}

void Texture::Free()
{
    delete [] m_pData;
    m_pData = 0;

    m_Width = 0;
    m_Height = 0;
    m_Depth = 0;
}

int Texture::ReadFile(const char *filename,
                       int width,  // = -1
                       int height, // = -1
                       int depth)  // = -1
{
    Free();

    m_FileName = filename;

    std::string filestring = filename;

    if (filestring.length() < 5) // j.c. 1/11/00
    {
        return 0;
    }
    std::string extension = GetFileExt(filestring);

#ifdef TEXTURE_DEBUG
std::cout << "Texture::ReadFile: filename: " 
 << filename << ": ext: " << extension.c_str() << "\n";
#endif

    if (ToLower(extension) == "bmp")
    {
#ifdef TEXTURE_DEBUG
std::cout << "File is a BMP file\n";
#endif
        return ReadFileBMP(filestring.c_str());
    }
    // TODO Other file formats.
#ifdef TEXTURE_DEBUG
std::cout << "Unexpected!\n";
#endif
    // Unrecognized file format
    string error = "Bad Texture file extension: ";
    error += filestring; // + string(filename);
    error += ": extension is: ";
    error += extension;
    ReportError(error.c_str());
    return 0;
}

extern bool IsPowerOfTwo(int i);

int Texture::ReadFileBMP(const char *filename)
{
  unsigned char* bits = LoadDIBitmap(filename, &m_Width, &m_Height);

  if (bits)
  {
    m_pData = bits;
  }
  else
  {
    return 0;
  }

  m_Depth = 24;
  UpdateWidthByte32();

  if (!IsPowerOfTwo(m_Width) || !IsPowerOfTwo(m_Height))
  {
    std::string err = filename;
    err += ": Bitmap is not power of two size.";
    ReportError(err.c_str());
    return 0;
  }

  return 1;
}

void Texture::UpdateWidthByte32()
{
    m_WidthByte32 = WidthByte32(m_Width,m_Depth);
}

unsigned int Texture::WidthByte32(unsigned int width,
                                                                     unsigned int depth)
{
    // 32 bits alignment (4 bytes)
    int rest=(width*depth/8)%4;
    if(rest != 0)
        return (width*depth/8 + 4-rest);
    else
        return (width*depth/8);
}

int Texture::IsValid()
{
    int success = 0;
    success = (m_Depth == 24) || (m_Depth == 32);
    success &= (m_Width != 0);
    success &= (m_Height != 0);
    success &= (m_pData != NULL);

    return success;
}

int Texture::HigherPowerOfTwo(int value)
{
    Assert(value > 0);
    if(value <= 0)
        return value;

    int x = 1;

    while(1)
    {
        if(x >= value)
            return x;
        x *= 2;
    }

    return value;
}

int Texture::LowerPowerOfTwo(int value)
{
    Assert(value > 0);
    if(value <= 0)
        return value;

    int power = 1;
    int x = 0;

    while(1)
    {
        x = (int)pow(2.0f,power);
        if(x >= value)
            return (int)pow(2.0f,power-1);
        power++;
    }

    return value;
}

int Texture::SameSize(Texture *pTexture)
{
    int success = (m_Width == pTexture->GetWidth());
    success &= (m_Height == pTexture->GetHeight());
    return success;
}

int Texture::BGRtoRGB(void)
{
    if(!IsValid())
        return 0;

    unsigned char pixel;
    int BytePerPixel = m_Depth/8;
    for(unsigned int j=0;j<m_Height;j++)
        for(unsigned int i=0;i<m_Width;i++)
        {
            pixel = m_pData[m_WidthByte32*j+i*BytePerPixel+2];
            m_pData[m_WidthByte32*j+i*BytePerPixel+2] = m_pData[m_WidthByte32*j+i*BytePerPixel];
            m_pData[m_WidthByte32*j+i*BytePerPixel] = pixel;
        }
    return 1;
}

int Texture::SetAlphaLayer(unsigned char alpha) // 0 - 255
{
    // Check
    if(!IsValid())
        return 0;

    if(m_Depth != 32)
        return 0;

    // Fill alpha layer 
    int size = m_Width * m_Height;
    for(int i=0;i<4*size;i+=4)
        m_pData[i+3] = alpha;

    return 1;
}

int Texture::AddAlphaLayer(unsigned char alpha) // 0 - 255
{
    // Check
    if(!IsValid())
        return 0;

    // Has soon alpha
    if(HasAlpha())
        return SetAlphaLayer(alpha);

    // Alloc memory
    unsigned char *pData = new unsigned char[4*m_Width*m_Height];
    if(pData == NULL)
        {
        //AfxMessageBox("Texture::AddAlphaLayer : insuffisant memory");
        return 0;
        }

    // Fill new data
    int size = m_Width * m_Height;
    int BytePerPixel = m_Depth / 8;
    Assert(size > 0);
    for(int i=0;i<size;i++)
    {
        pData[4*i+0] = m_pData[BytePerPixel*i+0];
        pData[4*i+1] = m_pData[BytePerPixel*i+1];
        pData[4*i+2] = m_pData[BytePerPixel*i+2];
        pData[4*i+3] = alpha;
    }

    // Set new depth
    m_Depth = 32;

    // Replace datas
    delete [] m_pData;
    m_pData = pData;

    return 1;
}

int Texture::PutAlpha(Texture *pTexture) 
{
    // Check
    if(!IsValid())
        return 0;
    if(!pTexture->IsValid())
        return 0;

    if(!SameSize(pTexture))
        return 0;

    if(!AddAlphaLayer(0))
        return 0;

    // Fill new data
    unsigned char *pData = pTexture->GetData();
    int size = m_Width * m_Height;
    int BytePerPixel = pTexture->GetDepth() / 8;
    Assert(size > 0);
    for(int i=0;i<size;i++)
  {
    int v = ((int)pData[BytePerPixel*i+0]+
                (int)pData[BytePerPixel*i+1]+
                        (int)pData[BytePerPixel*i+2])/3;
    unsigned char c = (unsigned char)(v);
    m_pData[4*i+3] = c;
  }

    return 1;
}

int Texture::Grey(unsigned int x, unsigned int y)
{
    Assert(x<m_Width && x>=0);
    Assert(y<m_Height && y>=0);
    int BytePerPixel = m_Depth / 8;
    // Grey scale
    if(BytePerPixel == 1)
        return (int)m_pData[m_WidthByte32*y + x]; 
    else // 24 or 32 bits (alpha layer)
        return (int)((int)m_pData[m_WidthByte32*y + x*BytePerPixel+0]+ 
                     (int)m_pData[m_WidthByte32*y + x*BytePerPixel+1]+
                                 (int)m_pData[m_WidthByte32*y + x*BytePerPixel+2])/3;
}

void Texture::AndMask(Texture* pTex)
{
  Assert(m_Depth == pTex->m_Depth);

  int w = m_Width;
  int h = m_Height;
  int BytePerPixel = m_Depth/8;

  if (SameSize(pTex))
  {
    int t = w * h * BytePerPixel;
    for (int i = 0; i < t; i++)
    {
      m_pData[i] &= pTex->m_pData[i];
    }
    return;
  }

  // Not the same size, so must loop to edges.
  if (w > pTex->GetWidth())
  {
    w = pTex->GetWidth();
  }
  if (h > pTex->GetHeight())
  {
    h = pTex->GetHeight();
  }  
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      for (int k = 0; k < BytePerPixel; k++)
      {
        int dest = j * m_WidthByte32 + i * BytePerPixel + k;
        int src  = j * pTex->m_WidthByte32  + i * BytePerPixel + k;
        m_pData[dest] &= pTex->m_pData[src];
      }
    }
  }
}

void Texture::SetFromTexture(Texture* pTex, int x, int y, int w, int h)
{
  //Assert((x + w) <= pTex->GetWidth());
  if ((x + w) > pTex->GetWidth())
  {
    w = pTex->GetWidth() - x;
  }

  //y = pTex->GetHeight() - y; // TODO TEMP TEST

  //Assert((y + h) <= pTex->GetHeight());
  if ((y + h) > pTex->GetHeight())
  {
    h = pTex->GetHeight() - y;
  }

  int depth = pTex->m_Depth; 
  Assert(depth == 8 || depth == 24 || depth == 32);
  int bytepp = depth / 8; // bytes per pixel

  int width32 = WidthByte32(w, depth); // width * depth, aligned to 32-bit boundary
  unsigned char* pData = new unsigned char[h * width32];

  int hSrc = pTex->GetHeight();

  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      int p = i * bytepp + (h - 1 - j) * width32; // dest
      //int p = i * bytepp + j * width32; // dest
      int pSrc = (i + x) * bytepp + (hSrc - 1 - y - j) * pTex->m_WidthByte32;

      pData[p]       = pTex->m_pData[pSrc];

      if (depth == 24 || depth == 32)
      {
        pData[p + 1] = pTex->m_pData[pSrc + 1];
        pData[p + 2] = pTex->m_pData[pSrc + 2];
      }

      if (depth == 32)
      {
        pData[p + 3] = pTex->m_pData[pSrc + 3];
      }
    }
  }

  delete [] m_pData;
  m_pData = pData;
  m_Width = w;
  m_WidthByte32 = width32;
  m_Height = h;
  m_Depth = depth;
  m_FileName = "";
  m_bindingset = false;
}

void Texture::MakePowerOfTwoSize()
{
  int width = HigherPowerOfTwo(m_Width); 
  int height = HigherPowerOfTwo(m_Height);
  // If width and height are already powers of two, do nothing.
  if (width == m_Width && height == m_Height)
  {
    return;
  }
  // Make extra space appear at bottom of texture, not the top.
  // This is needed because bitmaps are upside down ?
  int y = height - m_Height; // initial source y-position

  int bytepp = m_Depth / 8; // bytes per pixel
  int width32 = WidthByte32(width, m_Depth);
  unsigned char* pData = new unsigned char[height * width32];
  // Zero the new data
  memset(pData, 0, height * width32);
  // Copy data to the new location
  for (int i = 0; i < m_Width; i++)
  {
    for (int j = 0; j < m_Height; j++)
    {
      int p = i * bytepp + (j + y) * width32; // destination
      int pSrc = i * bytepp + j * m_WidthByte32;

      pData[p]       = m_pData[pSrc];

      if (m_Depth == 24 || m_Depth == 32)
      {
        pData[p + 1] = m_pData[pSrc + 1];
        pData[p + 2] = m_pData[pSrc + 2];
      }

      if (m_Depth == 32)
      {
        pData[p + 3] = m_pData[pSrc + 3];
      }
    }
  }

  delete [] m_pData;
  m_pData = pData;
  m_Width = width;
  m_WidthByte32 = width32;
  m_Height = height;
  m_FileName = "";
  m_bindingset = false;
}
}

