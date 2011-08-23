/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Directory.cpp,v $
Revision 1.1.4.1.4.1  2005/09/29 19:16:09  jay
Mac: directory for Save data is now under /Users/Shared, so it should be
accessible to everyone.
Also changed chmod to explicitly 777 for dirs

Revision 1.1.4.1  2004/10/01 09:04:52  Administrator
GetDesktopDir fails on some windows versions - use current dir in this case.

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include <iostream>
#include "Directory.h"
#include "GlueFile.h"
#include "FileImplGlue.h"
#include "StringUtils.h"
#include "File.h"

#if defined(WIN32)
#include <io.h>
#ifdef MSVC
#include "Direct.h"
#endif
#include <windows.h>
#include <shlobj.h>

#if !defined CSIDL_COMMON_APPDATA
#define CSIDL_COMMON_APPDATA 35
#endif

#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif
#ifdef MACOSX
#include <Carbon/Carbon.h>
#endif

namespace Amju
{
struct Remover : public std::unary_function<std::string, bool>
{
  Remover(const std::string s) : m_s(s) {}
  bool operator()(const std::string& s) const 
  {  
    return s.find(m_s) == std::string::npos; 
  }
  std::string m_s;
};

void Filter(
  std::vector<std::string>* pResult, 
  const std::string& substr,
  bool include)
{
  if (include)
  {
    pResult->erase(
      std::remove_if(pResult->begin(), pResult->end(), Remover(substr)),
      pResult->end()
    );
  }
  else
  {
    pResult->erase(
      std::remove_if(pResult->begin(), pResult->end(), 
        std::unary_negate<Remover>(substr)),
      pResult->end()
    );
  }
}

bool Dir(
  const std::string& directory, 
  std::vector<std::string>* pResult, 
  bool useGlue /* = true */)
{
  if (useGlue && FileImplGlue::IsReady())
  {
    // Return list of files in Glue File.
    return FileImplGlue::GetGlueFile()->Dir(pResult);
  }
 
  // Return list of files in directory.
#if defined(WIN32)
  _finddata_t fileinfo;
  std::string s = directory + "*.*";
  long f = _findfirst(s.c_str(), &fileinfo);
  if (f == -1)
  {
    return true; // no files
  }

  do
  {
    std::string f = directory; // TODO flag
    f += ToLower(fileinfo.name);
    pResult->push_back(f);
  }
  while (_findnext(f, &fileinfo) == 0); // 0 => success
  return true;

#else
  DIR* dirp = opendir(directory.c_str());
  if (!dirp)
  {
    return false;
  }

  while (struct dirent* dp = readdir(dirp))
  {
    std::string f = dp->d_name;
    f = ToLower(f);

    pResult->push_back(f);
  }
  closedir(dirp);
  return true;
#endif

  return true; 
}

bool MkDir(const std::string& dir)
{
#ifdef _DEBUG
  std::cout << "Creating directory " << dir.c_str() << "\n";
#endif

#ifdef WIN32
  int r = mkdir(dir.c_str());
#else
  int r = mkdir(dir.c_str(), 0);
  // Set permission: rwxr-xr-x
  // No, should be rwxrwxrwx
  chmod(dir.c_str(), S_IRWXU | S_IWGRP | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IWOTH);
#endif

#ifdef _DEBUG
  if (r != 0)
  {
    std::cout << "FAILED to create directory " << dir.c_str() << "\n"; 
  }
#endif

  return (r == 0);
}

extern const char * APPLICATION_NAME;

#ifdef WIN32
bool ShGetFolderPath(uint32 pathType, std::string* pResult)
{
  TCHAR path[MAX_PATH];
  // Function Sig for SHGetFolderPath
  typedef UINT (CALLBACK* FuncPtr)(HWND, int, HANDLE, DWORD, LPSTR);
  // Load library shell32.dll
  HINSTANCE hdll = LoadLibrary("shell32.dll");
  if (hdll)
  {
#ifdef _DEBUG
    std::cout << "Loaded shell32.dll\n";
#endif
    FuncPtr funcPtr = (FuncPtr)GetProcAddress(hdll, "SHGetFolderPathA");
    if (funcPtr)
    {
#ifdef _DEBUG
      std::cout << "Got address for SHGetFolderPathA\n";
#endif
      HRESULT hr = (funcPtr)(0, pathType, 0, 0, path); 
      if (SUCCEEDED(hr))
      {
        *pResult = path;
        return true;
      }
    }
  }
  return false;
}
#endif

std::string GetDesktopDir()
{
#ifdef WIN32
  static bool first = true;
  static std::string s;
  if (first)
  {
    first = false;
    bool success = ShGetFolderPath(CSIDL_DESKTOPDIRECTORY, &s);
    if (!success)
    {
#ifdef _DEBUG
      std::cout << "Failed to get SHGetFolderPathA\n";    
#endif
      s = "";
    }
  }
  return s;
#endif

#ifdef MACOSX
  static std::string s;
  static bool first = true;
  if (first)
  {
    first = false;
 
    static const int PATH_SIZE=200;
    char path[PATH_SIZE];
    // Get the user's desktop directory.
    FSRef fsref;
    FSFindFolder(kOnAppropriateDisk, kCurrentUserFolderType, kDontCreateFolder, &fsref);
    FSRefMakePath(&fsref, (unsigned char*)path, PATH_SIZE);

    s = path;
    s += "/Desktop/";
 //   s += APPLICATION_NAME;
//    s += "/";
//    MkDir(s);
  }
  return s;
#endif
}

std::string GetSaveDir()
{
  // Get directory for saving preferences, etc.
  // Not in the application directory, so the app can be upgraded without losing
  // preferences etc.
#ifdef WIN32

  static bool first = true;
  static std::string s;
  if (first)
  {
    first = false;
    bool success = ShGetFolderPath(CSIDL_APPDATA, &s);
    
    // This only works on Win2000+
    // TODO Try to load the function at run time. If this fails fall
    // back to a hack.

    if (success)
    {
      s += "\\";
      s += APPLICATION_NAME;
      s += "\\";
     
      MkDir(s);
    }
    else
    {
#ifdef _DEBUG
      std::cout << "Failed to get SHGetFolderPathA\n";    
#endif
      // Use the Data directory. 
      s = File::GetRoot();
    }
  }
  return s;

#endif // WIN32

#ifdef MACOSX
  static std::string s;
  static bool first = true;
  if (first)
  {
    first = false;

/* 
    static const int PATH_SIZE=200;
    char path[PATH_SIZE];
    // Get the user's home directory.
    FSRef fsref;
    FSFindFolder(kOnAppropriateDisk, kCurrentUserFolderType, kDontCreateFolder, &fsref);
    FSRefMakePath(&fsref, (unsigned char*)path, PATH_SIZE);

    s = path;
    s += "/Library/Application Support/";
*/

    s += "/Users/Shared/";
    s += APPLICATION_NAME;
    s += "/";

    MkDir(s);
  }

  return s;
#endif
}

}

