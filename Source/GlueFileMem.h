/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GlueFileMem.h,v $
Revision 1.1.10.1  2007/07/04 18:30:47  jay
Override GetBinary to return a GlueFileBinaryData with a simple pointer,
 not with allocated/copied data.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(GLUE_FILE_MEM_H_INCLUDED)
#define GLUE_FILE_MEM_H_INCLUDED

#include "GlueFile.h"

namespace Amju
{
// A glue file which loads the whole file into memory when it is opened
// for reading.
// This is to allow concurrent access to the file from multiple threads.
class GlueFileMem : public GlueFile
{
public:
  GlueFileMem();
  virtual ~GlueFileMem();
  virtual bool OpenGlueFile(const std::string gluefilename, bool read);
  virtual uint32 GetPos();
  virtual void SetPos(uint32 pos);

  // Copy data from mem, rather than read from a file.
  virtual uint32 GetBinary(uint32 numbytes, unsigned char* pBuffer);
  virtual uint32 GetBinary(uint32 seekPos, uint32 numbytes, unsigned char* pBuffer);

  // Get binary: this version is most efficient as the object returned
  // can just point to the glue file memory.
  virtual GlueFileBinaryData GetBinary(uint32 seekPos, uint32 numbytes);

protected:
  unsigned char* m_pMemFile;
  unsigned int m_fileSize; 
  unsigned int m_filePos;
};
}

#endif

