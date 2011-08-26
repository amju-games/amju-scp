/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GlueFile.cpp,v $
Revision 1.1.10.5  2007/07/04 22:28:06  Administrator
Function must return value: strangely only MSVC picks this up..?

Revision 1.1.10.4  2007/07/04 18:29:32  jay
Add ref counting to GlueFIleBinaryData, so it can be passed by value without
 trashing the data until there are no more refs.

Revision 1.1.10.3  2006/08/30 21:12:52  Administrator
Get Pool to build in MSVC

Revision 1.1.10.2  2006/03/09 23:18:53  jay
Merge binary data proxy class from trunk

Revision 1.4  2006/02/17 18:59:08  jay
Added new GetBinary() overload. This returns an _object_ containing a
pointer to a buffer. The buffer can be allocated by the object, which then
owns the buffer; or, the pointer can point to memory allocated by
something else.

Revision 1.3  2006/02/16 13:25:18  jay
Fixed endian issue so Glue now works on Mac + Win.
Added lots of debug output - turn on with GLUE_FILE_DEBUG

Revision 1.2  2006/01/16 14:10:15  jay
Fix warnings

Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GlueFile.h"
#include "Endian.h"
#include "StringUtils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "File.h"

using namespace std;
namespace Amju
{
void ReportError(const std::string&);

// The first position in the GlueFile where a sub-file will go.
// If there are no sub-files in the GlueFile, this is where the Table
// goes - but only conceptually, as the table is empty too.
const uint32 BASE = 2 * sizeof(uint32);

GlueFileBinaryData::GlueFileBinaryData()
{
  m_pBuf = 0;
  m_allocated = false;
  m_refCount = 0;
  IncCount();
}

GlueFileBinaryData::~GlueFileBinaryData()
{
  DecCount();
}

void GlueFileBinaryData::Copy(const GlueFileBinaryData& rhs)
{
  m_pBuf = rhs.m_pBuf;
  m_allocated = rhs.m_allocated;
  m_refCount = rhs.m_refCount;
}

void GlueFileBinaryData::IncCount()
{
  ++m_refCount;
}

void GlueFileBinaryData::DecCount()
{
  Assert(m_refCount > 0);
  --m_refCount;
  if (m_allocated && m_refCount == 0)
  {
    delete [] m_pBuf;
  }
}

GlueFileBinaryData& GlueFileBinaryData::operator=(const GlueFileBinaryData& rhs)
{
  DecCount();
  Copy(rhs);
  IncCount();
  return *this;
}

GlueFileBinaryData::GlueFileBinaryData(const GlueFileBinaryData& rhs)
{
  Copy(rhs);
  IncCount();
}

void GlueFileBinaryData::Allocate(uint32 size)
{
  m_pBuf = new unsigned char [size];
  m_allocated = true;
}

unsigned char* const GlueFileBinaryData::GetBuffer()
{
  return m_pBuf;
}

void GlueFileBinaryData::SetBuffer(unsigned char* pBuf)
{
  if (m_allocated)
  {
    delete [] m_pBuf;
  }
  m_pBuf = pBuf;
}

GlueFile::GlueFile()
{
  m_tablePos = 0;
}

GlueFile::~GlueFile()
{
#if defined(_DEBUG)
  PrintUnused();
#endif
}

void GlueFile::PrintUnused()
{
  if (m_unusedFiles.empty())
  {
    // This should be the case for distribution release.
    return;
  }

  ofstream unused("unused.txt", ios::out | ios::trunc);
  FileList::iterator it;
  int i = 0;
  unused << "Unused files follow" << endl;
  uint32 totalSize = 0;
  char buf[256]; // have to sprintf, op<< not working for some reason :-( 
  for (it = m_unusedFiles.begin(); it != m_unusedFiles.end(); ++it, ++i)
  {
    string unusedFile = *it;
    // Get the file size for this unused file, so we can see how much we can save!
    uint32 filesize = m_table[unusedFile].first;
    totalSize += filesize;

    sprintf(buf, "%d: %s      %u bytes\n", i, unusedFile.c_str(), filesize);

    unused << buf;
  }

  sprintf(buf, "Total bytes: %u\n", totalSize);
  unused << buf;

  // Now do something really useful: generate a batch file to delete the unused files!
  ofstream batch("del_unused.bat", ios::out | ios::trunc);
  batch << "REM delete unused files. Machine generated file." << endl;
  batch << "mkdir unused" << endl;

  for (it = m_unusedFiles.begin(); it != m_unusedFiles.end(); ++it)
  {
    string line = "move ";
    line += *it;
    line += " unused";

    batch << line << endl;
  }  
}

bool GlueFile::CreateGlueFile(const std::string gluefilename)
{
  int mode = ios::out | ios::binary;

#ifdef MSVC
  m_gluefile.open(gluefilename.c_str(), mode);
#else
  m_gluefile.open(gluefilename.c_str(), (std::_Ios_Openmode)mode);
#endif

  if (!m_gluefile.is_open())
  {
    return false;
  }
  // Set up the GlueFile format. As the file is empty, it's easy.
  // The number of files is zero, and the positon of the table is
  // at position 8, i.e. following these two uint32s. But there's no table.
  m_tablePos = BASE;
  WriteTable();

  return true;
}

bool GlueFile::SimpleOpen(const std::string gluefilename, bool read)
{
  int mode = ios::binary;
  if (read)
  {
    mode |= ios::in;
  }
  else
  {
    mode |= ios::in | ios::out;
  }

  string fileAndPath = File::GetRoot() + gluefilename; 
  
#ifdef MSVC
  m_gluefile.open(fileAndPath.c_str(), mode);
#else
  m_gluefile.open(fileAndPath.c_str(), (std::_Ios_Openmode)mode);
#endif

  if (!m_gluefile.is_open())
  {
    return false;
  }
  return true;
}

bool GlueFile::OpenGlueFile(const std::string gluefilename, bool read)
{
  if (!SimpleOpen(gluefilename, read))
  {
    return false;
  }

  return ReadTable();
}

GlueFileBinaryData GlueFile::GetBinary(uint32 seekPos, uint32 numbytes)
{
  GlueFileBinaryData data;
  data.Allocate(numbytes);
  GetBinary(seekPos, numbytes, const_cast<unsigned char*>(data.GetBuffer()));
  return data;  
}

uint32 GlueFile::GetBinary(uint32 numbytes, unsigned char* pBuffer)
{
  m_gluefile.read((char*)pBuffer, numbytes);
  return numbytes;
}

uint32 GlueFile::GetBinary(uint32 seekPos, uint32 numbytes, unsigned char* pBuffer)
{
#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: GetBinary: seek: " << seekPos << " num bytes: " << numbytes << "\n";
#endif

  // *** NB Not MT Safe ***
  uint32 pos = GetPos();
  SetPos(seekPos);
  uint32 bytesRead = GetBinary(numbytes, pBuffer);
  SetPos(pos);
  return bytesRead;
}

bool GlueFile::AddItem(const std::string& filename)
{
  string lowname = ToLower(filename);

  // Check for sub-file already existing.
  if (m_table.find(lowname) != m_table.end())
  {
    // Sub-file already exists
    std::string s = "GLUE FILE: file already exists in glue file: ";
    s += filename;
    ReportError(s);
    return false;
  }

  // Stick the contents of the file onto the end of the final sub-file.
  // Then update the number of files, the table position, and
  // rewrite the table.
  
  // Get the new sub-file size.
  struct stat buf;  
  if (stat(filename.c_str(), &buf) != 0)
  {
    // Couldn't get info for the sub-file. 
    std::string s = "GLUE FILE: failed to stat file: ";
    s += filename;
    ReportError(s);
    return false;
  }

  uint32 subfilesize = buf.st_size;

  // Open the new sub-file for reading.
  ifstream subfile;
  subfile.open(filename.c_str(), ios::in | ios::binary);
  if (!subfile.is_open())
  {
    std::string s = "GLUE FILE: failed to open file for reading: ";
    s += filename;
    ReportError(s);
    return false;
  }

  // Work out where the new sub-file should go in the GlueFile.
  // It's the position of the final sub-file plus its size.
  uint32 newpos = BASE;
  FileList::reverse_iterator rit = m_filenames.rbegin();
  if (rit != m_filenames.rend())
  {
    string last = *rit;
    uint32 lastsize = (m_table[last]).first;
    uint32 lastpos = (m_table[last]).second;
    newpos = lastpos + lastsize;
  } 
  // Seek to the new position.
  m_gluefile.seekg(newpos);

  // Write the new sub-file to the GlueFile.
#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: Writing file " << filename.c_str() << " starting at pos: " << newpos << " size: " << subfilesize << "\n";
#endif
  unsigned char* filebuf = new unsigned char[subfilesize];
  subfile.read((char*)filebuf, subfilesize);
  m_gluefile.write((char*)filebuf, subfilesize);
  delete filebuf;

  // Add the new sub-file to the table.
  m_table[lowname] = make_pair(subfilesize, newpos);
  // This reorders m_table, so we also maintain a list of the files
  // in the order in which they were added to the glue file.
  m_filenames.push_back(lowname);

  // New table position
  m_tablePos += subfilesize;

  // Rewrite the table.
  WriteTable();

  return true;
}

uint32 GlueFile::GetPos()
{
  return m_gluefile.tellg();
}

void GlueFile::SetPos(uint32 pos)
{
  m_gluefile.seekg(pos);
}

uint32 GlueFile::GetSize(const std::string& subfilename) const
{
  string lowname = ToLower(subfilename);
  FileTable::const_iterator it = m_table.find(lowname);
  if (it == m_table.end())
  {
    return 0;
  }
  return it->second.first;
}

bool GlueFile::GetSeekBase(const std::string& subfilename, uint32* pResult)
{
  string lowname = ToLower(subfilename);
  if (m_table.find(lowname) == m_table.end())
  {
    return false;
  } 

  *pResult = m_table[lowname].second;

  // Something has referred to this sub-file, so remove it from the list of 
  // unused files.
  m_unusedFiles.remove(lowname);

  return true;
}

bool GlueFile::ReadTable()
{
  // Get the number of sub-files. This is the uint32 at file pos zero.
  uint32 numFiles = 0;
  //m_gluefile.seekg(0);
  GetBinary(0,  sizeof(uint32), (unsigned char*)&numFiles);
  numFiles = Endian(numFiles);
#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: no of files: " << numFiles << "\n";
#endif

  // Get the file pos of the table. This is the next uint32.  
  //m_gluefile.read((char*)&m_tablePos, sizeof(uint32));
  GetBinary(4, sizeof(uint32), (unsigned char*)&m_tablePos);
  m_tablePos = Endian(m_tablePos);
#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: table start: " << m_tablePos << "\n";
#endif

  // Seek to the table start.
  uint32 tablePos = m_tablePos;
  //m_gluefile.seekg(m_tablePos);

  uint32 pos = BASE;
  // For each sub-file, get the name and file pos.
  for (uint32 i = 0; i < numFiles; i++)
  {
    // Get the length of the sub-file name.
    uint32 subfilenamelen = 0;
    //m_gluefile.read((char*)&subfilenamelen, sizeof(uint32));
    GetBinary(tablePos, sizeof(uint32), (unsigned char*)&subfilenamelen);
    tablePos += sizeof(uint32);
    subfilenamelen = Endian(subfilenamelen);
    unsigned char buf[1000]; 
    //m_gluefile.read(buf, subfilenamelen);
    GetBinary(tablePos, subfilenamelen, buf);
    buf[subfilenamelen] = '\0';

#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: file " << i << " size of name: " << subfilenamelen << " name: \"" << buf << "\"\n";
#endif

    tablePos += subfilenamelen;

    string lowname = ToLower((char*)buf);

    // Now get the size of this sub-file.
    uint32 size = 0;
    //m_gluefile.read((char*)&size, sizeof(uint32));
    GetBinary(tablePos, sizeof(uint32), (unsigned char*)&size);
    tablePos += sizeof(uint32);

    size = Endian(size);
   
#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: Found file: " << lowname.c_str() << " size: " << size << " pos: " << pos << "\n";
#endif
 
    // Store the sub-file's size and position..
    m_table[lowname] = make_pair(size, pos);
    // .. also append to this list so we keep the order in which the 
    // sub-files were added.
    m_filenames.push_back(lowname);

    pos += size;
  }

  // Copy the list of all subfiles. As subfiles are 'opened', they are removed
  // from the unused list. The ones left are printed out in the dtor.
  m_unusedFiles = m_filenames;

  return true;
}

bool GlueFile::WriteTable()
{
  // Write the number of files.
  uint32 numFiles = m_table.size();
  numFiles = Endian(numFiles);
  m_gluefile.seekg(0);
  m_gluefile.write((char*)&numFiles, sizeof(uint32));

  // Write the new table position. 
  uint32 tablepos = Endian(m_tablePos);
  m_gluefile.write((char*)&tablepos, sizeof(uint32));
  
  // Seek to the table position
  m_gluefile.seekg(m_tablePos);

  // Iterate over sub-file list, writing to the GlueFile.
  for (FileList::iterator it = m_filenames.begin(); it != m_filenames.end(); ++it)
  {
    // Get the length of the sub-file name.
    uint32 subfilenamelen = it->length();
    uint32 endiansubfilenamelen = Endian(subfilenamelen);

#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: Write table: writing size of name \"" << it->c_str() << "\": " << subfilenamelen << "\n";
#endif

    m_gluefile.write((char*)&endiansubfilenamelen, sizeof(uint32));

#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: Write table: writing name \"" << it->c_str() << "\"\n";
#endif

    // Write the string
    m_gluefile.write(it->c_str(), subfilenamelen);

    // Write the size of this sub-file.
    uint32 size = m_table[*it].first;

#ifdef GLUE_FILE_DEBUG
std::cout << "GLUE FILE: Write table: writing size of file \"" << it->c_str() << "\": " << size << "\n";
#endif

    size = Endian(size);
    m_gluefile.write((char*)&size, sizeof(uint32));
  }

  return true;  
}

bool GlueFile::Dir(vector<string>* pResult)
{
  // Table was read when GlueFile was opened.
  for (FileList::iterator it = m_filenames.begin(); it != m_filenames.end(); ++it)
  {
    pResult->push_back(*it);
  }
  return true;
}

}
