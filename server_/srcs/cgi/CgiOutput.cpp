/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiOutput.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:36:04 by ahajji            #+#    #+#             */
/*   Updated: 2024/04/01 21:50:45 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiOutput.hpp"

CgiOutput::CgiOutput()
{
    this->body = "";
    this->location = "";   
}

std::string CgiOutput::getBody()
{
    return this->body;
}

std::string CgiOutput::getLocation()
{
    return this->location;
}
void CgiOutput::setBody(std::string body)
{
    this->body = body;
}

void CgiOutput::setLocation(std::string location)
{
    this->location = location;
}

std::string CgiOutput::getCgiError()
{
    return this->cgiError;
}

void CgiOutput::setCgiError(std::string cgiError)
{
    this->cgiError = cgiError;
}