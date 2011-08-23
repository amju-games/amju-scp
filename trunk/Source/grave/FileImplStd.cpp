/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileImplStd.cpp,v $
Revision 1.1.10.3  2006/08/30 21:12:52  Administrator
Get Pool to build in MSVC

Revision 1.1.10.2  2006/05/15 08:18:41  jay
Chmod files when opened for reading or writing so they can be written
to by everyone.

Revision 1.1.10.1  2006/01/20 13:45:13  jay
Gcc 4.0 fix: no need for conditional compile

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#ifdef MACOSX
#include <sys/types.h>  // chmod
#include <sys/stat.h>   // chmod
#endif
#include "FileImplStd.h"

using namespace std;
namespace Amju
{
  FileImplStd::~FileImplStd()
  {
  }

  bool FileImplStd::OpenRead(const std::string& path, const std::string& filename, bool isBinary)
  {
    int mode = ios::in;
    if (isBinary)
    {
      mode |= ios::binary;
    }

#ifdef MSVC
    m_file.open((path + filename).c_str(), mode);
#else
    m_file.open((path + filename).c_str(), (std::_Ios_Openmode)mode);
#endif

    bool result = m_file.is_open();

#ifdef MACOSX
    if (result)
    {
      // We may be writing to a shared area so multiple users can access the
      // file.
      int CHMOD_MODE =
        S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH;
        // Read + write access for all
      chmod((path + filename).c_str(), CHMOD_MODE); 
    }
#endif

    return result;
  }

  bool FileImplStd::GetLine(std::string* pResult)
  {
    // Really read from a file.
    if (m_file.eof())
    {
      return false;
    }
    
    // TODO beware! max line length is 1000!
    char buf[1000];
    int buflen = 1000;
    
    m_file.getline(buf, buflen);
    *pResult = buf;
    
    // Chop off the last character if it happens to be char(13). This is ok for windows
    // but not unix.
    if (pResult->size() > 0 && (*pResult)[pResult->size()-1] == char(13) )
    {
      *pResult = pResult->substr(0, pResult->size() - 1);
    }
    
    return true;
  }

  unsigned int FileImplStd::GetBinary(unsigned int bytes, unsigned char* pBuffer)
  {
    if (m_file.eof())
    {
       return 0;
    }

    m_file.read((char*)pBuffer, bytes);
    return bytes;
  }

  void FileImplStd::BinarySeek(unsigned int pos)
  {
    m_file.seekg(pos);
  }

  unsigned int FileImplStd::GetBinaryFileSize()
  {
    return 0; // TODO never used
  }
}
