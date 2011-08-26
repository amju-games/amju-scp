/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Socket.cpp,v $
Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

// Sockets code based on sample code in BSD Sockets Primer by Jim Frost
// http://world.std.com/~jimf/papers/sockets/sockets.html

#include "Socket.h"
#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

namespace Amju
{
Socket::Socket() 
{
#if defined(WIN32)
  m_socket = socket(AF_INET, SOCK_STREAM, 0);
#else
  m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
}

Socket::Socket(const Socket& s)
{
  m_socket = s.m_socket;
}

Socket::Socket(int s) : 
  m_socket(s) 
{
}

Socket::~Socket()
{
  // This prevents Sockets from being passed by value.
  // Instead, explictly call Close().
  //Close();

  // TODO Have a ref count so we close the socket when the last 
  // reference goes away.
}

void Socket::Close()
{
#if defined(WIN32)
  closesocket(m_socket);
#else
  close(m_socket);
#endif
}

bool Socket::WriteInteger(int i)
{
  i = htonl(i);
  int n = sizeof(int);
  return (WriteData((char*) &i, n) == n);
}

bool Socket::WriteString(const std::string& s)
{
  // Prepend char data with string length.
  int n = s.size();
  if (!WriteInteger(n))
  {
    return false;
  }
  return (WriteData(s.c_str(), n) == n);
}

bool Socket::WriteRawString(const std::string& s)
{
  int n = s.size();
  return (WriteData(s.c_str(), n) == n);
}

bool Socket::GetInteger(int* p)
{
  int n = sizeof(int);
  if (ReadData((char*) p, n) == n)
  {
    *p = ntohl(*p);
    return true;
  }
  return false;
}

bool Socket::GetFloat(float* p)
{
  // TODO HACK This assumes sizeof(float) == sizeof(long)
  int n = sizeof(float);
  if (ReadData((char*) p, n) == n)
  {
    *(unsigned long*)p = htonl(*(unsigned long*)p);
    return true;
  }
  return false;
}

bool Socket::WriteFloat(float f)
{
  // TODO HACK This assumes sizeof(float) == sizeof(long)
  *(unsigned long*)&f = htonl(*(unsigned long*)&f);
  int n = sizeof(float);
  return (WriteData((char*) &f, n) == n);
}

bool Socket::GetString(std::string* p)
{
  // Get number of chars in string
  int len;
  if (!GetInteger(&len))
  {
    return false;
  }
  if (len == 0)
  {
    return false;
  }
  // Get the characters
  char* buf = new char[len + 1];
  bool b = (ReadData(buf, len) == len);
  if (b)
  {
    buf[len] = '\0';
    *p = buf;
  }
  delete [] buf;
  return b;
}

bool Socket::GetRawString(std::string* p)
{
  static const int n = 1000;
  char buf[n+1];
  int br = 0;
#if defined(WIN32)
  br = recv(m_socket,buf,n, 0);
#else
  br = read(m_socket,buf,n);
#endif
  buf[br] = 0;
  *p = buf;
  return true;
}

bool Socket::WriteChar(char c)
{
  // Don't use WriteData(), because if the other end of the socket goes away, it
  // will go into a 100% CPU loop! 

  int br = 0;

#if defined(WIN32)
  br = send(m_socket, &c, 1, 0);
#else
  br = send(m_socket, &c, 1, 0);
#endif

  return (br == 1);
}

bool Socket::GetChar(char* buf)
{
  // As for WriteChar(), don't use ReadData(). It will screw up if the socket closes.

  int br = 0;

#if defined(WIN32)
  br = recv(m_socket, buf, 1, 0);
#else
  br = read(m_socket, buf, 1);
#endif

  return (br == 1);
}

bool Socket::Bind(int  portnum)
{
  const int maxnamelength = 255;
  char  myname[maxnamelength + 1];
  struct sockaddr_in sa;
  struct hostent *hp;

  memset(&sa, 0, sizeof(struct sockaddr_in)); /* clear our address */
  gethostname(myname, maxnamelength);           /* who are we? */
  hp= gethostbyname(myname);                  /* get our address info */
  if (!hp)                             /* we don't exist !? */
  {
    return false;
  }
  sa.sin_family= hp->h_addrtype;              /* this is our host address */
  sa.sin_port= htons(portnum);                /* this is our port number */

  if (bind(m_socket,(struct sockaddr *)&sa,sizeof(struct sockaddr_in)) < 0) 
  {
    return false;
  }
  return true;
}

int Socket::ReadData(char* buf, int n)
{
  int bcount = 0; /* counts bytes read */
  int br = 0;     /* bytes read this pass */

  while (bcount < n) 
  {
#if defined(WIN32)
    br = recv(m_socket,buf,n-bcount, 0);
#else
    br = read(m_socket,buf,n-bcount);
#endif
    if (br > 0) 
    {
      bcount += br;                /* increment byte counter */
      buf += br;                   /* move buffer ptr for next read */
    }
    else if (br < 0)               /* signal an error to the caller */
    {
      return(-1);
    }
  }

  return bcount;
}

int Socket::WriteData(const char* buf, int n)
{
  int bcount = 0; /* counts bytes sent */
  int br = 0;     /* bytes sent this pass */

  while (bcount < n) 
  {
#if defined(WIN32)
    br = send(m_socket,buf+bcount,n-bcount, 0);
#else
    br = send(m_socket,buf+bcount,n-bcount, 0);
#endif
    if (br > 0) 
    {
      bcount += br;                /* increment byte counter */
      buf += br;                   /* move buffer ptr for next read */
    }
    else if (br < 0)               /* signal an error to the caller */
    {
      return(-1);
    }
  }

  return bcount;
}
}
