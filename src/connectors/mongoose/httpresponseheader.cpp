//
//  httpresponseheader.cpp
//  nanogear
//
//  Created by Chris Kruger on 18/11/2014.
//
//

#include "httpresponseheader.h"
#include <nmimetype.h>

namespace nanogear
{
   using std::string;
   
   HttpResponseHeader::HttpResponseHeader(int status, const string& text, int majorVersion, int minorVersion) :
      m_status(status),
      m_text(text),
      m_majorVersion(majorVersion),
      m_minorVersion(minorVersion)
   {
   }
   
   void HttpResponseHeader::setContentType(const NMimeType& type)
   {
      (*this)["Content-Type"] = type.toString();
   }
}