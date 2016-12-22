//
//  Header.h
//  nanogear
//
//  Created by Chris Kruger on 18/11/2014.
//
//

#ifndef nanogear_Header_h
#define nanogear_Header_h

#include <map>
#include <string>

class NMimeType;

namespace nanogear
{
   
class HttpResponseHeader : public std::map<std::string, std::string>
{
public:
   
   HttpResponseHeader(int status, const std::string& text, int majorVersion, int minorVersion);
   
   void setContentType(const NMimeType& type);
    
   void setContentLength(int bytes);
   
   inline int getStatus() const { return m_status; }
   
   inline int getMajorVersion() const { return m_majorVersion; }
   
   inline int getMinorVersion() const { return m_minorVersion; }
   
private:
   
   const int m_status;
   
   const int m_majorVersion;
   const int m_minorVersion;
   
};
   
}

#endif
