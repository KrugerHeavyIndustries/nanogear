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

#include "nrepresentation.h"
#include "bytearray.h"

#include <iconv.h>
#include <errno.h>
#include <iterator>

#define OUTBUFLEN 4096

using nanogear::ByteArray;
using std::string;
using std::vector;
using std::pair;
using std::back_inserter;
using std::logic_error;

namespace
{
   string key_retrieval_func(const pair<string, ByteArray>& pair)
   {
      return pair.first;
   }
}

void NRepresentation::setXhtml(const string& xhtml)
{
    char outbuf[OUTBUFLEN];
   
    size_t inbytesleft = xhtml.size();
    size_t outbytesleft = OUTBUFLEN;
   
    char* inptr = const_cast<char*>(xhtml.c_str());
    char* outptr = outbuf;
   
    iconv_t cd = iconv_open("UTF-8", "ASCII");
   
    if (iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft) == -1)
    {
        switch (errno) {
            case EILSEQ:
                throw logic_error("An invalid multibyte sequence was encountered in the input.");
            case EINVAL:
                throw logic_error("An incomplete multibyte sequence was encountered in the input.");
            case E2BIG:
                throw logic_error("The output buffer has no more room for the next converted character.");
            default:
                throw logic_error("An unknown error occured.");
        }
    }

    setData("application/xhtml+xml", ByteArray(outbuf, outbuf + (OUTBUFLEN - outbytesleft)));
   
    iconv_close(cd);

    string html = xhtml;
    //QRegExp fix;

/*
    // Convert XHTML to valid HTML.
#define ELEMENT_NONEMPTY(x) \
    fix = QRegExp("<"x" (.*)/>"); \
    while (fix.indexIn(html) != -1)\
        html.replace(fix, "<"x" " + fix.cap(1) + "></"x">");
    ELEMENT_NONEMPTY("a");
    ELEMENT_NONEMPTY("abbr");
    ELEMENT_NONEMPTY("acronym");
    ELEMENT_NONEMPTY("address");
    ELEMENT_NONEMPTY("applet");
    ELEMENT_NONEMPTY("b");
    ELEMENT_NONEMPTY("bdo");
    ELEMENT_NONEMPTY("big");
    ELEMENT_NONEMPTY("blockquote");
    ELEMENT_NONEMPTY("body");
    ELEMENT_NONEMPTY("button");
    ELEMENT_NONEMPTY("caption");
    ELEMENT_NONEMPTY("center");
    ELEMENT_NONEMPTY("cite");
    ELEMENT_NONEMPTY("code");
    ELEMENT_NONEMPTY("colgroup");
    ELEMENT_NONEMPTY("dd");
    ELEMENT_NONEMPTY("del");
    ELEMENT_NONEMPTY("dfn");
    ELEMENT_NONEMPTY("dir");
    ELEMENT_NONEMPTY("div");
    ELEMENT_NONEMPTY("dl");
    ELEMENT_NONEMPTY("dt");
    ELEMENT_NONEMPTY("em");
    ELEMENT_NONEMPTY("fieldset");
    ELEMENT_NONEMPTY("font");
    ELEMENT_NONEMPTY("form");
    ELEMENT_NONEMPTY("frameset");
    ELEMENT_NONEMPTY("h1");
    ELEMENT_NONEMPTY("h2");
    ELEMENT_NONEMPTY("h3");
    ELEMENT_NONEMPTY("h4");
    ELEMENT_NONEMPTY("h5");
    ELEMENT_NONEMPTY("h6");
    ELEMENT_NONEMPTY("head");
    ELEMENT_NONEMPTY("html");
    ELEMENT_NONEMPTY("i");
    ELEMENT_NONEMPTY("iframe");
    ELEMENT_NONEMPTY("ins");
    ELEMENT_NONEMPTY("kbd");
    ELEMENT_NONEMPTY("label");
    ELEMENT_NONEMPTY("legend");
    ELEMENT_NONEMPTY("li");
    ELEMENT_NONEMPTY("map");
    ELEMENT_NONEMPTY("menu");
    ELEMENT_NONEMPTY("noframes");
    ELEMENT_NONEMPTY("noscript");
    ELEMENT_NONEMPTY("object");
    ELEMENT_NONEMPTY("ol");
    ELEMENT_NONEMPTY("optgroup");
    ELEMENT_NONEMPTY("option");
    ELEMENT_NONEMPTY("p");
    ELEMENT_NONEMPTY("pre");
    ELEMENT_NONEMPTY("q");
    ELEMENT_NONEMPTY("s");
    ELEMENT_NONEMPTY("samp");
    ELEMENT_NONEMPTY("script");
    ELEMENT_NONEMPTY("select");
    ELEMENT_NONEMPTY("small");
    ELEMENT_NONEMPTY("span");
    ELEMENT_NONEMPTY("strike");
    ELEMENT_NONEMPTY("stong");
    ELEMENT_NONEMPTY("style");
    ELEMENT_NONEMPTY("sub");
    ELEMENT_NONEMPTY("table");
    ELEMENT_NONEMPTY("tbody");
    ELEMENT_NONEMPTY("td");
    ELEMENT_NONEMPTY("textarea");
    ELEMENT_NONEMPTY("tfoot");
    ELEMENT_NONEMPTY("th");
    ELEMENT_NONEMPTY("thead");
    ELEMENT_NONEMPTY("title");
    ELEMENT_NONEMPTY("tr");
    ELEMENT_NONEMPTY("tt");
    ELEMENT_NONEMPTY("u");
    ELEMENT_NONEMPTY("ul");
    ELEMENT_NONEMPTY("var");
#undef ELEMENT_NONEMPTY


#define ELEMENT_EMPTY(x) \
    fix = QRegExp("<"x" (.*)/>"); \
    while (fix.indexIn(html) != -1)\
        html.replace(fix, "<"x" " + fix.cap(1) + ">");
    ELEMENT_EMPTY("area");
    ELEMENT_EMPTY("base");
    ELEMENT_EMPTY("basefont");
    ELEMENT_EMPTY("br");
    ELEMENT_EMPTY("col");
    ELEMENT_EMPTY("frame");
    ELEMENT_EMPTY("hr");
    ELEMENT_EMPTY("img");
    ELEMENT_EMPTY("input");
    ELEMENT_EMPTY("isindex");
    ELEMENT_EMPTY("link");
    ELEMENT_EMPTY("meta");
    ELEMENT_EMPTY("param");
#undef ELEMENT_EMPTY
 */

    setHtml(html);
}

void NRepresentation::setHtml(const string& html)
{
   setData("text/html", ByteArray(html.begin(), html.end()));
}

std::vector<NMimeType> NRepresentation::formats() const
{
   vector<NMimeType> types;
   transform(m_data.begin(), m_data.end(), back_inserter(types), key_retrieval_func);
   return types;
}


