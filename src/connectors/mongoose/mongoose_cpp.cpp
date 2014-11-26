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

#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;

namespace nanogear
{
   #define MAX_PORT_SIZE 8
   #define SERVER_POLL_MILLISECONDS 1000
   #define unwrap() connection_cast(this)
   
   inline mg_connection* connection_cast(const Connection* connection)
   {
      return (mg_connection*)(connection);
   }
   
   inline HttpRequestHeader::value_type request_header_map(const mg_connection::mg_header& header)
   {
      return HttpRequestHeader::value_type(header.name, header.value);
   }
   
   struct send_header_t
   {
      send_header_t(const Connection* connection)
         :  m_connection(connection)
      {
      }
      
      void operator()(const HttpResponseHeader::value_type& tuple)
      {
         m_connection->sendHeader(tuple.first, tuple.second);
      }
      
      const Connection* m_connection;
   };
   
   HTTPServer::HTTPServer(int port)
      :  NServer(port)
   {
   }
   
   void HTTPServer::start()
   {
      char port_result[MAX_PORT_SIZE];
      struct mg_server* server = mg_create_server(NULL, eventHandler);
      snprintf(port_result, MAX_PORT_SIZE, "%d", port());
      mg_set_option(server, "listening_port", port_result);
      for (;;) {
         mg_poll_server(server, SERVER_POLL_MILLISECONDS);
      }
      mg_destroy_server(&server);
   }
   
   int HTTPServer::eventHandler(mg_connection* connection, mg_event event)
   {
      switch(event) {
         case MG_AUTH:
            return MG_TRUE;
         case MG_REQUEST:
            return handleRequest(reinterpret_cast<Connection*>(connection));
         default: return MG_FALSE;
      }
   }
   
   int HTTPServer::handleRequest(Connection* connection)
   {
      HttpRequestHeader requestHeader = connection->getHttpRequestHeader();
   
      NRepresentation entity(connection->getContent(), requestHeader["Content-Type"]);
      
      NPreferenceList<NMimeType> acceptedMimeTypes(getPreferenceListFromHeader<NMimeType>(requestHeader["Accept"]));
      NPreferenceList<QLocale> acceptedLocales(getPreferenceListFromHeader<QLocale>(requestHeader["Accept-Language"]));
      NPreferenceList<QTextCodec*> acceptedCharsets(getPreferenceListFromHeader<QTextCodec*>(requestHeader["Accept-Charset"]));
      
      NClientInfo clientInfo(acceptedMimeTypes, acceptedLocales, acceptedCharsets);
      
      NRequest request(connection->getMethod(), clientInfo, &entity);
      
      request.setResourceRef(connection->getUri());
      
      request.setParameters(connection->getQueryParameters());
      
      NResponse response;
      
      NResource* resource = NApplication::instance()->createRoot();
      
      resource->handleRequest(request, response);
      
      const NRepresentation* representation = response.representation();
      
      HttpResponseHeader responseHeader(response.status().code(),
                                        response.status().name(),
                                        requestHeader.getMajorVersion(),
                                        requestHeader.getMinorVersion());
      
      responseHeader.insert(make_pair("Connection", requestHeader["Connection"]));
      
      if (responseHeader.count("Connection") == 0) {
         if (responseHeader.getMajorVersion() <= 1 && responseHeader.getMinorVersion() == 0)
            responseHeader.insert(make_pair("Connection", "close"));
      }
      
      responseHeader.insert(make_pair("Server", "Nanogear"));
      
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
      
      connection->sendHttpResponseHeader(responseHeader);
      connection->sendData(responseData);
      
      delete resource;
      
      return MG_TRUE;
   }
   
   string Connection::getUri() const
   {
      return unwrap()->uri;
   }
   
   NMethod Connection::getMethod() const
   {
      return NMethod::valueOf(unwrap()->request_method);
   }
   
   unordered_map<string, string> Connection::getQueryParameters() const
   {
      unordered_map<string, string> parameters;
      size_t query_string_len = unwrap()->query_string ? strlen(unwrap()->query_string) : 0;
      
      const char* first = unwrap()->query_string;
      const char* last = unwrap()->query_string + query_string_len;
      
      const char* key_start = first;
      const char* key_end = NULL;
      const char* value_start = NULL;
      
      for (; first != last; ++first) {
         if (*first == '&') {
            parameters.insert(make_pair(string(key_start, key_end), string(value_start, first)));
            key_start = first + 1;
         }
         if (*first == '=') {
            key_end = first;
            value_start = first + 1;
         }
      }
      if (first && key_start && key_end && value_start) {
         parameters.insert(make_pair(string(key_start, key_end), string(value_start, first)));
      }
   
      return parameters;
   }
   
   char* Connection::getContent() const
   {
      return unwrap()->content;
   }
   
   int Connection::getContentLength() const
   {
      return unwrap()->content_len;
   }
   
   HttpRequestHeader Connection::getHttpRequestHeader() const
   {
      int majorVersion = 1;
      int minorVersion = 1;
      
      if (strcmp(unwrap()->http_version, "1.0") == 0) {
         majorVersion = 1;
         minorVersion = 0;
      } else if (strcmp(unwrap()->http_version, "1.1") == 0) {
         majorVersion = 1;
         minorVersion = 1;
      }

      HttpRequestHeader httpRequestHeader(majorVersion, minorVersion);
      
      transform(unwrap()->http_headers,
                unwrap()->http_headers + unwrap()->num_headers,
                inserter(httpRequestHeader, httpRequestHeader.begin()),
                request_header_map);
      
      return httpRequestHeader;
   }
   
   void Connection::sendStatus(int status)
   {
      mg_send_status(unwrap(), status);
   }
   
   void Connection::sendHeader(const std::string& name, const std::string& value) const
   {
      mg_send_header(unwrap(), name.c_str(), value.c_str());
   }
   
   void Connection::sendHttpResponseHeader(const HttpResponseHeader& header)
   {
      sendStatus(header.getStatus());
      for_each(header.begin(), header.end(), send_header_t(this));
   }
   
   void Connection::sendData(const ByteArray& data)
   {
      mg_send_data(unwrap(), data.data(), data.size());
   }
}
