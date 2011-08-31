/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CompositeFactory.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "StringUtils.h"
#include "CompositeFactory.h"
#include "SolidComponent.h"
#include "SolidLeaf.h"
#include "SolidComposite.h"
#include "MovingSolid.h"
//#include "ParticleSystem.h"
#include "TextLeaf.h"
#include "AttribDecorator.h"
#include "BillBoard.h"
//#include "Fire.h"
#include "Text3dLeaf.h"
//#include "VisibilityPoint.h"
//#include "PulsatingBillboard.h"
#include "SolidLeafCharacterMesh.h"

using namespace std;

namespace Amju
{
CompositeFactory* CompositeFactory::Instance()
{
  static CompositeFactory cf;
  return &cf;
}

CompositeFactory::CompositeFactory() {}

PSolidComponent CompositeFactory::Create(const string& filename)
{
  std::string ext = GetFileExt(filename);

  PSolidComponent pc;

  if (ext == "leaf")
  {
    pc = new SolidLeaf;
  }
  else if (ext == "comp")
  {
    pc = new SolidComposite;
  }
  else if (ext == "move")
  {
    pc = new MovingSolid;
  }
  else if (ext == "text")
  {
    pc = new TextLeaf;
  }
  else if (ext == "attr")
  {
    pc = new AttribDecorator;
  }
  else if (ext == "bill")
  {
    pc = new BillBoard;
  }
  else if (ext == "char") // TODO or other formats
  {
    pc = new SolidLeafCharacterMesh;
  }

  return pc;
}
}

