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
    class HttpMessage;
    class HTTPServer;

    class HTTPServer : public NServer
    {
      private:
      
         HTTPServer(int port);

         friend HTTPServer* HTTPServer_Create(int);
      
      public:
      
         virtual void start();
      
      private:
      
         static void eventHandler(mg_connection* connection, int event, void* eventData);
      
         static void handleRequest(Connection* connection, HttpMessage* message);
    };

    inline HTTPServer* HTTPServer_Create(int port = 8080) { return new HTTPServer(port); }

    class Connection
    {
      private:
      
         Connection();
        
         mg_connection* unwrap() const;
      
      public:
      
         void sendStatus(int status) const;
      
         void sendHeader(const std::string& name, const std::string& value) const;
      
         void sendHttpResponseHeader(const HttpResponseHeader& responseHeader) const;

         void sendData(const std::vector<unsigned char>& data) const;
        
         void close() const;
    };

    class HttpMessage
    {
    private:
        
        HttpMessage();
        
        http_message* unwrap() const;
        
        std::pair<std::string, std::string> parseQueryPair(const std::string& keypair) const;

        std::unordered_map<std::string, std::string> parseQueryParameters(const std::string& encoded) const;
    
    public:
        
        std::string getUri() const;
        
        NMethod getMethod() const;
        
        HttpRequestHeader getHttpRequestHeader() const;

        std::unordered_map<std::string, std::string> getQueryParameters() const;
        
        std::string getContent() const;
        
        int getContentLength() const;
    };
}
