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

#include "nmimetype.h"

void NMimeType::fromString(const std::string& type)
{
    m_whole = type;
    m_type.clear();
    m_subtype.clear();
}

const std::string& NMimeType::toString() const
{
    return m_whole;
}

bool NMimeType::operator==(const NMimeType& type) const
{
    return m_whole == type.m_whole;
}

bool NMimeType::isConcrete() const
{
	return m_whole.find("*") == std::string::npos;
}

const std::string& NMimeType::type() const
{
    if (m_type.empty())
        const_cast<NMimeType*>(this)->m_type = m_whole.substr(0, m_whole.find_first_of('/'));

    return m_type;
}

const std::string& NMimeType::subtype() const
{
    if (m_type.empty())
        const_cast<NMimeType*>(this)->m_subtype = m_whole.substr(m_whole.find_first_of('/'));

    return m_subtype;
}

bool NMimeType::isCompatible(const NMimeType& other) const
{
	return other.m_whole.find(m_whole) != std::string::npos || m_whole.find(other.m_whole) != std::string::npos;
}
