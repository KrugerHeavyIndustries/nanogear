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

#ifndef NREPRESENTATION_H
#define NREPRESENTATION_H

#include <unordered_map>
#include <vector>

#include "nmimetype.h"
#include "npreferencelist.h"

/*!
 * \class NRepresentation
 * \brief A resource's representation
 *
 * "A representation is a sequence of bytes, plus representation metadata to
 * describe those bytes."
 *
 * \see Roy Fielding's dissertation at:
 *      http://roy.gbiv.com/pubs/dissertation/rest_arch_style.htm#sec_5_2_1_2
 */

class NRepresentation
{
public:
    NRepresentation() {}

    NRepresentation(const std::string& data, const NMimeType& mimeType = NMimeType("text/plain"))
    { setData(mimeType, std::vector<unsigned char>(data.begin(), data.end())); }
   
    NRepresentation(const std::vector<unsigned char>& data, const NMimeType& mimeType = NMimeType("text/plain"))
    { setData(mimeType, data); }

    /*!
     * \return the data (in raw form) attached to this representation
     */
    std::vector<unsigned char> data(const NPreferenceList<NMimeType>& mimeTypes) const
    { return data(format(mimeTypes)); }

    /*!
     * \return the best matching MIME type out of the MIME types supported by
     *    the client
     */
    NMimeType format(const NPreferenceList<NMimeType>& mimeTypes) const
    { return mimeTypes.outOf(formats()); }
   
   /*!
    * \return the list of formats this representation can give back to the
    *    clients as MIME types.
    */
    std::vector<NMimeType> formats() const;
 
    /*!
     * \return the data (in raw form) attached to this representation
     */
   std::vector<unsigned char> data(const NMimeType& mimeType) const
	{ return m_data.at(mimeType.toString()); }

    /*!
     * An overloaded function provided for convenience
     * \param mimeType A reference to a MimeType
     * \param data A reference to the raw data
     */
    void setData(const NMimeType& mimeType, const std::vector<unsigned char>& data)
    { m_data.insert(std::pair<std::string, std::vector<unsigned char> >(mimeType.toString(), data)); }

    /*!
     * \return true if the requested format is available
     */
    bool hasFormat(const NMimeType& mimeType) const
    { return m_data.find(mimeType.toString()) != m_data.end(); }
   
    /*!
     * A facility to easily add Xhtml content to this representation
     * \param xhtml A string representing the XHTML document
     */
    void setXhtml(const std::string& xhtml);

    /*!
     * \return true if this representation is holding an XHTML document
     */
    bool hasXhtml() const
    { return hasFormat("application/xhtml+xml"); }

    /*!
     * \return the XHTML document attached to this representation
     */
    std::string xhtml() const
    {
       const std::vector<unsigned char>& d = data("application/xhtml+xml");
       return std::string(d.begin(), d.end());
    }

    void setHtml(const std::string& html);
   
    std::string html() const
    {
       const std::vector<unsigned char>& d = data("application/html");
       return std::string(d.begin(), d.end());
    }

    void setText(const std::string& text);
   
  private:
   
   std::unordered_map<std::string, std::vector<unsigned char> > m_data;
};

#endif /* NREPRESENTATION_H */
