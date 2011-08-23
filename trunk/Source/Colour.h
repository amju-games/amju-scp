/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Colour.h,v $
Revision 1.1.10.1  2007/03/06 20:56:47  jay
Add ColourList typedef to Pool

Revision 1.3  2006/09/16 11:12:00  Administrator
Added Load(), Save() and Interpolate()

Revision 1.2  2006/07/05 21:59:12  jay
Added ColourList typedef

Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#ifndef AMJU_COLOUR_H_INCLUDED
#define AMJU_COLOUR_H_INCLUDED

#include <vector>

namespace Amju
{
class File;

struct Colour
{
  Colour(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : 
    m_r(r), m_g(g), m_b(b), m_a(a) {}

  bool Load(File*);
#ifdef SCENE_EDITOR
  bool Save(File*);
#endif

  float m_r, m_g, m_b, m_a;
};

bool operator<(const Colour& c1, const Colour& c2);
bool operator==(const Colour& c1, const Colour& c2);
bool operator!=(const Colour& c1, const Colour& c2);

// Interpolate between 2 colours; 0 <= between <= 1.0
Colour Interpolate(const Colour& c1, const Colour& c2, float between);

typedef std::vector<Colour> ColourList;

}

#endif

