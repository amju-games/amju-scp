/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureAnimation.h,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if !defined(TEXTURE_ANIMATION_H_INCLUDED)
#define TEXTURE_ANIMATION_H_INCLUDED

#include "TextureSequence.h"

namespace Amju
{
class TextureAnimation
{
public:
  TextureAnimation();

  void Draw();
  void Reset(); // hide the animation.
  void Start(); // start the animation
  void Set(int startElement, int endElement, bool loop, float frameTime);
  void SetSequence(TextureSequence* );

protected:
  SharedPtr<TextureSequence> m_pTextureSequence;

  float m_frameTime;
  float m_time;
  bool m_loop;
  int m_startElement;
  int m_endElement;
  int m_element;
};
}

#endif

