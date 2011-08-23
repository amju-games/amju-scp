/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GlueFileMem.cpp,v $
Revision 1.1.10.1  2007/07/04 18:30:47  jay
Override GetBinary to return a GlueFileBinaryData with a simple pointer,
 not with allocated/copied data.

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "GlueFileMem.h"
#include "FileSize.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
GlueFileMem::GlueFileMem()
{
  m_pMemFile = 0;
  m_filePos = 0;
}

GlueFileMem::~GlueFileMem()
{
  delete [] m_pMemFile;
}

bool GlueFileMem::OpenGlueFile(const std::string gluefilename, bool read)
{
  Assert(read);
  if (!read)
  {
#ifdef _DEBUG
    std::cout << "Glue file mem: read-only flag is " << (read ? "true" : "false") << std::endl;
#endif
    return false;
  }

  if (!GlueFile::SimpleOpen(gluefilename.c_str(), read))
  {
    return false;
  }

  // Get the size of the file, and allocate a big enough buffer.
  // Then read the whole file into the buffer.
  m_fileSize = FileSize(gluefilename.c_str());

  Assert(m_fileSize > 0);

  m_pMemFile = new unsigned char[m_fileSize];
  if (!m_pMemFile)
  {
#ifdef _DEBUG
    std::cout << "Failed to allocate memory: " << m_fileSize << " bytes" << std::endl;
#endif
    return false;
  }

  m_gluefile.seekg(0);
  m_gluefile.read((char*)m_pMemFile, m_fileSize);

  bool reachedEnd = ((unsigned long)m_gluefile.tellg() == m_fileSize);
  if (reachedEnd)
  {
#ifdef _DEBUG
    cout << "Read all of glue file." << endl;
#endif
  }
  else
  {
#ifdef _DEBUG
    cout << "Read " << (unsigned long)m_gluefile.tellg() << " bytes!" << endl;
#endif
  }
  
  return ReadTable();
}

void GlueFileMem::SetPos(uint32 pos)
{
  m_filePos = pos;
}

uint32 GlueFileMem::GetPos()
{
  return m_filePos;
}

uint32 GlueFileMem::GetBinary(uint32 numbytes, unsigned char* pBuffer)
{
  Assert(m_filePos + numbytes <= m_fileSize);

  // dest, src, count
  memcpy(pBuffer, m_pMemFile + m_filePos, numbytes);
  m_filePos += numbytes;
  return numbytes;
}

uint32 GlueFileMem::GetBinary(uint32 seekPos, uint32 numbytes, unsigned char* pBuffer)
{
  // ** NB Unlike the base class implementation, this _is_ MT safe for read-only
  // files.

  // dest, src, count
  memcpy(pBuffer, m_pMemFile + seekPos, numbytes);
  return numbytes;
}

GlueFileBinaryData GlueFileMem::GetBinary(uint32 seekPos, uint32 numbytes)
{
  // In this implementation, we do not need to allocate a buffer and copy
  // data into it.
  // We can just set the ptr in the BinaryData object to point to the 
  // data in the mem file.
  GlueFileBinaryData data;
  data.SetBuffer(m_pMemFile + seekPos);
  return data;
}

}
