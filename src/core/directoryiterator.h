/*
 * Nanogear - C++ web development framework
 *
 * This library is based on Restlet (R) <http://www.restlet.org> by Noelios Technologies
 * Copyright (C) 2005-2008 by Noelios Technologies <http://www.noelios.com>
 * Restlet is a registered trademark of Noelios Technologies. All other marks and
 * trademarks are property of their respective owners.
 *
 * Copyright (C) 2008-2009 Chris Kruger.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Foundation_DirectoryIterator_INCLUDED
#define Foundation_DirectoryIterator_INCLUDED

#include "file.h"
#include "path.h"

namespace nanogear {

   namespace platform
   {
   	class DirectoryIterator;
   }

   class DirectoryIterator
   /// The DirectoryIterator class is used to enumerate
   /// all files in a directory.
   ///
   /// DirectoryIterator has some limitations:
   ///   * only forward iteration (++) is supported
   ///   * an iterator copied from another one will always
   ///     point to the same file as the original iterator,
   ///     even is the original iterator has been advanced
   ///     (all copies of an iterator share their state with
   ///     the original iterator)
   ///   * because of this you should only use the prefix
   ///     increment operator
   {
   public:
      DirectoryIterator();
      /// Creates the end iterator.
      
      DirectoryIterator(const std::string& path);
      /// Creates a directory iterator for the given path.
      
      DirectoryIterator(const DirectoryIterator& iterator);
      /// Creates a directory iterator for the given path.
      
      DirectoryIterator(const File& file);
      /// Creates a directory iterator for the given file.
      
      DirectoryIterator(const Path& path);
      /// Creates a directory iterator for the given path.
      
      virtual ~DirectoryIterator();
      /// Destroys the DirectoryIterator.
      
      const std::string& name() const;
      /// Returns the current filename.
      
      const Path& path() const;
      /// Returns the current path.
      
      DirectoryIterator& operator = (const DirectoryIterator& it);
      DirectoryIterator& operator = (const File& file);
      DirectoryIterator& operator = (const Path& path);
      DirectoryIterator& operator = (const std::string& path);
      
      virtual DirectoryIterator& operator ++ ();   // prefix
      
      //@ deprecated
      DirectoryIterator operator ++ (int); // postfix
      /// Please use the prefix increment operator instead.
      
      const File& operator * () const;
      File& operator * ();
      const File* operator -> () const;
      File* operator -> ();
      
      bool operator == (const DirectoryIterator& iterator) const;
      bool operator != (const DirectoryIterator& iterator) const;
      
   protected:
      Path m_path;
      File m_file;
      
   private:

      platform::DirectoryIterator* m_impl;
   };

   //
   // inlines
   //
   inline const std::string& DirectoryIterator::name() const
   {
      return m_path.getFileName();
   }

   inline const Path& DirectoryIterator::path() const
   {
      return m_path;
   }

   inline const File& DirectoryIterator::operator * () const
   {
      return m_file;
   }

   inline File& DirectoryIterator::operator * ()
   {
      return m_file;
   }

   inline const File* DirectoryIterator::operator -> () const
   {
      return &m_file;
   }

   inline File* DirectoryIterator::operator -> ()
   {
      return &m_file;
   }

   inline bool DirectoryIterator::operator == (const DirectoryIterator& iterator) const
   {
      return name() == iterator.name();
   }

   inline bool DirectoryIterator::operator != (const DirectoryIterator& iterator) const
   {
      return name() != iterator.name();
   }
   
   
} // namespace nanogear


#endif // Foundation_DirectoryIterator_INCLUDED
