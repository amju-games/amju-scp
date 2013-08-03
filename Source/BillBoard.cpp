#include "BillBoard.h"
#include "CompositeFactory.h"
#include "SolidComponent.h"
#include "File.h"
#include "Camera.h"
#include "TextureServer.h"
#include <math.h>
#include <Matrix.h>
#include <AmjuAssert.h>

using namespace std;

namespace Amju
{
BillBoard::BillBoard()
{
  m_pTexture = 0;
  m_size = 0;
  m_vertical = false;
}

void BillBoard::Draw()
{
  Assert(m_pTexture);

  m_pTexture->Bind();

  // This Billboarding algorithm is by Nate Miller. 
  // See www.nate.scuzzy.net. 
  // Get the current transformation matrix. Use the transpose to get
  // 'right' and 'up' vectors. Make a quad using these vectors.
  // This face will always directly face the camera.
  Matrix m;
  m.ModelView();

//  Matrix pr;
//  pr.Projection();
//  m = m * pr;

  Vec3f right(m[0], m[4], m[8]); //m.at(0), m.at(4), m.at(8));
  right.Normalise();
  Vec3f up;

  if (m_vertical)
  {
    up = Vec3f(0, 1, 0);
  }
  else
  {
    up = Vec3f(m[1], m[5], m[9]); //m.at(1), m.at(5), m.at(9));
    up.Normalise();
  }

  Vec3f v0 = ( up + right); v0 *= m_size;
  Vec3f v1 = ( up - right); v1 *= m_size;
  Vec3f v2 = (Vec3f()-up - right); v2 *= m_size;
  Vec3f v3 = (Vec3f()-up + right); v3 *= m_size;

  AmjuGL::Vert verts[4] =
  {
    AmjuGL::Vert(v0.x, v0.y, v0.z,   1, 0,   0, 1, 0), // x, y, z, u, v, nx, ny, nz
    AmjuGL::Vert(v1.x, v1.y, v1.z,   1, 1,   0, 1, 0),
    AmjuGL::Vert(v2.x, v2.y, v2.z,   0, 1,   0, 1, 0),
    AmjuGL::Vert(v3.x, v3.y, v3.z,   0, 0,   0, 1, 0)
  };

  AmjuGL::Tris tris;
  tris.resize(2); 
  AmjuGL::Tri* tri =  &tris[0];
  tri->m_verts[0] = verts[0];
  tri->m_verts[1] = verts[1];
  tri->m_verts[2] = verts[2];

  tri = &tris[1];
  tri->m_verts[0] = verts[0];
  tri->m_verts[1] = verts[2];
  tri->m_verts[2] = verts[3];

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ); // TODO have a member flag
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::PushMatrix();

  if (!m_triList)
  {
    m_triList = (TriListDynamic*)AmjuGL::Create(TriListDynamic::DRAWABLE_TYPE_ID);
  }
  m_triList->Set(tris);
  AmjuGL::Draw(m_triList);

  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}

#if defined(SCENE_EDITOR)
bool BillBoard::Save(bool)
{
  if (!IsDirty())
  {
    return true;
  }

  File jf;  
  if (!jf.OpenWrite(GetName()))
  {
    return false;
  }
  if (!m_pTexture)
  {
    return false;
  }
  if (!TextureServer::Instance()->Save(&jf, m_pTexture))
  {
    return false;
  }
  if (!jf.WriteComment("// Size"))
  {
    return false;
  }
  jf.WriteFloat(m_size);
  jf.WriteComment("// Vertical (y=1,2=n)");
  jf.WriteInteger(m_vertical ? 1 : 2);
  return true;
}
#endif

bool BillBoard::Load(const std::string& filename)
{
#if defined(SCENE_EDITOR)
  m_name = filename;
#endif

  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open billboard file.");
    return false;
  }
  return Load(&f);
}

bool BillBoard::Load(File* pf)
{
  m_pTexture = TextureServer::Instance()->Get(pf);
  if (!m_pTexture)
  {
    pf->ReportError("Expected billboard texture.");
    return false;
  }
  m_pTexture->CreateBinding(PoolTexture::REGULAR);

  if (!pf->GetFloat(&m_size))
  {
    pf->ReportError("Expected billboard size.");
    return false;
  }

  // Vertical flag: if 1, billboard should always be vertical, e.g. fire.
  // If 2, the billboard always directly faces the camera.
  int vertical;
  if (!pf->GetInteger(&vertical))
  {
    pf->ReportError("Expected billboard vertical flag.");
    return false;
  }
  if (vertical == 1)
  {
    m_vertical = true;
  }
  else if (vertical == 2)
  {
    m_vertical = false;
  }
  else 
  {
    pf->ReportError("Bad billboard vertical flag.");
    return false;
  }  
  return true;
}

void BillBoard::CreateBoundingSphere(const Matrix& cm)
{
  // This fixes bounding sphere for Billboards being way off.
  // The orientation is used in the calling function to transform
  // cm, so we don't use it again.
  Vec3f v = Vec3f(0, 0, 0) * cm; //m_or.GetVertex() * cm;
  m_bsphere.SetCentre(v);
  m_bsphere.SetRadius(m_size); 
}

}




