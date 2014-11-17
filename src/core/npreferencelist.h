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

#ifndef NPREFERENCELIST_H
#define NPREFERENCELIST_H

#include <map>
#include <vector>

#include "npreference.h"

/*!
 * \class NPreferenceList
 * \brief Convenience list of client preferences.
 *
 * Preference's are meant to be chosen from. PreferenceList provides convenience methods for doing just that.
 */

template <typename T> class NPreferenceList : public std::vector< NPreference<T> >
{
public:
    /*!
     * An empty constructor, the result is an empty list
     */
    NPreferenceList() {}


    NPreferenceList(const std::vector< NPreference<T> >& other)
            :  std::vector< NPreference<T> >(other) {}

    /*!
     * \return the Preference with the highest quality. The Preference the client wants most.
     */
    T top() const
    { return *(toMap().end()); }

    /*!
     * Out of a list of possible choices, returns the "best" one.
     * \param server the list of possible choices.
     * \return the "best" choice, or a default-constructed one if no choice at all is available.
     */
    T outOf(const std::vector<T>& server) const {
       std::map<float, T> map = toMap();

       for (typename std::map<float, T>::iterator i = map.end(); i != map.begin();) {
            --i;

          if (std::find(server.begin(), server.end(), i->second) != server.end())
             return i->second;
        }

        return T();
    }

    /*!
     * Convert this PreferenceList to a map from quality to data.
     * \return a map from Preference quality to Preference data.
     */
    std::map<float, T> toMap() const {
       std::map<float, T> ret;
       for (typename NPreferenceList<T>::const_iterator i = NPreferenceList<T>::begin();
            i != NPreferenceList<T>::end(); ++i)
       {
          ret.insert(std::make_pair<float, T>(i->quality(), i->data()));
       }
          
       //for (template <> NPreferenceList<T>::const_iter i = this->begin(); i != this->end(); ++i) {
          //ret.insert(std::make_pair<float, T>(i->quality(), i->data()));
        //}
        //std::transform(begin(), end(), std::back_inserter(ret.first()), _UnaryOperation __op)
        //foreach(const NPreference<T>& type, *this) {
         //   ret.insertMulti(type.quality(), type.data());
        //}

        return ret;
    }
};

#endif /* NPREFERENCELIST_H */
