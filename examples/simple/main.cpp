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

#include "application.h"
#include "resource.h"
#include "request.h"
#include "response.h"
#include "representation.h"
#include "status.h"
#include "router.h"
#include <mongoose/mongoose_cpp.h>

using namespace nanogear;

class RootResource : public Resource {
public:
    RootResource() : m_representation("<h1>Simple example</h1><br/><a href=\"/second\">Another resource</a>", "text/html") {}

    virtual void handleGet(const Request& request, Response& response) {
        N_UNUSED(request)
        response.setStatus(Status::SUCCESS_OK);
        response.setRepresentation(&m_representation);
    }

private:
    Representation m_representation;
};

class SecondResource : public Resource {
public:
    SecondResource() : m_representation("<h1>Another resource</h1>", "text/html") {}

    virtual void handleGet(const Request& request, Response& response) {
        N_UNUSED(request)
        response.setStatus(Status::SUCCESS_OK);
        response.setRepresentation(&m_representation);
    }

private:
    Representation m_representation;
};

class SimpleApplication : public Application {
public:
    SimpleApplication(int argc, char** argv) : Application(argc, argv) {}
    
    virtual Resource* createRoot() {
        Router* router = new Router();
        RootResource* root = new RootResource();
        SecondResource* secondResource = new SecondResource();
        router->attach("/", root);
        router->attach("/second", secondResource);
        
        return router;
    }
};

int main(int argc, char** argv) {
    SimpleApplication app(argc, argv);
    app.setServer(HTTPServer_Create());
    return app.exec();
}

