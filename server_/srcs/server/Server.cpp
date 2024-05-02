#include "Server.hpp"
#include "../config/DataConfig.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

void Server::createSocket(DataConfig config)
{
    std::vector<std::string> ports = config.getListen();
    size_t i;
    for (i = 0; i < ports.size(); i++)
    {
        int socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFd == -1)
        {
            // std::cout << "Failed to create socket. Exiting..." << std::endl;
            close(socketFd);
            exit(1);
        }
        if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
        {
            // std::cerr << "Error setting socket to non-blocking\n";
            close(socketFd);
            return ;
        }
        int enable = 1;
        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            std::cerr << "setsockopt(SO_REUSEADDR) failed";
        setServerSocket(socketFd);
        servers[socketFd] = config;
    }
    std::cout << "[INFO] " << i << " Sockets successfully created" << std::endl;
}

void Server::createServer(std::vector<DataConfig> config)
{
    for (size_t i = 0; i < config.size(); i++)
    {
        std::vector<std::string> ports = config[i].getListen();
        for (size_t j = 0; j < ports.size(); j++)
        {
            sockaddr_in address;
            std::memset(&address, 0, sizeof(address));
            address.sin_family = AF_INET;
            if (!strcmp(config[i].getHost().c_str(), "localhost"))
            {
                address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            }
            else
            {
                if (inet_pton(AF_INET, config[i].getHost().c_str(), &address.sin_addr) <= 0) {
                    // std::cerr << "Invalid address or address not supported\n";
                    return ;
                }
            }
            address.sin_port = htons(atoi(ports[j].c_str()));
            serverAddress.push_back(address);
        }
    }
 
    for (size_t i = 0; i < serverSockets.size(); i++)
    {
        int bindResult = bind(this->getServerSocket(i), reinterpret_cast<sockaddr*>(&(this->serverAddress[i])), sizeof(this->serverAddress[i]));
        if (bindResult == -1)
        {
            std::cout << "Failed to bind the socket to " << config[i].getHost() << ":" << ntohs(serverAddress[i].sin_port) << ". Ignoring it..." << std::endl;
            close(this->getServerSocket(i));
            this->serverSockets.erase(this->serverSockets.begin() + i);
            return ;
        }
        std::cout << "[INFO] Server created : socket successfully binded with " << config[i].getHost() << ":" << ntohs(serverAddress[i].sin_port) << std::endl;
    }
}

void Server::putServerOnListening()
{
    for (size_t i = 0; i < serverSockets.size(); i++)
    {
        int listenResult = listen(getServerSocket(i), SOMAXCONN);
        if (listenResult == -1) {
            std::cerr << "Failed to listen on socket for port " << ntohs(serverAddress[i].sin_port) << ". Exiting..." << std::endl;
            close(getServerSocket(i));
            this->serverSockets.erase(this->serverSockets.begin() + i);
            return ;
        }
        std::cout << "[INFO] Server listening on port " << ntohs(serverAddress[i].sin_port) << std::endl;
    }
}

const std::vector<int>& Server::getServerSockets()
{
    return serverSockets;
}
bool Server::isServerSocket(int socket)
{
    std::vector<int>::iterator it = std::find(serverSockets.begin(), serverSockets.end(), socket);
    if (it != serverSockets.end())
    {
        return true;
    }
    return false;
}
const std::vector<sockaddr_in>& Server::getServerAddress()
{
    return serverAddress;
}

const int& Server::getServerSocket(int index)
{
    return (this->serverSockets[index]);
}

void Server::setServerSocket(int socket)
{
    this->serverSockets.push_back(socket);
}

std::map<int, DataConfig>& Server::getServers()
{
    return (this->servers);
}

void Server::setServer(int socketFd, DataConfig config)
{
    this->servers[socketFd] = config;
}

void    Server::acceptNewConnections(std::vector<pollfd>& fds, std::vector<pollfd>& fdsTmp, std::map<int, Client>& Clients, size_t& i)
{
    if (fdsTmp[i].revents & POLLIN)
    {
        if (this->isServerSocket(fdsTmp[i].fd) == true)
        {
            int clientSocket = accept(fdsTmp[i].fd, NULL, NULL);
            if (clientSocket == -1)
            {
                // std::cerr << "Error accepting client connection\n";
                close(fdsTmp[i].fd);
            }
            else
            {
                if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) {
                    // std::cerr << "Error setting socket to non-blocking\n";
                    close(clientSocket);
                    return ;
                }
                
                std::cout << "New client connected, socket: " << clientSocket << "\n";
                Clients.insert(std::make_pair(clientSocket, Client()));
                pollfd pfd;
                pfd.fd = clientSocket;
                pfd.events = POLLIN | POLLOUT;
                fds.push_back(pfd);
                this->setServer(clientSocket, this->getServers()[fdsTmp[i].fd]);
            }
        }
    }
}


void    Server::handleClientInput(std::vector<pollfd>& fds, std::vector<pollfd>& fdsTmp, std::map<int, Client>& Clients, size_t& i)
{
    if ((fdsTmp[i].revents & POLLIN) && this->isServerSocket(fdsTmp[i].fd) == false)
    {
        int clientSocket = fdsTmp[i].fd;
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead > 0)
        {
            DataConfig configData = this->getServers()[clientSocket];
            Clients[clientSocket].getRequestBuffer().append(buffer, bytesRead);
            if (Clients[clientSocket].getRequestBuffer().find("Transfer-Encoding: chunked") != std::string::npos)
            {
                if (Clients[clientSocket].getRequestBuffer().find("\r\n0") != std::string::npos)
                {
                    this->parseChunkedRequest(Clients[clientSocket].getRequestBuffer());
                    Request req(Clients[clientSocket].getRequestBuffer());
                    Response response = req.handleRequest(configData);
                    Clients[clientSocket].setResponse(response.getResponseEntity());
                }
            }
            else
            {
                std::string request = Clients[clientSocket].getRequestBuffer();
                size_t headerEnd = request.find("\r\n\r\n");
                if (headerEnd != std::string::npos)
                {
                    std::string headers = request.substr(0, headerEnd);
                    size_t contentLengthPos = headers.find("Content-Length: ");
                    if (contentLengthPos != std::string::npos)
                    {
                        size_t contentLengthEnd = headers.find("\r\n", contentLengthPos);
                        std::string contentLengthStr = headers.substr(contentLengthPos + 16, contentLengthEnd - (contentLengthPos + 16));
                        size_t contentLength = atoi(contentLengthStr.c_str());
                        std::string body = request.substr(headerEnd + 4);
                        if (body.size() >= contentLength)
                        {
                            std::cout << "request : " << GREEN << Clients[clientSocket].getRequestBuffer() << RESET << "\n";
                            Request req(Clients[clientSocket].getRequestBuffer());
                            Response response = req.handleRequest(configData);
                            Clients[clientSocket].setResponse(response.getResponseEntity());
                            Clients[clientSocket].served = 0;

                        }
                    }
                    else
                    {
                            std::cout << "request : " << GREEN << Clients[clientSocket].getRequestBuffer() << RESET << "\n";
                        Request req(Clients[clientSocket].getRequestBuffer());
                        Response response = req.handleRequest(configData);
                        Clients[clientSocket].setResponse(response.getResponseEntity());
                        Clients[clientSocket].served = 0;
                    }
                }
            }
        } 
        else if (bytesRead == 0)
        {
            // std::cout << "Client disconnected, socket: " << clientSocket << "\n";
            close(clientSocket);
            for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end();) {
                if (it->fd == clientSocket)
                    it = fds.erase(it);
                else
                    ++it;
            }
            Clients.erase(clientSocket);
        }
        else
        {
            close(clientSocket);
            for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); )
            {
                if (it->fd == clientSocket)
                {
                    it = fds.erase(it);
                } else {
                    ++it;
                }
            }
            Clients.erase(clientSocket);
        }
    }
}

void    Server::deliverResponseToClient(std::vector<pollfd>& fds, std::vector<pollfd>& fdsTmp, std::map<int, Client>& Clients, size_t& i)
{
    if ((fdsTmp[i].revents & POLLOUT) && this->isServerSocket(fdsTmp[i].fd) == false)
        {
        int clientSocket = fdsTmp[i].fd;
        if (this->sendResponse(clientSocket, Clients[clientSocket]) == 0 && !Clients[clientSocket].served)
        {
            close(clientSocket);
            for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end();) {
                if (it->fd == clientSocket)
                {
                    it = fds.erase(it);
                } else {
                    ++it;
                }
            }
            Clients[clientSocket].getRequestBuffer().clear();
            Clients[clientSocket].getResponseBuffer().clear();
            Clients[clientSocket].setOffset(0);
            Clients[clientSocket].served = 1;
        }
    }
}

int Server::sendResponse(int socket, Client& client)
{
    size_t totalSize = client.getResponseBuffer().size();

    if (client.getSentOffset() < totalSize) {
        ssize_t sendResult = send(socket, client.getResponseBuffer().c_str() + client.getSentOffset(), totalSize - client.getSentOffset(), 0);
        if (sendResult == -1) {
            // std::cerr << "Error sending data\n";
            close(socket);
            return -1;
        }
        client.incremetOffset(sendResult);
        return 1;
    }
    return 0;
}

void Server::parseChunkedRequest(std::string& requestBuffer) {
    std::string buffer;
    size_t pos = 0;

    pos = requestBuffer.find("\r\n\r\n");
    if (pos == std::string::npos) {
        std::cerr << "Error: Couldn't find end of headers" << std::endl;
        return;
    }

    buffer += requestBuffer.substr(0, pos + 4);
    pos += 4;

    while (true)
    {
        if (pos >= requestBuffer.size())
            break;
         if (!isdigit(requestBuffer[pos]) && !(requestBuffer[pos] <= 'f' && requestBuffer[pos] >= 'a'))
            break ;
        size_t chunkSizePos = requestBuffer.find("\r\n", pos);
        if (chunkSizePos == std::string::npos) {
            std::cerr << "Error: Couldn't find chunk size" << std::endl;
            return;
        }

        int chunkSize;
        std::istringstream(requestBuffer.substr(pos, chunkSizePos - pos)) >> std::hex >> chunkSize;
        if (chunkSize <= 0) {
            break;
        }

        buffer += requestBuffer.substr(chunkSizePos + 2, chunkSize);
        pos = chunkSizePos + 2 + chunkSize;
        if (pos + 1 < requestBuffer.size() && requestBuffer[pos] == '\r' && requestBuffer[pos + 1] == '\n')
        {
            pos += 2;
        }
    }
    requestBuffer = buffer;
}