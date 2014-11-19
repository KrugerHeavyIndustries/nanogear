//
//  httprequestheader.cpp
//  nanogear
//
//  Created by Chris Kruger on 18/11/2014.
//
//

#include "httprequestheader.h"
#include <string>

using namespace std;

namespace nanogear
{
   HttpRequestHeader::HttpRequestHeader(int majorVersion, int minorVersion)
   : map<string, string>(),
      m_majorVersion(majorVersion),
      m_minorVersion(minorVersion)
   {
      
   }
}