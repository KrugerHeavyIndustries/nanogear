#include "mongoose.h"
#include <nserver.h>
#include <nmethod.h>

#include <unordered_map>
#include <vector>

#include "httprequestheader.h"
#include "httpresponseheader.h"

namespace nanogear
{
   class HTTPServer;
   class Connection;
   class HTTPServer;
   
   class HTTPServer : public NServer
   {
      private:
      
         HTTPServer(int port);
    
         friend HTTPServer* HTTPServer_Create(int);
      
      public:
      
         virtual void start();
      
      private:
      
         static int eventHandler(mg_connection* connection, mg_event event);
      
         static int handleRequest(Connection* connection);
   };
   
   inline HTTPServer* HTTPServer_Create(int port = 8080) { return new HTTPServer(port); }
   
   class Connection
   {
      private:
      
         Connection();
      
      public:
      
         std::string getUri() const;
      
         NMethod getMethod() const;
      
         std::unordered_map<std::string, std::string> getQueryParameters() const;
      
         char* getContent() const;
      
         int getContentLength() const;
      
         HttpRequestHeader getHttpRequestHeader() const;
      
         void sendStatus(int status);
      
         void sendHeader(const std::string& name, const std::string& value);
      
         void sendHttpResponseHeader(const HttpResponseHeader& responseHeader);

         void sendData(const std::vector<unsigned char>& data);
   };
}