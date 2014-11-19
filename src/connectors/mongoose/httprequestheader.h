//
//  httprequestheader.h
//  nanogear
//
//  Created by Chris Kruger on 18/11/2014.
//
//

#ifndef __nanogear__httprequestheader__
#define __nanogear__httprequestheader__

#include <map>

namespace nanogear
{
   class HttpRequestHeader : public std::map<std::string, std::string>
   {
      public:
      
         HttpRequestHeader(int majorVerion, int minorVersion);
      
         inline int getMajorVersion() { return m_majorVersion; }
      
         inline int getMinorVersion() { return m_minorVersion; }
   
      private:
      
         const int m_majorVersion;
         const int m_minorVersion;

   };
}
#endif /* defined(__nanogear__httprequestheader__) */
