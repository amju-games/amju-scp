/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImpl.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(FILE_IMPL_H_INCLUDED)
#define FILE_IMPL_H_INCLUDED

#include <string>
#include "SharedPtr.h"

namespace Amju
{
// Interface for classes implementing File operations.
class FileImpl : public Shareable
{
public:
  virtual ~FileImpl();
  virtual bool OpenRead(const std::string& path, const std::string& filename, bool isBinary) = 0;
  virtual bool GetLine(std::string* pResult) = 0;
  virtual unsigned int GetBinary(unsigned int bytes, unsigned char* pBuffer) = 0;
  virtual void BinarySeek(unsigned int pos) = 0;
  virtual unsigned int GetBinaryFileSize() = 0;
};
}


#endif
