//
// Path_UNIX.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Path_UNIX.h#1 $
//
// Library: Foundation
// Package: Filesystem
// Module:  Path
//
// Definition of the PathImpl class fo rUnix.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PATH_UNIX_H
#define PATH_UNIX_H

#include <vector>

namespace nanogear { namespace platform
{
   class Path
   {
   public:
      static std::string current();
      static std::string home();
      static std::string temp();
      static std::string null();
      static std::string expand(const std::string& path);
      static void listRoots(std::vector<std::string>& roots);
   };
} // namespace platform
} // namespace nanogear

#endif // PATH_UNIX_H
