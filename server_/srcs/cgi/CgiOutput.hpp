/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiOutput.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:36:01 by ahajji            #+#    #+#             */
/*   Updated: 2024/04/01 21:49:15 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_OUTPUT
#define CGI_OUTPUT
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

class CgiOutput
{
    private:
        std::string body;
        std::string location;
        std::string cgiError;
    public:
        CgiOutput();
        std::string getBody();
        std::string getLocation();
        std::string getCgiError();
        void setBody(std::string body);
        void setLocation(std::string location);
        void setCgiError(std::string cgiError);
};

#endif