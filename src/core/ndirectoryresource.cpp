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

#include "ndirectoryresource.h"

#include <magic.h>
#include <QFile>
#include <QFileInfo>

#include <sstream>
#include <unordered_map>

#include "nrequest.h"
#include "nresponse.h"
#include "bytearray.h"

using nanogear::ByteArray;

NDirectoryResource::NDirectoryResource(const std::string& root)
:  m_root(QString::fromStdString(root)),
   m_indexAllowed(true)
{
    m_notAllowed.setHtml("<html><head><title>403 Forbidden</title></head><body>"
                         "<h1>Forbidden</h1>"
                         "<p>Unable to display this resource</p>"
                         "</body>");

    m_xhtmlRepr = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">"
                  "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">"
                  "<head>"
                  "<title>Index of %1</title>"
                  "<style type=\"text/css\">"
                  "a, a:active {text-decoration: none; color: blue;}"
                  "a:visited {color: #48468F;}"
                  "a:hover, a:focus {text-decoration: underline; color: red;}"
                  "body {background-color: #F5F5F5;}"
                  "h2 {margin-bottom: 12px;}"
                  "table {margin-left: 12px;}"
                  "th, td { font: 90% monospace; text-align: left;}"
                  "th { font-weight: bold; padding-right: 14px; padding-bottom: 3px;}"
                  "td {padding-right: 14px;}"
                  "td.s, th.s {text-align: right;}"
                  "div.list { background-color: white; border-top: 1px solid #646464; border-bottom: 1px solid #646464; padding-top: 10px; padding-bottom: 14px;}"
                  "div.foot { font: 90% monospace; color: #787878; padding-top: 4px;}"
                  "</style>"
                  "</head>"
                  "<body>"
                  "<h2>Index of %1</h2>"
                  "<div class=\"list\">"
                  "<table summary=\"Directory Listing\" cellpadding=\"0\" cellspacing=\"0\">"
                  "<thead><tr><th class=\"n\">Name</th><th class=\"m\">Last Modified</th><th class=\"s\">Size</th><th class=\"t\">Type</th></tr></thead>"
                  "<tbody>"
                  "%2"
                  "</tbody>"
                  "</table>"
                  "</div>"
                  "<div class=\"foot\">Nanogear</div>"
                  "</body>" 
                  "</html>";
}

void NDirectoryResource::handleGet(const NRequest& request, NResponse& response)
{
    QFileInfo pathInfo(m_root.absolutePath() + QString::fromStdString(request.resourceRef()));

    if (pathInfo.exists()) {
        if (pathInfo.isDir()) {
			QFileInfo indexFileInfo(pathInfo.absoluteFilePath() + "/" + QString::fromStdString(m_indexName));

            if (indexFileInfo.exists() && indexFileInfo.isFile()) {
                representFile(indexFileInfo, response);
            } else if (m_indexAllowed) {
                // Return an HTML representation of this directory

				    std::ostringstream htmlTableEntries;
                foreach(const QFileInfo& dirFileInfo, QDir(pathInfo.absoluteFilePath()).entryInfoList()) {
                    std::string fileType("File");
                    std::string dirIdentifier("");
                    std::string size(QString::number(dirFileInfo.size()).toStdString());

                    if (dirFileInfo.isDir()) {
                        fileType = "Directory";
                        dirIdentifier = "/";
                        size = "-";
                    }

					std::string arg1 = (pathInfo.fileName() + "/" + dirFileInfo.fileName()).toStdString();
					std::string arg2 = dirFileInfo.fileName().toStdString();
					std::string arg3 = dirIdentifier;
					
					htmlTableEntries <<	"<tr>"
										"<td class=\"n\"><a href=\"" << arg1 << "\">" << arg2 << "</a>" << arg3 << "</td>" <<
										"<td class=\"m\">" << dirFileInfo.lastModified().toString().toStdString() << "</td>" <<
										"<td class=\"s\">" << size << "</td>" <<
										"<td class=\"t\">" << fileType << "</td>"
										"</tr>\n";

                }
               
                m_directoryIndex.setXhtml(QString::fromStdString(m_xhtmlRepr).arg(pathInfo.fileName()).arg(QString::fromStdString(htmlTableEntries.str())).toStdString());

                response.setStatus(NStatus::SUCCESS_OK);
                response.setRepresentation(&m_directoryIndex);
            } else {
                response.setStatus(NStatus::CLIENT_ERROR_FORBIDDEN);
                response.setRepresentation(&m_notAllowed);
            }
        } else {
            // Send a file back to the client
            representFile(pathInfo, response);
        }
    }
}

void NDirectoryResource::representFile(const QFileInfo& pathInfo, NResponse& response)
{
    // Try with libmagic first, it has a fairly complete default database. When an error
    // occurs, mimeType will be empty
    //! \todo: Handle libmagic errors
    //! \todo: Create a thin layer over libmagic?
    magic_t magicCookie = magic_open(MAGIC_MIME_TYPE | MAGIC_SYMLINK | MAGIC_ERROR);
    magic_load(magicCookie, NULL);
	 std::string mimeType(magic_file(magicCookie, pathInfo.absoluteFilePath().toStdString().c_str()));
    magic_close(magicCookie);

    // If libmagic faild to find the MIME type, we query for custom-mapped extensions
    // if none is found we return a generic "application/octet-stream" MIME type

    if (mimeType.empty()) {
		std::string customMapping(m_mimeMappings.find(pathInfo.completeSuffix().toStdString())->first);

        if (!customMapping.empty())
            mimeType = customMapping;
        else
            mimeType = "application/octet-stream";
    }

    //! \note: Awful
    QFile file(pathInfo.absoluteFilePath());

    file.open(QIODevice::ReadOnly);
    QByteArray array = file.readAll();
    m_rawFile.setData(mimeType.c_str(), ByteArray(array.begin(), array.end()));
    file.close();

    response.setStatus(NStatus::SUCCESS_OK);
    response.setRepresentation(&m_rawFile);
}
