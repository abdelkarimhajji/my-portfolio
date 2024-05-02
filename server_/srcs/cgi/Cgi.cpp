/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:48:53 by ahajji            #+#    #+#             */
/*   Updated: 2024/05/01 16:19:58 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

std::string Cgi::getLastPart(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return "";
}
std::string Cgi::getPath(const std::string& path) {
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        return path.substr(0, pos + 1);
    }
    return "";
}

CgiOutput Cgi::CallCgi(std::string path, Request& request, std::string check, DataConfig config)
{
    CgiOutput data;
    std::string outputFile = "/Users/ahajji/Desktop/webserv/server_/tmp/file.txt";  

    std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=" + request.getHttpVersion();
    std::string CONTENT_TYPE = "CONTENT_TYPE=" + request.getHeader("Content-Type");
    std::stringstream leng;
    leng << request.getBody().length();
    std::string CONTENT_LENGTH = "CONTENT_LENGTH=" + leng.str();
    std::string STATUS = "STATUS=200 OK";
    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200";
    std::string REQUEST_METHOD = "REQUEST_METHOD="+request.getRequestMethod();
    std::string QUERY_STRING = "QUERY_STRING=" + request.getQueryString();
    std::string DOCUMENT_ROOT;
    std::string SCRIPT_FILENAME;
    if(check == "/")
    {
        DOCUMENT_ROOT = "DOCUMENT_ROOT=" + request.getPath();
        SCRIPT_FILENAME = "SCRIPT_FILENAME=" + path;
    }
    else
    {
        DOCUMENT_ROOT = "DOCUMENT_ROOT=" + getPath(path);
        SCRIPT_FILENAME = "SCRIPT_FILENAME=" + request.getPath();
    }
    
    std::string headers[] = {
    SERVER_PROTOCOL,
    CONTENT_TYPE,
    CONTENT_LENGTH,
    STATUS,
    REDIRECT_STATUS,
    REQUEST_METHOD,
    DOCUMENT_ROOT,
    SCRIPT_FILENAME,
    QUERY_STRING
};

int numHeaders = 10;
char** arr = new char*[numHeaders + 1];
size_t i = 0;
for (i = 0; i < 10; ++i) {
    arr[i] = new char[headers[i].size() + 1];
    std::strcpy(arr[i], headers[i].c_str());
}
arr[i] = NULL;

std::time_t start = 0;
start = std::time(nullptr);
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    int out;
    out = dup(STDOUT_FILENO);

    if (pid == 0) {
    if (request.getRequestMethod() == "POST")
    {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        std::string postdata = request.getBody(); 
        write(pipefd[1], postdata.c_str(), postdata.size());
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
    }
        int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);  
        const char* argv[] = {"php-cgi" ,path.c_str(), NULL};
        
        execve("php-cgi", (char* const*)argv, arr);
        data.setCgiError("error");
        return data;
    } else {
        int status;
        
        while(1)
        {
            std::time_t end = std::time(nullptr);
            if(end - start >= config.getSpecificLocation(request.getLocation())->cgiTime + 1)
            {
                data.setCgiError("time out");
                kill(pid, SIGKILL);
                return data;
            }
            if(waitpid(pid, &status, WNOHANG) > 0)
                break;
        }
        waitpid(pid, &status, 0);
        for (size_t j = 0; j < i; ++j) {
            delete[] arr[j];
        }
        delete[] arr;
        dup2(STDOUT_FILENO, out);
        
        std::ifstream file(outputFile.c_str(), std::ios::binary);
        std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::string body;
        remove(outputFile.c_str());
        
        std::map<std::string, std::string> header_s;
        size_t separator = result.find("\r\n\r\n");
        if (separator != std::string::npos) {
            body = result.substr(separator + 4);
            std::string headers = result.substr(0, separator);
            size_t pos = 0;
            while((pos =  headers.find('\n')) != std::string::npos)
            {
                std::string line = headers.substr(0, pos);
                size_t doublePoint = line.find(": ");
                if(doublePoint != std::string::npos)
                {
                    std::string key = line.substr(0, doublePoint);
                    std::string value = line.substr(doublePoint + 2);
                    header_s[key] = value;
                }
                headers.erase(0, pos + 1);
            }
        }
        else
            body = result;
        std::map<std::string, std::string>::iterator it = header_s.find("Content-Length");
        if(it != header_s.end())
        {
            std::string strNumber = it->second;
            if(static_cast<unsigned long>(std::stoi(strNumber)) < body.length())
                body.erase(std::stoi(strNumber), body.length());
        }
        std::map<std::string, std::string>::iterator it2 = header_s.find("Location");
        if(it2 != header_s.end())
           data.setLocation(it2->second);
        data.setBody(body);
        return data;
    }
}