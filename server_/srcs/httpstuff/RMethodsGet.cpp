#include "Request.hpp"
#include "RequestMethods.hpp"
#include "Response.hpp"
#include <fstream>
#include <sstream>
#include <vector>

std::string generateHTML(const char* path) {
    std::ostringstream ss;
    DIR *dir = opendir(path);
    if (!dir) {
        return "";
    }

    ss << "<html><head><title>Directory Listing</title><style>h1 {text-align:center;}</style></head><body><h1>Directory Listing</h1><ul><br>";

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            ss << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
        }
    }

    ss << "</ul></body></html>";
    closedir(dir);
    return ss.str();
}

void fillResponse(Response &response, std::ostringstream& ss, std::string filetype) {
    response.setContentType(filetype);
    response.setContentLength(ss.str().size());
    response.setResponseBody(ss.str());
    response.buildResponse(OK);
}

int checkCgiRequired(std::string file) {
    std::string extension = "";
    size_t pos = file.find_last_of(".");
    if (pos != std::string::npos) {
        extension = file.substr(pos);
        if (extension == ".php") {
            return (1);
        } else {
            return (0);
        }
    } else {
        return (0);
    }
}

void handleFolder(Response &response, std::vector<Location>::iterator &it, DataConfig &config, Request &request) {
    std::ostringstream ss;
    std::string path = request.getPath();
    if (it != config.getLocation().end()) {
        std::string indexFile = it->index.empty() ? config.getIndex() : it->index;
        if (checkCgiRequired(indexFile)) {
            response.buildResponseWithCgi(response, config, request, request.getPath() + indexFile);
            return ;
        }
        std::ifstream file(path + indexFile);
        if (!file.is_open() || indexFile.empty()) {
            if (it->autoIndex) {
                ss << generateHTML(path.c_str());
                fillResponse(response, ss, ".html");
            } else {
                response.buildResponse(config, request.getLocation(), FORBIDDEN);
            }
        } else {
            ss << file.rdbuf();
            fillResponse(response, ss, indexFile);
        }
    } else if (it == config.getLocation().end()) {
        std::ifstream file(path + config.getIndex());
        if (checkCgiRequired(config.getIndex())) {
            response.buildResponseWithCgi(response, config, request, request.getPath() + config.getIndex());
            return ;
        }
        if (!file.is_open() && config.getIndex().empty()) {
            if (config.getAutoIndex()) {
                ss << generateHTML(path.c_str());
                fillResponse(response, ss, ".html");
            } else {
                response.buildResponse(config, request.getLocation(), FORBIDDEN);
            }
        } else {
            ss << file.rdbuf();
            fillResponse(response, ss, config.getIndex());
        }
    }
}

void handleFile(DataConfig &config, Response &response, Request &request) {
    std::ostringstream ss;

    if (checkCgiRequired(request.getPath())) {
        response.buildResponseWithCgi(response, config, request, request.getPath());
        return ;
    }
    std::ifstream file(request.getPath());
    if (!file.is_open()) {
        response.buildResponse(config, request.getLocation(), NOT_FOUND);
    } else {
        ss << file.rdbuf();
        fillResponse(response, ss, request.getPath());
    }
}

Response buildResponseWithFile(DataConfig config, Request &request) {
    std::ostringstream ss;
    Response response;

    std::string path = request.getPath();
    std::string location = request.getLocation();
    if (path.back() == '/') {
        std::vector<Location>::iterator locationData = config.getSpecificLocation(location);
        handleFolder(response, locationData, config, request);
    } else {
        handleFile(config, response, request);
    } 
    return (response);
}


Response RequestMethod::GET(Request& request, DataConfig config) {
    Response response;
    std::string requestedRessource = request.getRequestRessource();
    if (requestedRessource[requestedRessource.size() - 1] == '/') {
        // if request wants a directory
        response = buildResponseWithFile(config, request);
    } else {
        // specific ressource is requested instead of default
        response = buildResponseWithFile(config, request);
    }
    return (response);
}
