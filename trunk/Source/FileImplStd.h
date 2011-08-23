/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImplStd.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(FILE_IMPL_STD_H_INCLUDED)
#define FILE_IMPL_STD_H_INCLUDED

#include "FileImpl.h"
#include <fstream>

namespace Amju
{
// Implements File operations using standard file functions.
class FileImplStd : public FileImpl
{
public:
  virtual ~FileImplStd();
  virtual bool OpenRead(const std::string& path, const std::string& filename, bool isBinary);
  virtual bool GetLine(std::string* pResult);
  virtual unsigned int GetBinary(unsigned int bytes, unsigned char* pBuffer);
  virtual void BinarySeek(unsigned int pos);
  virtual unsigned int GetBinaryFileSize();

protected:
  // This is the real file to read from
  std::fstream m_file; 

};
}

#endif
