//
// DirectoryIterator_UNIX.cpp
//
// $Id: //poco/1.4/Foundation/src/DirectoryIterator_UNIX.cpp#2 $
//
// Library: Foundation
// Package: Filesystem
// Module:  DirectoryIterator
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "directoryiteratorunix.h"
#include "path.h"
#include "file.h"

namespace nanogear { namespace platform
{
   DirectoryIterator::DirectoryIterator(const std::string& path)
   : 	m_dir(0),
   	m_rc(1)
   {
      Path p(path);
      p.makeFile();
      
#if defined(POCO_VXWORKS)
      m_dir = opendir(const_cast<char*>(p.toString().c_str()));
#else
      m_dir = opendir(p.toString().c_str());
#endif
      if (!m_dir) nanogear::File::handleLastError(path);
      
      next();
   }

   DirectoryIterator::~DirectoryIterator()
   {
      if (m_dir) closedir(m_dir);
   }

   const std::string& DirectoryIterator::next()
   {
      do
      {
         struct dirent* entry = readdir(m_dir);
         if (entry)
            m_current = entry->d_name;
         else
            m_current.clear();
      }
      while (m_current == "." || m_current == "..");
      return m_current;
   }

} // namespace platform
} // namespace nanogear
