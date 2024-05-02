#include "RequestMethods.hpp"


int deleteDir( DataConfig config, std::string path) {
    DIR *dir = opendir(path.c_str());
    if (!dir) {
        return (NOT_FOUND);
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            std::remove((path + entry->d_name).c_str());
        }
    }
    config.getRoot();
    return (NO_CONTENT);
}

Response RequestMethod::DELETE(Request &request, DataConfig config) {
    Response response;
    std::string requestedRessource = request.getRequestRessource();
    if (requestedRessource[requestedRessource.size() - 1] == '/') {
        // check if location has cgi first
        unsigned int code = deleteDir(config, request.getPath());
        if (code != NO_CONTENT) {
            response.buildResponse(config, request.getLocation(), code);
        } else {
            response.buildResponse(deleteDir(config, request.getPath()));
        }
    } else {
        if (std::remove(request.getPath().c_str()) != 0) {
            response.buildResponse(config, request.getLocation(), NOT_FOUND);
        } else {
            response.buildResponse(NO_CONTENT);
        }
    }
    return (response);
}