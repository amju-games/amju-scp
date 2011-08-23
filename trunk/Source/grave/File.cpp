/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: File.cpp,v $
Revision 1.1.4.1.4.4  2007/03/16 08:51:07  jay
Turn off debug output

Revision 1.1.4.1.4.3  2006/05/15 08:18:41  jay
Chmod files when opened for reading or writing so they can be written
to by everyone.

Revision 1.1.4.1.4.2  2005/08/15 16:01:44  jay
Added TokenGetLineRemainder(). Needed to parse MD3 config files but could
be generally useful.

Revision 1.1.4.1.4.1  2005/05/09 05:53:07  jay
Fixes for new gcc version

Revision 1.1.4.1  2004/10/01 09:04:08  Administrator
Don't write filename on open, it's no good if appending

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#ifdef MACOSX
#include <sys/types.h>  // chmod
#include <sys/stat.h>   // chmod
#endif
#include "File.h"
#include "FileImplStd.h"
#include "FileImplGlue.h"
#include "Endian.h"
#include "StringUtils.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& error);

const int File::CURRENT_VERSION = 11;
const std::string File::SEPARATOR = ""; // separate lines in file.
string File::s_root;
const bool File::NO_VERSION = false;
const bool File::BINARY = true;
  
File::File(bool hasVersionInfo /* = true */, bool useGlueImpl /* = true */) : 
  m_hasVersionInfo(hasVersionInfo)
{
  m_bEOFReached = false; 
  m_version = -1; // meaning no version info found yet.
  m_currentLine = 0;
  m_pTokeniser = &m_defaultTokeniser;
  m_isBinary = false;

  if (useGlueImpl && FileImplGlue::IsReady())
  {
    m_pImpl = new FileImplGlue;
  }
  else
  {
    m_pImpl = new FileImplStd;
  }
}

File::~File()
{
}

void File::SetRoot(const std::string& root, const std::string& sep)
{
  s_root = root;
  if (root.empty())
  {
    return;
  }
  // Get last character. If it isn't a slash/backslash, append one.
  char lastch = s_root[s_root.size() - 1];
  string last(lastch, 1); // construct string from 1 char
  if (last == sep)
  {
    return;
  }
  s_root += sep;
}

bool File::ReadVersion()
{
  // If this file has been flagged as containing version info,
  // get the first integer, and store as the version.
  m_version = 0;
  if (m_hasVersionInfo)
  {
    // Get the version number.
    if (!GetInteger(&m_version))
    {
      return false;
    }
  }
  return true;
}

bool File::OpenRead(const std::string& filename, bool isBinary, bool useRoot) 
{
  m_filename = filename;
  m_isBinary = isBinary;

  std::string root;
  if (useRoot) 
  {
    root = s_root;
  }

  if (!m_pImpl->OpenRead(root, filename, isBinary))
  {
    m_bEOFReached = true; 
    return false;
  }

  return ReadVersion();
}

unsigned int File::GetBinary(unsigned int bytes, unsigned char* pBuffer)
{
  if (m_bEOFReached)
  {
    return 0;
  }

  return m_pImpl->GetBinary(bytes, pBuffer);
}

void File::BinarySeek(unsigned int seekPos)
{
  m_pImpl->BinarySeek(seekPos);
}

void File::ReportError(const string& error)
{
  // Add filename and line number to error message.
  char buf[10];
  sprintf(buf, "%d", GetLine() );
  string newError = GetName() + ", Line " + string(buf) + ": ";
  Amju::ReportError(newError);
  Amju::ReportError(error);
}

bool File::OpenWrite(
  const std::string& filename, 
  int version, 
  bool isBinary,
  bool useRoot,
  bool truncate)
{
  m_filename = filename;
  m_isBinary = isBinary;
  
  int mode = ios::out;
  if (truncate)
  {
    mode |= ios::trunc;
  }
  else
  {
    mode |= ios::app;
  }

  if (isBinary)
  {
    mode |= ios::binary;
  }

  std::string root;
  if (useRoot) 
  {
    root = s_root;
  }

#ifdef FILE_DEBUG
  std::cout << "opening " << std::string(root + m_filename).c_str() << " for writing.\n";
#endif

#if defined(MINGW) || defined (MACOSX)
  m_os.open((root + m_filename).c_str(), (std::_Ios_Openmode)mode);
#else
  m_os.open((root + m_filename).c_str(), mode);
#endif
  if (!m_os.is_open())
  {
    return false;
  }
 
#ifdef MACOSX
  // We may be writing to a shared area so multiple users can access the
  // file.
  int CHMOD_MODE = 
    S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH;
    // Read + write access for all
  chmod((root + m_filename).c_str(), CHMOD_MODE);
#endif
 
  if (m_hasVersionInfo)
  {
    WriteComment("// File version");
    // Write version number
    WriteInteger(version);
    WriteComment(SEPARATOR);
  }
  return true;
}

bool File::Write(const std::string& text)
{
  if (!m_os.is_open())
  {
    return false;
  }
  if (m_isBinary)
  { 
    m_os.write(text.c_str(), text.length());
    m_os.write("\0", 1);
  }
  else
  {
    m_os << text.c_str() << endl;
    m_currentLine++;
  }
  return true;
}

bool File::WriteBinary(const char* c, unsigned int length)
{
  Assert(m_isBinary);
  m_os.write(c, length);
  return true;
}

bool File::WriteComment(const std::string& comment)
{
  if (m_isBinary)
  {
    return true;
  }
  return Write(comment);
}

bool File::WriteInteger(int i)
{
  if (!m_os.is_open())
  {
    return false;
  }

  if (m_isBinary)
  {
    // File reads use Endian, so we must do the same for writes.
    i = Endian(i);
    m_os.write((const char*)(&i), sizeof(int));
  }
  else
  {
    m_os << i << endl;
    m_currentLine++;
  }
  
  return true;
}

bool File::WriteFloat(float f)
{
  if (!m_os.is_open())
  {
    return false;
  }

  if (m_isBinary)
  {
    f = Endian(f);
    m_os.write((const char*)(&f), sizeof(float));
  }
  else
  {
    m_os << f << endl;   
    m_currentLine++;
  }

  return true;
}

bool File::GetLine(string* pResult)
{
  Assert(pResult);
  if (m_bEOFReached) 
  {
    return false;
  }

  if (m_isBinary)
  {
    // Get characters until '\0' is found.
    char c;
    while (1)
    {
      if (GetBinary(1, (unsigned char*)&c) == 0)
      {
        m_bEOFReached = true;
        return false;
      }
      if (c)
      {
        (*pResult) += c;
      }
      else
      {
        return true;
      }
    }
  }
  else
  {
    // Get a line from the current file (only valid for Text mode files).
    if (m_pImpl->GetLine(pResult))
    {
      m_currentLine++;
      return true;
    }
  }

  return false;
}

bool File::IsComment(std::string str)
{
  if (str.empty() ) 
  {
    return true;
  }
  if (str.size() < 2)
  {
    return false;
  }

  return (str.substr(0, 2) == "//");
}

bool File::GetDataLine(string* pResult)
{
  std::string str;
  do 
  {
    if (!GetLine(&str))
    {
      return false;
    }
    // Strip whitespace from beginning and end of the string.
    Trim(&str);

  } while (IsComment(str) && More());
    
  *pResult = str;
  return true;
}

bool File::GetInteger(int* pResult)
{
  if (m_isBinary)
  {
    int bytes = GetBinary(sizeof(int), (unsigned char*)pResult);
    *pResult = Endian((int32)*pResult);
    return (bytes != 0);
  }

  // Text mode: get the next non-comment line, and atoi() it.
  std::string str;
  if (!GetDataLine(&str))
  {
    ReportError("Unexpected end of file, wanted integer.");
    return false;
  }

#if defined(_DEBUG) || defined(SCENE_EDITOR)
  // Check this is an integer - i.e. is a sequence of digits.
  int s = 0;
  int len = str.size();
  Assert(len > 0); // GetDataLine() should not return an empty line
  // Leading minus sign is ok, as long as it is followed by a number.
  if (str[0] == '-')
  {
    s = 1;
    if (len == 1) 
    {
      ReportError("'-' is not an integer.");
      return false;
    }
  }
  for (int i = s; i < len; i++)
  {
    if (!isdigit(str[i]))
    {
      string error = "'" + str + "' is not an integer.";
      ReportError(error);
      return false;
    }
  }
#endif

  *pResult = atoi(str.c_str());
  return true;
}

bool File::GetFloat(float* pResult)
{
  if (m_isBinary)
  {
    int bytes = GetBinary(sizeof(float), (unsigned char*)pResult);
    *pResult = Endian(*pResult);
    return (bytes != 0);
  }

  std::string str;
  if (!GetDataLine(&str))
  {
    return false;
  }
  *pResult = atof(str.c_str());
  return true;
}

bool File::GetToken(std::string* pResult)
{
  // For importing text files, e.g. .ASC format
  if (m_tokenLine.empty())
  {
    if (!GetDataLine(&m_tokenLine))
    {
      // No more data lines left.
      return false;
    }
  }
  Assert(!m_tokenLine.empty());
  Assert(m_pTokeniser);
  
  // 'Tokenisation' is delegated to a tokeniser object. 
  return m_pTokeniser->Tokenise(&m_tokenLine, pResult);
}

std::string File::TokenGetLineRemainder()
{
  std::string s = m_tokenLine;
  m_tokenLine = "";
  return s;
}

bool File::FindToken(const std::string& token)
{
  string s;
  while (GetToken(&s))
  {
    if (s == token)
    {
      return true;
    }
  }
  return false;
}

bool File::GetIntToken(int* pResult)
{
  // Importing text file.. no binary version.
  string s;
  if (!GetToken(&s))
  {
    return false;
  }
  *pResult = atoi(s.c_str());
  return true;
}

bool File::GetFloatToken(float* pResult)
{
  // Importing text file.. no binary version.
  string s;
  if (!GetToken(&s))
  {
    return false;
  }
  *pResult = atof(s.c_str());
  return true;
}

void File::SetTokeniser(Tokeniser* p)
{
  m_pTokeniser = p;
}

}


