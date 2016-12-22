//
//  mongoose_cpp.cpp
//  nanogear
//
//  Created by Chris Kruger on 18/11/2014.
//
//

#include "mongoose_cpp.h"
#include "nutility.h"
#include "httpresponseheader.h"

#include <nrequest.h>
#include <nrepresentation.h>
#include <nclientinfo.h>
#include <napplication.h>
#include <bytearray.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;

namespace std {
    ostream& operator <<(ostream& os, const pair<string, string>& pair) {
        return os << pair.first << ": " << pair.second << "\r\n";
    }
}

namespace nanogear
{
    #define MAX_PORT_SIZE 8
    #define SERVER_POLL_MILLISECONDS 1000
    
    inline HttpRequestHeader::value_type request_header_map(mg_str& name, mg_str& value)
    {
        return HttpRequestHeader::value_type(name.p, value.p);
    }

    inline bool is_3xx_redirection(int code)
    {
        return 300 < code && code < 400;
    }
   
    HTTPServer::HTTPServer(int port)
      :  NServer(port)
    {
    }

    void HTTPServer::start()
    {
        const char *err_str;
        char http_port[MAX_PORT_SIZE];
        snprintf(http_port, MAX_PORT_SIZE, "%d", port());
       
        struct mg_mgr mgr;
        struct mg_connection *nc;
        struct mg_bind_opts bind_opts;
       
        mg_mgr_init(&mgr, NULL);
       
        memset(&bind_opts, 0, sizeof(bind_opts));
        bind_opts.error_string = &err_str;
       
        nc = mg_bind_opt(&mgr, http_port, eventHandler, bind_opts);
        mg_set_protocol_http_websocket(nc);
        for (;;) {
            mg_mgr_poll(&mgr, SERVER_POLL_MILLISECONDS);
        }
        mg_mgr_free(&mgr);
    }

   void HTTPServer::eventHandler(mg_connection* connection, int event, void* eventData)
   {
       http_message* message = (http_message*)eventData;
       switch(event) {
         case MG_EV_HTTP_REQUEST:
               handleRequest(reinterpret_cast<Connection*>(connection), reinterpret_cast<HttpMessage*>(message));
         default:
               break;
      }
   }
   
   void HTTPServer::handleRequest(Connection* connection, HttpMessage* message)
   {
      HttpRequestHeader requestHeader = message->getHttpRequestHeader();

      NRepresentation entity(message->getContent(), requestHeader["Content-Type"]);
      
      NPreferenceList<NMimeType> acceptedMimeTypes(getPreferenceListFromHeader<NMimeType>(requestHeader["Accept"]));
      NPreferenceList<QLocale> acceptedLocales(getPreferenceListFromHeader<QLocale>(requestHeader["Accept-Language"]));
      NPreferenceList<QTextCodec*> acceptedCharsets(getPreferenceListFromHeader<QTextCodec*>(requestHeader["Accept-Charset"]));
      
      NClientInfo clientInfo(acceptedMimeTypes, acceptedLocales, acceptedCharsets);
      
      NRequest request(message->getMethod(), clientInfo, &entity);
      
      request.setResourceRef(message->getUri());
      request.setParameters(message->getQueryParameters());
      
      NResponse response;
      
      NResource* resource = NApplication::instance()->createRoot();
      
      resource->handleRequest(request, response);
      
      const NRepresentation* representation = response.representation();
      
      HttpResponseHeader responseHeader(response.status().code(),
                                        response.status().name(),
                                        requestHeader.getMajorVersion(),
                                        requestHeader.getMinorVersion());
      
      //responseHeader.insert(make_pair("Connection", requestHeader["Connection"]));
     
      if (is_3xx_redirection(response.status().code())) {
         responseHeader.insert(make_pair("Location", response.location()));
      }

      //if (responseHeader.count("Connection") == 0) {
      //   if (responseHeader.getMajorVersion() <= 1 && responseHeader.getMinorVersion() == 0)
            //responseHeader.insert(make_pair("Connection", "close"));
      //}
      
      //responseHeader.insert(make_pair("Server", "Nanogear"));
      
      if (response.expirationDate().isValid()) {
         //responseHeader.insert(make_pair("Expires", responseHeader.getExpirationDate().toUTC());
      }
      
      ByteArray responseData;
      
      if (representation != NULL) {
         if (representation->formats().size() == 1) {
            responseHeader.setContentType(representation->formats().front());
            responseData = representation->data(representation->formats().front());
         } else {
            responseHeader.setContentType(representation->format(clientInfo.acceptedMimeTypes()));
            responseData = representation->data(clientInfo.acceptedMimeTypes());
          
         }
      }
       
      responseHeader.setContentLength(responseData.size());
       
      connection->sendHttpResponseHeader(responseHeader);
      connection->sendData(responseData);
       
       //connection->close();
      delete resource;
   }
   
    http_message* HttpMessage::unwrap() const
    {
        return (http_message*)this;
    }
    
    pair<string, string> HttpMessage::parseQueryPair(const string& keypair) const
    {
        size_t eq = keypair.find("=");
        if (eq == std::string::npos)
            throw "Failed to find '=' in key-value pair.";
        return make_pair<string, string>(keypair.substr(0, eq), keypair.substr(eq + 1));
    }
    
    unordered_map<string, string> HttpMessage::parseQueryParameters(const string& query) const
    {
        unordered_map<string, string> parameters;
        if (query.empty()) {
            return parameters;
        }
    
        size_t last = 0;
        bool more = true;
        while (more)
        {
            size_t next = query.find('&', last + 1);
            parameters.insert(parseQueryPair((next == string::npos) ? query.substr(last) : query.substr(last, next - last)));
            last = next + 1;
            if (next == string::npos)
            {
                more = false;
            }
        }
        return parameters;
    }

    string HttpMessage::getUri() const
    {
        return unwrap()->uri.len > 0 ? string(unwrap()->uri.p, unwrap()->uri.len) : "";
    }
   
    NMethod HttpMessage::getMethod() const
    {
        return NMethod::valueOf(string(unwrap()->method.p, unwrap()->method.len));
    }
   
    unordered_map<string, string> HttpMessage::getQueryParameters() const
    {
        for (int i = 0; unwrap()->header_names[i].len > 0; ++i)
        {
            if (strncmp(unwrap()->header_names[i].p, "Content-Type", 12) == 0 &&
                strncmp(unwrap()->header_values[i].p, "application/x-www-form-urlencoded", 33) == 0)
            {
                return parseQueryParameters(string(unwrap()->body.p, unwrap()->body.len));
            }
        }
        return parseQueryParameters(string(unwrap()->query_string.p, unwrap()->query_string.len));
    }
   
    string HttpMessage::getContent() const
    {
        return string(unwrap()->body.p, unwrap()->body.len);
    }
   
   int HttpMessage::getContentLength() const
   {
      return unwrap()->body.len;
   }
   
    HttpRequestHeader HttpMessage::getHttpRequestHeader() const
    {
        int majorVersion = 1;
        int minorVersion = 1;
        if (strncmp(unwrap()->proto.p, "HTTP/1.0", 8) == 0) {
           majorVersion = 1;
           minorVersion = 0;
        } else if (strncmp(unwrap()->proto.p, "HTTP/1.1", 8) == 0) {
           majorVersion = 1;
           minorVersion = 1;
        }

        HttpRequestHeader httpRequestHeader(majorVersion, minorVersion);
      
        for (int i = 0; unwrap()->header_names[i].len > 0; i++) {
            httpRequestHeader[string(unwrap()->header_names[i].p, unwrap()->header_names[i].len)] = string(unwrap()->header_values[i].p, unwrap()->header_values[i].len);
        }
        return httpRequestHeader;
   }
   
   void Connection::sendHttpResponseHeader(const HttpResponseHeader& header) const
   {
       ostringstream result;
       copy(header.begin(), header.end(), ostream_iterator<pair<string, string> >(result));
       mg_send_response_line(unwrap(), header.getStatus(), result.str().c_str());
   }
   
   void Connection::sendData(const ByteArray& data) const
   {
      mg_send(unwrap(), data.data(), data.size());
   }
    
    void Connection::close() const
    {
        unwrap()->flags = MG_F_CLOSE_IMMEDIATELY;
    }
    
    mg_connection* Connection::unwrap() const {
        return (mg_connection*)(this);
    }
}
