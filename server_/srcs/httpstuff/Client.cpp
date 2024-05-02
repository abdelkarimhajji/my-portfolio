#include "Client.hpp"

Client::Client()
{
    this->sentOffset = 0;
    served = 0;
    // std::cout << "client created\n";
}
void Client::setRequest(const std::string& req)
{
    this->requestBuffer = req;
}
void Client::setResponse(const std::string& res)
{
    this->responseBuffer = res;
}

void Client::incremetOffset(size_t n)
{
    this->sentOffset += n;
}

void Client::setOffset(size_t n)
{
    this->sentOffset = n;
}

size_t Client::getSentOffset()
{
    return (sentOffset);
}

std::string& Client::getRequestBuffer()
{
    return requestBuffer;
}
std::string& Client::getResponseBuffer()
{
    return responseBuffer;
}
