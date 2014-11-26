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


#ifndef Foundation_DirectoryIterator_UNIX_INCLUDED
#define Foundation_DirectoryIterator_UNIX_INCLUDED

#include <dirent.h>
#include <string>

namespace nanogear {
   
   
   class DirectoryIteratorImpl
   {
   public:
      DirectoryIteratorImpl(const std::string& path);
      ~DirectoryIteratorImpl();
      
      void duplicate();
      void release();
      
      const std::string& get() const;
      const std::string& next();
      
   private:
      DIR*        _pDir;
      std::string _current;
      int _rc;
   };
   
   
   //
   // inlines
   //
   const std::string& DirectoryIteratorImpl::get() const
   {
      return _current;
   }
   
   
   inline void DirectoryIteratorImpl::duplicate()
   {
      ++_rc;
   }
   
   
   inline void DirectoryIteratorImpl::release()
   {
      if (--_rc == 0)
         delete this;
   }
   
   
} // namespace Poco


#endif // Foundation_DirectoryIterator_UNIX_INCLUDED
