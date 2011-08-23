/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Lensflare.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LENSFLARE_H_INCLUDED)
#define LENSFLARE_H_INCLUDED

#include "SharedPtr.h"
#include "BillBoard.h"

namespace Amju
{
class VertexBase;

// A Facade around Mark Kilgard's OpenGL lens flare demo
// http://reality.sgi.com/opengl/tips/lensflare
class Lensflare
{
public:
  Lensflare();
  ~Lensflare();

  void Draw(const VertexBase& sunpos,
            const VertexBase& camerapos,
			const VertexBase& lookatpos);

  bool Init();

protected:
  // TODO vector
  SharedPtr<BillBoard> m_billboards[20];

};
}

#endif

