/*
 * Nanogear - C++ web development framework
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

#include <NApplication>
#include <NResource>
#include <NRequest>
#include <NResponse>
#include <NRepresentation>
#include <NStatus>
#include <NRouter>
#include <mongoose/mongoose_cpp.h>

#include "datetimeformat.h"
#include "datetimeformatter.h"

using namespace nanogear;

class TimeResource : public NResource {
public:
    virtual void handleGet(const NRequest& request, NResponse& response) {
        N_UNUSED(request)

        m_representation.setText(DateTimeFormatter::format(DateTime::now(), DateTimeFormat::RFC1123_FORMAT)); 

        response.setStatus(NStatus::SUCCESS_OK);
        response.setRepresentation(&m_representation);
    }

private:
    NRepresentation m_representation;
};

class TimeserveApplication : public NApplication {
public:
    TimeserveApplication(int argc, char** argv) : NApplication(argc, argv) {}
    
    virtual NResource* createRoot() {
        return new TimeResource();
    }
};

int main(int argc, char** argv) {
    TimeserveApplication app(argc, argv);
    app.setServer(HTTPServer_Create());
    return app.exec();
}

