//
// DirectoryIterator_UNIX.h
//
// $Id: //poco/1.4/Foundation/include/Poco/DirectoryIterator_UNIX.h#1 $
//
// Library: Foundation
// Package: Filesystem
// Module:  DirectoryIterator
//
// Definition of the DirectoryIteratorImpl class for UNIX.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NANOGEAR_PLATFORM_DIRECTORY_ITERATOR_H
#define NANOGEAR_PLATFORM_DIRECTORY_ITERATOR_H

#include <dirent.h>
#include <string>

namespace nanogear { namespace platform
{
   class DirectoryIterator
   {
   public:
      DirectoryIterator(const std::string& path);
      ~DirectoryIterator();
      
      void duplicate();
      void release();
      
      const std::string& get() const;
      const std::string& next();
      
   private:
      DIR*        m_dir;
      std::string m_current;
      int m_rc;
   };

   //
   // inlines
   //
   const std::string& DirectoryIterator::get() const
   {
      return m_current;
   }

   inline void DirectoryIterator::duplicate()
   {
      ++m_rc;
   }
   
   inline void DirectoryIterator::release()
   {
      if (--m_rc == 0)
         delete this;
   }
   
} // namespace platform
} // namespace nanogear


#endif // NANOGEAR_PLATFORM_DIRECTORY_ITERATOR_H
