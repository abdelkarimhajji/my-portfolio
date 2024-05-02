#include "Response.hpp"
#include "../server/Server.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>

std::string Response::getMimeType(std::string fileExtension) {
    std::map<std::string, std::string> contentType;
    contentType.insert(std::make_pair("txt", "text/plain"));
    contentType.insert(std::make_pair("html", "text/html"));
    contentType.insert(std::make_pair("htm", "text/html"));
    contentType.insert(std::make_pair("php", "text/html"));
    contentType.insert(std::make_pair("css", "text/css"));
    contentType.insert(std::make_pair("js", "text/javascript"));
    contentType.insert(std::make_pair("json", "application/json"));
    contentType.insert(std::make_pair("jpg", "image/jpeg"));
    contentType.insert(std::make_pair("jpeg", "image/jpeg"));
    contentType.insert(std::make_pair("png", "image/png"));
    contentType.insert(std::make_pair("gif", "image/gif"));
    contentType.insert(std::make_pair("svg", "image/svg+xml"));

    // audio mime type
    contentType.insert(std::make_pair("mp3", "audio/mpeg"));
    contentType.insert(std::make_pair("mp4", "video/mp4"));
    contentType.insert(std::make_pair("ogg", "audio/ogg"));
    contentType.insert(std::make_pair("wav", "audio/wav"));
    contentType.insert(std::make_pair("aac", "audio/aac"));
    contentType.insert(std::make_pair("flac", "audio/flac"));
    contentType.insert(std::make_pair("m4a", "audio/mp4"));
    contentType.insert(std::make_pair("webm", "audio/webm"));
    contentType.insert(std::make_pair("midi", "audio/midi"));
    contentType.insert(std::make_pair("wma", "audio/x-ms-wma"));
    contentType.insert(std::make_pair("ra", "audio/x-pn-realaudio"));
    
    // other mime types
    contentType.insert(std::make_pair("pdf", "application/pdf"));
    contentType.insert(std::make_pair("xml", "application/xml"));
    contentType.insert(std::make_pair("rss", "application/rss+xml"));
    contentType.insert(std::make_pair("atom", "application/atom+xml"));
    contentType.insert(std::make_pair("zip", "application/zip"));
    contentType.insert(std::make_pair("gzip", "application/gzip"));
    contentType.insert(std::make_pair("tar", "application/x-tar"));
    contentType.insert(std::make_pair("xls", "application/vnd.ms-excel"));
    contentType.insert(std::make_pair("doc", "application/msword"));
    contentType.insert(std::make_pair("ppt", "application/vnd.ms-powerpoint"));
    contentType.insert(std::make_pair("odt", "application/vnd.oasis.opendocument.text"));
    contentType.insert(std::make_pair("odp", "application/vnd.oasis.opendocument.presentation"));
    contentType.insert(std::make_pair("ods", "application/vnd.oasis.opendocument.spreadsheet"));
    contentType.insert(std::make_pair("markdown", "text/markdown"));
    contentType.insert(std::make_pair("csv", "text/csv"));
    contentType.insert(std::make_pair("yaml", "application/x-yaml"));
    contentType.insert(std::make_pair("rdf", "application/rdf+xml"));
    std::map<std::string, std::string>::iterator it = contentType.find(fileExtension);
    if (it != contentType.end()) {
        return (it->second);
    } else {
        return ("application/octet-stream");
    }
}

Response::Response() {
    httpVersion = "HTTP/1.1";
    code = 0;
    status = "";
    contentType = "";
    contentLength = 0;
    body = "";
    responseEntity = "";
    // socket = 0;
    // state = 0;
    // offset = 0;
};

Response::Response(const Response& ref) {
    httpVersion = ref.httpVersion;
    code = ref.code;
    status = ref.status;
    contentType = ref.contentType;
    contentLength = ref.contentLength;
    body = ref.body;
    responseEntity = ref.responseEntity;
    // socket = ref.offset;
    // state = ref.state;
    // offset = ref.offset;
};


Response& Response::operator=(const Response &ref) {
    responseEntity = ref.responseEntity;
    httpVersion = ref.httpVersion;
    code = ref.code;
    status = ref.status;
    contentType = ref.contentType;
    contentLength = ref.contentLength;
    body = ref.body;
    // socket = ref.offset;
    // state = ref.state;
    // offset = ref.offset;
    return (*this);
}

void Response::buildResponse(unsigned int code) {
    std::ostringstream ss;
    setStatus(code);
    ss << httpVersion << " " << code << " " << status << "\r\n" 
        << "Content-Type: " << contentType << "\r\n" 
        << "Content-Length: " << contentLength << "\r\n";
    if (headers.size() >= 1) {
        for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
            ss << it->first << " " << it->second << "\r\n";
        }
    }
    ss << "\r\n" << body;
    responseEntity = ss.str();
}

// build response overload for handling error pages
void Response::buildResponse(DataConfig &config, std::string location, unsigned int code) {
    std::ostringstream ss;
    std::ostringstream errorPageSs;
    setStatus(code);
    
    // first check if it's a location, if yes get the path of the error page if it exists
    std::vector<Location>::iterator locationdata = config.getSpecificLocation(location);
    std::vector<Location> locations = config.getLocation();
    if (locationdata != locations.end()) {
        for (size_t i = 0; i < locations.size(); i++) {
            // if this location has an error page for the specific code
        }
    }
    // if the location of the error page is not present, check if it's present in the config file
    if (errorPageSs.str().size() == 0) {
        std::vector<ErrorPage> errorPages = config.getErrorPage();
        for (size_t i = 0; i < errorPages.size(); i++) {
            if (atoi(errorPages[i].error.c_str()) == static_cast<int>(code)) {
                std::cout << errorPages[i].page << std::endl;
                struct stat statbuf;
                if (stat(errorPages[i].page.c_str(), &statbuf) != 0 || S_ISDIR(statbuf.st_mode)) {
                    errorPageSs << config.getRoot() << errorPages[i].page;
                } else {
                    errorPageSs << errorPages[i].page;
                }
            }
        }
    }
    // if the error page path is not specified in a location nor the root, use the default one
    struct stat statbuf;
    if (errorPageSs.str().size() == 0 || (stat(errorPageSs.str().c_str(), &statbuf) != 0 || S_ISDIR(statbuf.st_mode))) {
        errorPageSs.str("");
        errorPageSs << config.getRoot() << "errorPages/" << code << ".html"; 
    }

    std::ifstream file(errorPageSs.str().c_str());
    if (!file) {
        buildResponse(code);
        return ;
    }
    ss << file.rdbuf();
    setContentType(".html");
    setContentLength(ss.str().size());
    setResponseBody(ss.str());
    ss.str("");
    ss << httpVersion << " " << code << " " << status << "\r\n" 
        << "Content-Type: " << contentType << "\r\n" 
        << "Content-Length: " << contentLength << "\r\n";
    if (headers.size() >= 1) {
        for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
            ss << it->first << " " << it->second << "\r\n";
        }
    }
    ss << "\r\n" << body;
    responseEntity = ss.str();
}

void Response::buildResponseWithCgi(Response& response, DataConfig& config, Request& request, std::string path) {
    CgiOutput  data;

    std::string location = request.getLocation().empty() ? "/" : request.getLocation();
    if (config.getSpecificLocation(location)->cgiExtension != "") {
        data = Cgi::CallCgi(path, request, "/", config);

        if(data.getCgiError() == "error")
            response.buildResponse(config, location, INTERNAL_SERVER_ERROR);
        else if(data.getCgiError() == "time out")
            response.buildResponse(GATEWAY_TIMEOUT);
        else if(data.getLocation().empty())
        {
            response.setContentType(path);
            response.setContentLength(data.getBody().size());
            response.setResponseBody(data.getBody());
            response.buildResponse(OK);
        }
        else
        {
            response.setHeader("Location:", data.getLocation());
            response.buildResponse(TEMPORARY_REDIRECT);
        }
    } else {
        response.buildResponse(config, location, FORBIDDEN);
    }
}

int Response::sendResponse(int socket, Client client) {
    size_t totalSize = client.getResponseBuffer().size();

    // std::cout << "Total bytes sent: " << client.getSentOffset() << std::endl;
    // std::cout << "Response size: " << totalSize << std::endl;
    if (client.getSentOffset() < totalSize) {
        size_t sendResult = send(socket, client.getResponseBuffer().c_str() + client.getSentOffset(), sizeof(client.getResponseBuffer().c_str() + client.getSentOffset()), 0);
        if ((int)sendResult == -1) {
            std::cout << "############################Error sending data\n";
            return -1;
        }
        client.incremetOffset(sendResult);
        return 1;
    }
    return 0;
}

std::string Response::getContentType() {
    return (contentType);
}

unsigned int Response::getContentLength() {
    return (contentLength);
}

std::string Response::getStatus() {
    return (status);
}

std::string Response::getResponseEntity() {
    return (responseEntity);
}

// int Response::getSocket() {
//     return (socket);
// }

// int Response::getState() {
//     return (state);
// }


// int Response::getFileOffset() {
//     return (offset);
// }

// ************ SETTERS ************

void Response::setContentType(std::string fileExtension) {
    contentType = getMimeType(fileExtension.substr(fileExtension.find_last_of('.') + 1));
}

void Response::setContentLength(unsigned int length) {
    contentLength = length;
}

void Response::setResponseBody(std::string content) {
    body = content;
}

void Response::setHeader(std::string key, std::string value) {
    this->headers[key] = value;
}

// void Response::setSocket(int socket) {
//     this->socket = socket;
// }

// void Response::setState(int state) {
//     this->state = state;
// }

// void Response::setFileOffset(int offset) {
//     this->offset = offset;
// }

void Response::setStatus(unsigned int code) {
    switch(code) {
        case OK:
             status = "OK";
             break ;
        case NO_CONTENT:
            status = "No Content";
            break ;
        case PERMANENTLY_MOVED:
            status = "Permanently Moved";
            break ;
        case Found:
            status = "Found";
            break ;
        // 4XX status codes
        case TEMPORARY_REDIRECT:
            status = "Temporary Redirect";
            break ;
        case BAD_REQUEST:
            status =  "Bad Request";
            break ;
        case FORBIDDEN:
            status =  "Forbidden";
            break ;
        case NOT_FOUND:
            status =  "Not Found";
            break ;
        case METHOD_NOT_ALLOWED:
            status =  "Method Not Allowed";
            break ;
        case NOT_ACCEPTABLE:
            status =  "Not Acceptable";
            break ;
        case LENGTH_REQUIRED:
            status =  "Length Required";
            break ;
        case ENTITY_LENGTH_EXCEEDED:
            status =  "Payload Too Large";
            break ;
        case REQUEST_URI_EXCEEDED:
            status =  "URI Too Long";
            break ;
        case INTERNAL_SERVER_ERROR:
            status =  "Internal Server Error";
            break ;
        case NOT_IMPLEMENTED:
            status =  "Not Implemented";
            break ;
        case GATEWAY_TIMEOUT:
            status =  "Gateway Timeout";
            break ;
        default:
            status = "";
            break ;
    }
}

void Response::setResponseEntity(std::string response) {
    this->responseEntity = response;
}