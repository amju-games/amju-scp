/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureModFire.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "TextureModFire.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
TextureModFire::TextureModFire(Texture* pTexture) : TextureModifier(pTexture)
{
  for (int i = 0; i < FIRE_ARRAY_SIZE; i++)
  {
    src[i] = 0;
  }  

}

void TextureModFire::Update()
{
  int i; 

  static float time = 0;
  float dt = Engine::Instance()->GetDeltaTime();
  time +=dt;
  if (time < 0.1f)
  {
    return;
  }
  time = 0;

  const int BORDER = 8;
  for (int j = 0; j < FIRE_SIZE; j++)
  {
    if (j < BORDER || j >= FIRE_SIZE - BORDER)
    {
      src[j] = 0;
      src[j + FIRE_SIZE] = 0;
    }
    else
    {
      uint8 r = (uint8)((rand()%128) + 128);  
      uint8 r1 = (uint8)((rand()%128) + 128);  
      src[j] = r;
      src[j + FIRE_SIZE] = r1;
    }
  }

  for (i = 1+2*FIRE_SIZE; i < (FIRE_ARRAY_SIZE - 1); i++)
  {
    int a = src[i-1] + src[i+1] + src[i-1-FIRE_SIZE] + src[i-FIRE_SIZE] + src[i+1-FIRE_SIZE] +
         src[i-1-2*FIRE_SIZE] + src[i-2*FIRE_SIZE] + src[i+1-2*FIRE_SIZE];
    //a = a >> 3;
    a /= 8;
    //std::cout << a << " "; 
    a &= 0xf0;
    //if (a < 16) a = 0;
      
    dst[i] = (uint8)a;
  }

  //std::cout << "\n\n";

  // Copy uint8 data to uint32 data.
  uint32* data = (uint32*)m_pTexture->GetData();
  for (i = 0; i < FIRE_ARRAY_SIZE; i++)
  {
    //cout << "src " << (int)src[i] << " <- dst " << (int)dst[i] << "\n";
    src[i] = dst[i];

#ifdef WIN32
    uint32 alpha = data[i] & 0xff000000; // ENDIAN
    uint32 res = s_pColourTable[dst[i]] & 0x00ffffff | alpha; // ENDIAN
#endif

#ifdef MACOSX
    uint32 alpha = 0x000000c0; // data[i] & 0x000000ff; // ENDIAN
    uint32 res = s_pColourTable[dst[i]];// & 0xffffff00 | alpha; // ENDIAN
#endif

    data[i] = res;

  }
  m_pTexture->Bind();

/*
  // TODO AmjuGL equivalent ?
  glTexSubImage2D(AmjuGL::AMJU_TEXTURE_2D, 
                  0, 
                  0, 
                  0, 
                  FIRE_SIZE, 
                  FIRE_SIZE, 
                  GL_RGBA, 
                  GL_UNSIGNED_BYTE, 
                  m_pTexture->GetData());
*/
}
}

