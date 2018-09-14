//
// DirectoryIterator.cpp
//
// $Id: //poco/1.4/Foundation/src/DirectoryIterator.cpp#1 $
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


#include "directoryiterator.h"


#if defined(POCO_OS_FAMILY_WINDOWS) && defined(POCO_WIN32_UTF8)
#include "DirectoryIterator_WIN32U.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "DirectoryIterator_WIN32.cpp"
#elif defined(__APPLE__)
#include "directoryiteratorunix.cpp"
#else
#include "DirectoryIterator_VMS.cpp"
#endif

namespace nanogear
{
   DirectoryIterator::DirectoryIterator(): m_impl(NULL)
   {
   }

   DirectoryIterator::DirectoryIterator(const std::string& path): m_path(path), m_impl(new platform::DirectoryIterator(path))
   {
      m_path.makeDirectory();
      m_path.setFileName(m_impl->get());
      m_file = m_path;
   }
   
   DirectoryIterator::DirectoryIterator(const DirectoryIterator& iterator): m_path(iterator.m_path), m_impl(iterator.m_impl)
   {
      if (m_impl)
      {
         m_impl->duplicate();
         m_file = m_path;
      }
   }

   DirectoryIterator::DirectoryIterator(const File& file): m_path(file.path()), m_impl(new platform::DirectoryIterator(file.path()))
   {
      m_path.makeDirectory();
      m_path.setFileName(m_impl->get());
      m_file = m_path;
   }

   DirectoryIterator::DirectoryIterator(const Path& path): m_path(path), m_impl(new platform::DirectoryIterator(path.toString()))
   {
      m_path.makeDirectory();
      m_path.setFileName(m_impl->get());
      m_file = m_path;
   }

   DirectoryIterator::~DirectoryIterator()
   {
      if (m_impl) m_impl->release();
   }

   DirectoryIterator& DirectoryIterator::operator = (const DirectoryIterator& it)
   {
      if (m_impl) m_impl->release();
      m_impl = it.m_impl;
      if (m_impl)
      {
         m_impl->duplicate();
         m_path = it.m_path;
         m_file = m_path;
      }
      return *this;
   }

   DirectoryIterator& DirectoryIterator::operator = (const File& file)
   {
      if (m_impl) m_impl->release();
      m_impl = new platform::DirectoryIterator(file.path());
      m_path.parseDirectory(file.path());
      m_path.setFileName(m_impl->get());
      m_file = m_path;
      return *this;
   }

   DirectoryIterator& DirectoryIterator::operator = (const Path& path)
   {
      if (m_impl) m_impl->release();
      m_impl = new platform::DirectoryIterator(path.toString());
      m_path = path;
      m_path.makeDirectory();
      m_path.setFileName(m_impl->get());
      m_file = m_path;
      return *this;
   }

   DirectoryIterator& DirectoryIterator::operator = (const std::string& path)
   {
      if (m_impl) m_impl->release();
      m_impl = new platform::DirectoryIterator(path);
      m_path.parseDirectory(path);
      m_path.setFileName(m_impl->get());
      m_file = m_path;
      return *this;
   }

   DirectoryIterator& DirectoryIterator::operator ++ ()
   {
      if (m_impl)
      {
         m_path.setFileName(m_impl->next());
         m_file = m_path;
      }
      return *this;
   }

   DirectoryIterator DirectoryIterator::operator ++ (int dummy)
   {
      if (m_impl)
      {
         m_path.setFileName(m_impl->next());
         m_file = m_path;
      }
      return *this;
   }
} // namespace nanogear
