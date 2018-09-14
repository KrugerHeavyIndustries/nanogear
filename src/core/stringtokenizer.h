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

#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include <vector>
#include <cstddef>

namespace nanogear
{
   
   class StringTokenizer
   /// A simple tokenizer that splits a string into
   /// tokens, which are separated by separator characters.
   /// An iterator is used to iterate over all tokens.
   {
   public:
      enum Options
      {
         TOK_IGNORE_EMPTY = 1, /// ignore empty tokens
         TOK_TRIM	 = 2  /// remove leading and trailing whitespace from tokens
      };
      
      typedef std::vector<std::string> TokenVec;
      typedef TokenVec::const_iterator Iterator;
      
      StringTokenizer(const std::string& str, const std::string& separators, int options = 0);
      /// Splits the given string into tokens. The tokens are expected to be
      /// separated by one of the separator characters given in separators.
      /// Additionally, options can be specified:
      ///   * TOK_IGNORE_EMPTY: empty tokens are ignored
      ///   * TOK_TRIM: trailing and leading whitespace is removed from tokens.
      
      ~StringTokenizer();
      /// Destroys the tokenizer.
      
      Iterator begin() const;
      Iterator end() const;
      
      const std::string& operator [] (std::size_t index) const;
      /// Returns const reference the index'th token.
      /// Throws a RangeException if the index is out of range.
      
      std::string& operator [] (std::size_t index);
      /// Returns reference to the index'th token.
      /// Throws a RangeException if the index is out of range.
      
      bool has(const std::string& token) const;
      /// Returns true if token exists, false otherwise.
      
      std::size_t find(const std::string& token, std::size_t pos = 0) const;
      /// Returns the index of the first occurence of the token
      /// starting at position pos.
      /// Throws a NotFoundException if the token is not found.
      
      std::size_t replace(const std::string& oldToken, const std::string& newToken, std::size_t pos = 0);
      /// Starting at position pos, replaces all subsequent tokens having value
      /// equal to oldToken with newToken.
      /// Returns the number of modified tokens.
      
      std::size_t count() const;
      /// Returns the total number of tokens.
      
      std::size_t count(const std::string& token) const;
      /// Returns the number of tokens equal to the specified token.
      
   private:
      StringTokenizer(const StringTokenizer&);
      StringTokenizer& operator = (const StringTokenizer&);
      
      void trim (std::string& token);
      
      TokenVec m_tokens;
   };
   
   
   //
   // inlines
   //

   inline StringTokenizer::Iterator StringTokenizer::begin() const
   {
      return m_tokens.begin();
   }
   
   
   inline StringTokenizer::Iterator StringTokenizer::end() const
   {
      return m_tokens.end();
   }

   inline std::string& StringTokenizer::operator [] (std::size_t index)
   {
      //if (index >= _tokens.size()) throw RangeException();
      return m_tokens[index];
   }

   inline const std::string& StringTokenizer::operator [] (std::size_t index) const
   {
      //if (index >= _tokens.size()) throw RangeException();
      return m_tokens[index];
   }
   
   inline std::size_t StringTokenizer::count() const
   {
      return m_tokens.size();
   }
} // namespace nanogear

#endif // STRINGTOKENIZER_H
