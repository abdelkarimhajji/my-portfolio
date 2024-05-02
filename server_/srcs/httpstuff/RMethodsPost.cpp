/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RMethodsPost.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 00:00:05 by ahajji            #+#    #+#             */
/*   Updated: 2024/04/06 01:49:31 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestMethods.hpp"

std::string getLastPart(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return "";
}

int checkUpload(Request request)
{
    if(request.getHeader("Content-Type").find("multipart/form-data; boundary=-") != std::string::npos)
    {
        std::cout << "exist ok\n";
        return 1;
    }
    return 0;
}
struct FormData {
    std::string fileName;
    std::string fileContent;
};


void    upload(Request request, DataConfig config)
{
    std::vector<FormData> formData;
    size_t startBoundary = request.getHeader("Content-Type").find("boundary=");
    if(startBoundary == std::string::npos)
        return ;
    std::string boundary = request.getHeader("Content-Type").substr(startBoundary + 9);
    size_t firstBoundary = request.getBody().find(boundary);
    while (firstBoundary != std::string::npos)
    {
        size_t pos = request.getBody().find(boundary, firstBoundary + boundary.length());
        if(pos == std::string::npos)
            break;
        FormData data;
        std::string partBoundary = request.getBody().substr(firstBoundary + boundary.length() + 6 , pos);
        size_t posNameFile =  request.getBody().find("filename=\"", firstBoundary);
        
        if (posNameFile == std::string::npos || posNameFile > pos) 
        {
            firstBoundary = pos;
            continue; 
        }
        size_t posLasNameFile =  request.getBody().find("\"", posNameFile + 10);
        std::string fileName = request.getBody().substr(posNameFile + 10, posLasNameFile - posNameFile - 10);
        
        size_t valueStart = request.getBody().find("\r\n\r\n", posLasNameFile) + 4;
        size_t valueEnd = request.getBody().find(boundary, valueStart);

        data.fileName = fileName;
        data.fileContent = request.getBody().substr(valueStart, valueEnd - valueStart);
        formData.push_back(data);
        firstBoundary = pos;
    }
    for (unsigned long i = 0; i < formData.size(); ++i) {
        std::string filePath = config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->upload + formData[i].fileName;
        std::ofstream outFile(filePath.c_str());

        if (!outFile) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            continue;
        }

        outFile << formData[i].fileContent;
        outFile.close();
    }
}

std::string listFils(const std::string& path)
{
        std::ostringstream ss;
        DIR *dir = opendir(path.c_str());
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

std::string returnContentFile(std::string path, Response& response, Request request, DataConfig config)
{
    std::ifstream file;
    file.open(path.c_str(), std::ios::binary);
    if (!file || !file.is_open()) {
        response.buildResponse(config, request.getLocation(),404);
        std::cerr << "Unable to open file\n";
        return "error file";
    }
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string fileContent(buffer.begin(), buffer.end());
    return fileContent;
}

void  returnDefaultContentFile(Request& request, DataConfig config, std::string path, Response& response)
{
    std::string line;
    std::string nameFile;
    CgiOutput  data;

    if(config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->index.empty() == 0)
        nameFile = config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->index;
    path += nameFile;
    
    std::vector<Location>::iterator it = config.getSpecificLocation(request.getLocation());
    if (it != config.getLocation().end() && config.getSpecificLocation(request.getLocation())->autoIndex == 1 && nameFile.empty())
    {
        response.setStatus(200);
        response.setContentType("index.html");
        response.setContentLength(listFils(path).size());
        response.setResponseBody(listFils(path));
        response.buildResponse(200);
        return ;
    }   
        
   if(nameFile.empty())
   {
        response.buildResponse(config, request.getLocation(),403);
            return ;
   }
    
    std::string extension = "";
    std::size_t pos = nameFile.find_last_of(".");
    if (pos != std::string::npos)
        extension = nameFile.substr(pos);
    std::string fileContent;
   
    if (extension == ".php")
    {   
        if(config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->cgiExtension != "")
        {
            data = Cgi::CallCgi(path, request, "/", config);
            if(data.getCgiError() == "error")
                response.buildResponse(config, request.getLocation(),500);
            else if(data.getCgiError() == "time out")
                response.buildResponse(config, request.getLocation(),504);
            else if(data.getLocation().empty())
            {
                response.setContentType(getLastPart(path));
                response.setContentLength(data.getBody().size());
                response.setResponseBody(data.getBody());
                response.buildResponse(200);
            }
            else
            {
                response.setHeader("Location:", data.getLocation());
                response.buildResponse(307);
            }
        }
        else
            response.buildResponse(config, request.getLocation(),403);
    }
    else
    {
        if (checkUpload(request) && config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->upload.empty() == 0)
            upload(request, config);
        fileContent = returnContentFile(path, response, request, config);
        if (fileContent == "error file")
            return ;
        response.setContentType(getLastPart(path));
        response.setContentLength(fileContent.size());
        response.setResponseBody(fileContent);
        response.buildResponse(200);
    }
}

void returnSpecificContentFile(std::string path, DataConfig config,Response& response, Request& request)
{
    CgiOutput  data;
    std::string nameFile = getLastPart(path);

    std::string extension = "";
    std::size_t pos = nameFile.find_last_of(".");
    if (pos != std::string::npos)
        extension = nameFile.substr(pos);
    std::string fileContent;
    if (extension == ".php")
    {   
        if(config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->cgiExtension != "")
        {
            data = Cgi::CallCgi(path, request, "", config);
            if(data.getCgiError() == "error")
                response.buildResponse(config, request.getLocation(),500);
            else if(data.getCgiError() == "time out")
                response.buildResponse(config, request.getLocation(),504);
            else if(data.getLocation().empty())
            {
                response.setContentType(getLastPart(path));
                response.setContentLength(data.getBody().size());
                response.setResponseBody(data.getBody());
                response.buildResponse(200);
            }
            else
            {
                response.setHeader("Location:", data.getLocation());
                response.buildResponse(307);
            }
        }
        else
           response.buildResponse(config, request.getLocation(),403);
    }
    else
    {
        if (checkUpload(request) && config.getSpecificLocation(request.getLocation().empty() ? "/" : request.getLocation())->upload.empty())
            upload(request, config);
        fileContent = returnContentFile(path, response, request, config);
        if (fileContent == "error file")
            return ;
        response.setContentType(getLastPart(path));
        response.setContentLength(fileContent.size());
        response.setResponseBody(fileContent);
        response.buildResponse(200);
    }
}
Response RequestMethod::POST(Request& request, DataConfig config)
{   
    std::string path = request.getPath();
    Response response;
    std::string content;
    std::string fileContent;
    if (path[path.size() - 1] == '/')
        returnDefaultContentFile(request, config, path, response);
    else
        returnSpecificContentFile(path, config, response, request);
    return response;
}