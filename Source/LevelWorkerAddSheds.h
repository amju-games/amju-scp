/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerAddSheds.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/


#if !defined(LEVEL_WORKER_ADD_SHEDS_H_INCLUDED)
#define LEVEL_WORKER_ADD_SHEDS_H_INCLUDED

#include "LevelWorkerAddSolidsBase.h"
#include "SolidComponent.h"

namespace Amju
{
class LevelWorkerAddSheds : public LevelWorkerAddSolidsBase
{
public:
  static const char* TypeName;

  virtual bool Load(File* );
  virtual bool Save(File* );
  virtual const char* GetTypeName();
  virtual bool AddFeature(Level*, CubeMap* );

protected:
  // Block to put under sheds in the air.
  std::string m_blockName;
};
}


#endif


