#pragma once

#include "Response.hpp"
#include "Request.hpp"

class Request;
class Response;

class Client
{
    private :
        std::string requestBuffer;
        std::string responseBuffer;
        size_t   sentOffset;

    public :
        Client();
        void setRequest(const std::string& req);
        void setResponse(const std::string& res);
        void incremetOffset(size_t n);
        void setOffset(size_t n);
        size_t getSentOffset();
        bool served;
        std::string& getRequestBuffer();
        std::string& getResponseBuffer();

};