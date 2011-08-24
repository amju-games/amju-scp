/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Lensflare.cpp,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Lensflare.h"
#include "Vertex.h"
#include "Common.h"
#include "Engine.h"
#include "File.h"
#include "SolidComponent.h"
#include "BillBoard.h"
#include "Geometry.h"
#include <string>

using namespace std;

namespace Amju
{
Lensflare::Lensflare()
{
}

Lensflare::~Lensflare()
{
}

void Lensflare::Draw(const VertexBase& light, //sunpos,
        const VertexBase& from, //camerapos,
		const VertexBase& at) //lookatpos)
{
  // Near clip same as in Engine::InitGl()
  /* view_dir = normalize(at-from) */
  VertexBase viewdir(at);
  viewdir -= from;
  viewdir.Normalize();

  /* center = from + near_clip * view_dir */
  VertexBase centre(viewdir);
  centre *= 3.0f;
  centre += from;

  /* axis = light - center */
  VertexBase axis(light);
  axis -= centre;


  // position = center + flare[i].loc * axis 


  axis.Normalize();
  VertexBase v;

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
//  AmjuGL::Disable(GL_FOG);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ); // Don't need to depth test

  //AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  for (int i = 10; i < 13; i++) // was 11..13
  {
    AmjuGL::PushMatrix();
    v = axis;
    v *= 3.0f * (i - 10);
    v += centre;
    AmjuGL::Translate(v.x, v.y, v.z);
    //glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    m_billboards[0]->SetSize(1.0f);
    m_billboards[0]->Draw();
    AmjuGL::PopMatrix();
  }

  // Turn the things we disabled back on.
  AmjuGL::PopAttrib();
}

bool Lensflare::Init()
{
  // TODO CONFIG!
  PSolidComponent pBb = SolidComponent::LoadSolid("lensfl.bill");
  m_billboards[0] = (BillBoard*)(pBb.GetPtr());
  m_billboards[0]->SetSize(1.0f);

  return true;
}
}


