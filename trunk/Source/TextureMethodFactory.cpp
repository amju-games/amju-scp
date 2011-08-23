/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureMethodFactory.cpp,v $
Revision 1.1.10.2  2005/06/25 13:29:57  Administrator
More info in error msg

Revision 1.1.10.1  2005/03/18 22:03:05  jay
Added NullWithCoords texture method

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#include "TextureMethodFactory.h"
#include "TextureMethod.h"
#include "File.h"
#include "StringUtils.h"

namespace Amju
{
TextureMethod* TextureMethodFactory::Create(File* pf)
{
    int typeint = 0;
    if (!pf->GetInteger(&typeint))
    {
      pf->ReportError("Expected leaf texture method flag.");
      return 0;
    }
    if (typeint == 1)
    {
      return new TexMethodRegular;
    }
    else if (typeint == 2)
    {
      return new TexMethodAuto;
    }
    else if (typeint == 3)
    {
      return new TexMethodEnv;
    }
    else if (typeint == 4)
    {
      // New Null texture method - allows us to reuse geometry data. 
      return new TexMethodNull;
    }
    else if (typeint == 5)
    {
      return new TexMethodRegularClamp;
    }
    else if (typeint == 6)
    {
      return new TexMethodNullWithCoords;
    }

    std::string s = "Bad leaf texture method flag: ";
    s += ToString(typeint); 
    pf->ReportError(s);
    return 0;
}
}

