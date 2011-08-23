/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Shadow.cpp,v $
Revision 1.1.10.3  2007/12/12 10:27:41  jay
Add per-shadow height offset

Revision 1.1.10.2  2007/12/10 22:45:55  jay
Change offset so player shadows don't z-fight too badly

Revision 1.1.10.1  2005/08/31 17:28:57  jay
Increase shadow offset to minimize z-fighting

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <string>
#include <utility>
#include "Shadow.h"
#include "Common.h"
#include "Geom2d.h"
#include "HeightServer.h"
#include "Vertex.h"
#include "Engine.h"
#include "SchAssert.h"

#include "SchAssert.h"
#include <vector>

namespace Amju
{
// Shadow offset above ground.
const float Shadow::OFFSET = 0.2f;

float Shadow::s_maxHeight;

Shadow::Shadow()
{
  m_oldx = 1000.0f; // so list is initially built
  m_oldy = 0;
  m_oldz = 0;
  m_oldsize = -1;
  m_yOffset = OFFSET;
}

Shadow::~Shadow()
{
}

void Shadow::SetYOffset(float yOffset)
{
  m_yOffset = yOffset;
}

bool Shadow::Init()
{
  s_maxHeight = Engine::Instance()->GetConfigFloat("shadow_max_height");
  if (s_maxHeight <= 0)
  {
    s_maxHeight = 1.0f;
  }
  return true;
}

void Shadow::Reset()
{
  m_oldx = 1000.0f; // so list is rebuilt
  m_oldy = 0;
  m_oldz = 0;
  m_oldsize = -1;
}

void Shadow::SetCaster(VisibleGameObject* pVgo)
{
}

void Shadow::Draw(float x, float y, float z,
                  float size,
                  const HeightServer& hs)
{
  if (x == m_oldx && y == m_oldy && z == m_oldz && size == m_oldsize)
  {
    // No need to recalculate shadow. Just draw the contents of the list.
    DrawList();
    return;
  }

  m_oldx = x;
  m_oldy = y;
  m_oldz = z;
  m_oldsize = size;

  // Recalculate the shadow, and fill the list. 
  RecalculateList(x, y, z, size, hs);
  DrawList();
}

}

