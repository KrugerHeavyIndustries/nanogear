/*
 * Nanogear - C++ web development framework
 *
 * This library is based on Restlet (R) <http://www.restlet.org> by Noelios Technologies
 * Copyright (C) 2005-2008 by Noelios Technologies <http://www.noelios.com>
 * Restlet is a registered trademark of Noelios Technologies. All other marks and
 * trademarks are property of their respective owners.
 *
 * Copyright (C) 2008-2009 Lorenzo Villani.
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

#ifndef CONNECTORS_IHTTP_NUTILITY_H
#define CONNECTORS_IHTTP_NUTILITY_H 1

#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QLocale>

#include <cstdlib>

#include <npreference.h>
#include <npreferencelist.h>
#include <nmimetype.h>

/**! 
 * Implicit converter to DRY up getPreferenceListFromHeader
 */
namespace utility
{
	namespace implicit
	{
		template<typename T>
		struct ArgConverter
		{
			ArgConverter(const T& type) : m_member(type) {};
			
			operator T () { return m_member; }
			
			T m_member;
		};

		template<>
		struct ArgConverter<NMimeType>
		{
         ArgConverter(const std::string& type) : m_member(type) {};
			
			operator NMimeType () { return NMimeType(m_member); }
			
         std::string m_member;
		};
      
      template<>
      struct ArgConverter<QLocale>
      {

         ArgConverter(const std::string& type) : m_member(type) {};
         
         operator QLocale () { return QLocale(QString::fromStdString(m_member)); }
         
         std::string m_member;
      };
	}
   
   std::vector<std::string> split(const std::string& s, const std::string& sep)
   {
      std::vector<std::string> v;
      std::string::size_type i = 0;
      std::string::size_type j = s.find(sep);
      
      if (j == std::string::npos)
      {
         v.push_back(s);
      }
      
      while (j != std::string::npos) {
         v.push_back(s.substr(i, j - i));
         i = ++j;
         j = s.find(sep, j);
         
         if (j == std::string::npos)
            v.push_back(s.substr(i, s.length()));
      }
      
      return v;
   }

   float valueOf1OrDefault(std::vector<std::string>& pair, float def)
   {
      if (pair.size() > 1)
         return ::atof(pair.at(1).c_str());
      return def;
   }
   
   template <typename T>
   NPreference<T> makePreference(const std::string& item)
   {
      std::vector<std::string> pair = split(item, ";q=");
      return NPreference<T>(utility::implicit::ArgConverter<T>(pair.at(0)), valueOf1OrDefault(pair, 1.0f));
   }
}


/*!
 * Convert an HTTP header to a PreferenceList.
 * \arg h The value of an HTTP header.
 */
template <typename T>
NPreferenceList<T> getPreferenceListFromHeader(const std::string& h)
{
    NPreferenceList<T> accept;

    std::string header = h;
   
    std::vector<std::string> list = utility::split(header, ",");
    std::transform(list.begin(), list.end(), std::back_inserter(accept), utility::makePreference<T>);
   
    return accept;
}

template <>
NPreferenceList<QTextCodec*> getPreferenceListFromHeader(const std::string& h)
{
    NPreferenceList<QTextCodec*> accept;
    QString header = QString::fromStdString(h);
    foreach(const QString& item, header.remove(" ").split(",")) {
        QStringList pair = item.split(";q=");
        NPreference<QTextCodec*> preference(QTextCodec::codecForName(pair.at(0).toUtf8()), pair.value(1, "1").toFloat());
        accept.push_back(preference);
    }

    return accept;
}

#endif /* CONNECTORS_IHTTP_NUTILITY_H */
