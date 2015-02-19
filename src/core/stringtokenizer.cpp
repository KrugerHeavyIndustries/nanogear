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
#include "stringtokenizer.h"

#include <string>
#include <algorithm>

namespace nanogear
{
   StringTokenizer::StringTokenizer(const std::string& str, const std::string& separators, int options)
   {
      std::string::const_iterator it = str.begin();
      std::string::const_iterator end = str.end();
      std::string token;
      bool doTrim = ((options & TOK_TRIM) != 0);
      bool ignoreEmpty = ((options & TOK_IGNORE_EMPTY) != 0);
      bool lastToken = false;
      
      for (;it != end; ++it)
      {
         if (separators.find(*it) != std::string::npos)
         {
            if (doTrim) trim(token);
            if (!token.empty() || !ignoreEmpty)m_tokens.push_back(token);
            if (!ignoreEmpty) lastToken = true;
            token = "";
         }
         else
         {
            token += *it;
            lastToken = false;
         }
      }
      
      if (!token.empty())
      {
         if (doTrim) trim(token);
         if (!token.empty()) m_tokens.push_back(token);
      }
      else if (lastToken) m_tokens.push_back("");
   }
   
   StringTokenizer::~StringTokenizer()
   {
   }

   void StringTokenizer::trim (std::string& token)
   {
      std::size_t front = 0, back = 0, length = token.length();
      std::string::const_iterator tIt = token.begin();
      std::string::const_iterator tEnd = token.end();
      for (; tIt != tEnd; ++tIt, ++front)
      {
         if (!::isspace(*tIt)) break;
      }
      if (tIt != tEnd)
      {
         std::string::const_reverse_iterator tRit = token.rbegin();
         std::string::const_reverse_iterator tRend = token.rend();
         for (; tRit != tRend; ++tRit, ++back)
         {
            if (!::isspace(*tRit)) break;
         }
      }
      token = token.substr(front, length - back - front);
   }
   
   std::size_t StringTokenizer::count(const std::string& token) const
   {
      std::size_t result = 0;
      TokenVec::const_iterator it = std::find(m_tokens.begin(), m_tokens.end(), token);
      while(it != m_tokens.end())
      {
         result++;
         it = std::find(++it, m_tokens.end(), token);
      }
      return result;
   }

   std::size_t StringTokenizer::find(const std::string& token, std::size_t pos) const
   {
      TokenVec::const_iterator it = std::find(m_tokens.begin() + pos, m_tokens.end(), token);
      if ( it != m_tokens.end() )
      {
         return it - m_tokens.begin();
      }
      //throw NotFoundException(token);
   }
   
   bool StringTokenizer::has(const std::string& token) const
   {
      TokenVec::const_iterator it = std::find(m_tokens.begin(), m_tokens.end(), token);
      return it != m_tokens.end();
   }
   
   std::size_t StringTokenizer::replace(const std::string& oldToken, const std::string& newToken, std::size_t pos)
   {
      std::size_t result = 0;
      TokenVec::iterator it = std::find(m_tokens.begin() + pos, m_tokens.end(), oldToken);
      while(it != m_tokens.end())
      {
         result++;
         *it = newToken;
         it = std::find(++it, m_tokens.end(), oldToken);
      }
      return result;
   }
} // namespace nanogear

