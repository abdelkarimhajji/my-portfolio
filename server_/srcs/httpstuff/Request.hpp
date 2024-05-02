#pragma once

#include "../config/DataConfig.hpp"
#include "RequestMethods.hpp"
#include "../server/Server.hpp"
#include "Response.hpp"
#include <map>
#include <algorithm>
class Response;
class Request {
    private:
        std::string requestEntity;
        std::string requestMethod;
        std::string requestRessource;
        std::string httpVersion;
        std::string queryString;
        std::map<std::string, std::string> headers;
        std::string body;

        std::string path;
        std::string location;
        std::string host;
        std::string port;
        // validation
        int checkAllowedChars(std::string value);
        int validateUri(DataConfig &config);
        int validRequest(DataConfig config);

        void buildPath(DataConfig &config);

        // parsing
        void parseRequest(std::string buffer, std::string delim);
        void parseRequestLine(std::string buffer);
        void parseHeaders(std::string buffer);
        std::string& trimSpaces(std::string& val);
        // karim request
        // void    checkValidPath();
        
        void        checkWichServer();
        void        parseHostPort();
    public:
        // constructor
        Request(std::string buffer);
        ~Request();

        // getters
        std::string getRequestMethod() const;
        std::string getRequestRessource() const;
        std::string getHttpVersion() const;
        std::string getQueryString() const;
        std::string getHeader(std::string key) const;
        std::string getBody() const;
        std::string getPath() const;
        std::string getLocation() const;
        std::string getHost() const;
        std::string getPort() const;

        int methodAllowed(DataConfig config);
        Response handleRequest(DataConfig config);
        Response runHttpMethod(DataConfig config);
        // void buildPath(DataConfig config);
        // void printHeaders();
};