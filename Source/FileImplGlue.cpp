/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImplGlue.cpp,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "FileImplGlue.h"
#include "SchAssert.h"

namespace Amju
{
SharedPtr<GlueFile> FileImplGlue::s_pGlueFile;
bool FileImplGlue::s_isReady = false;

FileImplGlue::FileImplGlue()
{
  Assert(s_pGlueFile.GetPtr());
  // Store the old file position
  m_oldPos = s_pGlueFile->GetPos();
}

FileImplGlue::~FileImplGlue()
{
  Assert(s_pGlueFile.GetPtr());
  // Restore the stored file pos
  s_pGlueFile->SetPos(m_oldPos);
}

GlueFile* FileImplGlue::GetGlueFile()
{
  return s_pGlueFile.GetPtr();
}

bool FileImplGlue::OpenRead(const std::string& path, const std::string& filename, bool isBinary)
{
  // Ignore the path.
  // Get the start pos of the sub-file we want and seek to there.
  uint32 base = 0;
  if (!s_pGlueFile->GetSeekBase(filename, &base))
  {
    return false;
  }
  s_pGlueFile->SetPos(base);
  m_subfilename = filename;
  return true;
}

bool FileImplGlue::GetLine(std::string* pResult)
{
  // Only called if the current sub-file is text.
  // Get characters until we hit end-of-line char.
  // Strip off any MS-DOS ^M char preceding it.
  // TODO Optimize this!
  *pResult = std::string();
  unsigned char c;
  while (GetBinary(1, &c) == 1)
  {
    if (c == 10)
    {
      return true;
    }
    if (c != 13)
    {
      (*pResult) += c;
    }
  }
  return true;
}

unsigned int FileImplGlue::GetBinary(unsigned int bytes, unsigned char* pBuffer)
{
  return s_pGlueFile->GetBinary(bytes, pBuffer);
}

void FileImplGlue::BinarySeek(unsigned int pos)
{
  uint32 base = 0;
  if (!s_pGlueFile->GetSeekBase(m_subfilename, &base))
  {
    return;
  }
  s_pGlueFile->SetPos(base + pos);
}

unsigned int FileImplGlue::GetBinaryFileSize()
{
  return s_pGlueFile->GetSize(m_subfilename);
}

bool FileImplGlue::OpenGlueFile(const std::string& gluefilename, GlueFile* pGf)
{
  // Set up the GlueFile which every implementation will access.
  if (pGf)
  {
    s_pGlueFile = pGf;
  }
  else
  {
    s_pGlueFile = new GlueFile;
  }
  bool b = s_pGlueFile->OpenGlueFile(gluefilename, true); // true means read-only
  s_isReady = b;
  return b;
}
}
