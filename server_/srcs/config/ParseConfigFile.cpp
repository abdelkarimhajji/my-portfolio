/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahajji <ahajji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:55:15 by ahajji            #+#    #+#             */
/*   Updated: 2024/05/01 17:20:52 by ahajji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfigFile.hpp"
#include "DataConfig.hpp"

void    ParseConfigFile::errorParse()
{
    std::cout << "error in parse" << std::endl;
    exit(1);
}

ParseConfigFile::ParseConfigFile()
{
    this->findBraciteRight = 0;
    this->findBraciteLeft = 0;
    this->findBraciteRightLocation = 0;
    this->findBraciteLeftLocation = 0;
}

bool ParseConfigFile::isDirectory(const std::string& path) {
    struct stat info;

    if(stat(path.c_str(), &info) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

std::vector<std::string>     ParseConfigFile::split(std::string str)
{
    std::vector<std::string> allElments;
    std::string part, part2;
    std::stringstream convert(str);
    while (std::getline(convert, part, ' '))
    {
        std::stringstream convert2(part);
        while (std::getline(convert2, part2, '\t'))
        {
            allElments.push_back(part2);
        }
    }
    return allElments;
}

void    ParseConfigFile::checkValidServer(std::vector<std::string> splitVector)
{
    if ((splitVector[1] == "{" && splitVector.size() == 2)
        || (splitVector.size() == 1))
    {
            this->findBraciteRight = 1;
            this->findBraciteLeftLocation = 0;
            this->findBraciteRightLocation = 0;
            DataConfig newConfig;
            data.push_back(newConfig);
    }
    else
    {
        std::cout << "i check server" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidListen(std::vector<std::string> splitVector)
{
    size_t i = 0;
    if(splitVector.size() == 2)
    {
        while(i < splitVector[1].size())
        {
            if(!std::isdigit(splitVector[1][i]))
                errorParse();
            i++;
        }
        this->data.back().setListen(splitVector[1]);
    }   
    else
    {
        std::cout << "i check listen" << std::endl;
        errorParse();
    }
        
}
void    ParseConfigFile::checkValidServerName(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        this->data.back().setServerName(splitVector[1]);
    }
    else
    {
        std::cout << "i check servername" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidRoot(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        this->data.back().setRoot(splitVector[1]);
    }
    else
    {
        std::cout << "i check root" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidIndex(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        this->data.back().setIndex(splitVector[1]);
    }
    else
    {
        std::cout << "i error validIndex" << std::endl;
        errorParse();
    }
}
void    ParseConfigFile::checkValidErrorPage(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 3)
    {
        this->data.back().setErrorPage(splitVector[1], splitVector[2]);
    }
    else
    {
        std::cout << "i error page" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocation(std::vector<std::string> splitVector)
{
    if(this->findBraciteLeftLocation == 1 && this->findBraciteRightLocation == 1)
    {
        this->findBraciteLeftLocation = 0;
        this->findBraciteRightLocation = 0;
    }
    if(splitVector.size() == 3 && splitVector[2] == "{")
    {
        
        this->data.back().setLocation(splitVector[1]);
        this->findBraciteRightLocation = 1;
    }
    else
    {
        std::cout << "location" << std::endl;
        errorParse();
    }

}
void    ParseConfigFile::checkValidLocationRoot(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        this->data.back().setLocationRoot(splitVector[1]);
    }
    else
    {
        std::cout << "i check location root" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationAlias(std::vector<std::string> splitVector)
{

    if(splitVector.size() == 2)
    {
        this->data.back().setLocationAlias(splitVector[1]);
    }
    else
    {
        std::cout << "i check location alias" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationIndex(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        this->data.back().setLocationIndex(splitVector[1]);
    }
    else
    {
        std::cout << "i check location index" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationMethods(std::vector<std::string> splitVector)
{
    if(splitVector.size() >= 2 && splitVector.size() <= 4)
    {
        if(splitVector.size() == 2)
            this->data.back().setLocationMethods(splitVector[1], "", "");
        if(splitVector.size() == 3)
            this->data.back().setLocationMethods(splitVector[1], splitVector[2], "");
        if(splitVector.size() == 4)
            this->data.back().setLocationMethods(splitVector[1], splitVector[2], splitVector[3]);
    }
    else
    {
        std::cout << "i check loaction methods" << std::endl;
        errorParse();
    }
}
void    ParseConfigFile::checkValidLocationAutoIndex(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
        this->data.back().setLocationAutoIndex(splitVector[1]);
    else
    {
        std::cout << "i check autoInedx" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationCgiExtention(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        if(splitVector[1] == ".php")
            this->data.back().setLocationCgiExtention(splitVector[1]);
        else errorParse();
    }
    else
    {
        std::cout << "i check cgiextention" << std::endl;
        errorParse();
    }
}
void    ParseConfigFile::checkValidLocationCgiBin(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
        this->data.back().setLocationCgiBin(splitVector[1]);
    else
    {
        std::cout << "i check cgiBIn" << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationReturn(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 3)
        this->data.back().setLocationReturn(splitVector[1], splitVector[2]);
    else
    {
        std::cout << "i check return " << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationLeftBrackite(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 1)
    {
        this->findBraciteLeftLocation = 1;
    }
    else
        errorParse();
}

void    ParseConfigFile::checkValidServerLeftBrackite(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 1)
    {
        this->findBraciteRight = 0;
        this->findBraciteLeft = 0;
    }
    else
        errorParse();
}
void    ParseConfigFile::checkValidHost(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
        this->data.back().setHost(splitVector[1]);
    else
    {
        std::cout << "i check host " << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidAutoIndex(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        if(splitVector[1] == "off")
            this->data.back().setAutoIndex(0);
        else if(splitVector[1] == "on")
            this->data.back().setAutoIndex(1);
        else
            errorParse();
    }
    else
    {
        std::cout << "i check autoIndex " << std::endl;
        errorParse();
    }
}
void    ParseConfigFile::checkValidLocationUpload(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        if(isDirectory(splitVector[1]) == false)
            errorParse();
        this->data.back().setLocationUpload(splitVector[1]);
    }
    else
    {
        std::cout << "i check upload " << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationCgiTime(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        int i = 0;
        while (splitVector[1][i])
        {
            if(std::isdigit(splitVector[1][i]) == 0)
                errorParse();
            i++;
        }
        this->data.back().setLocationCgiTime(std::atoi(splitVector[1].c_str()));
    }
    else
    {
        std::cout << "i check autoIndex " << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationErrorPage(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 3)
    {
        this->data.back().setLocationErrorPage(splitVector[1], splitVector[2]);
    }
    else
    {
        std::cout << "i check errr page " << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::checkValidLocationMaxSize(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        int i = 0;
        while (splitVector[1][i])
        {
            if(std::isdigit(splitVector[1][i]) == 0)
                errorParse();
            i++;
        }
        long long size = std::stoll(splitVector[1]);
        if(size > 1000000000)
            errorParse();
        this->data.back().setLocationMaxSize(size);
    }
    else
    {
        std::cout << "i check max size " << std::endl;
        errorParse();
    }
}
void    ParseConfigFile::checkValidMaxSize(std::vector<std::string> splitVector)
{
    if(splitVector.size() == 2)
    {
        int i = 0;
        while (splitVector[1][i])
        {
            if(std::isdigit(splitVector[1][i]) == 0)
                errorParse();
            i++;
        }
        long long size = std::stoll(splitVector[1]);
        if(size > 1000000000)
            errorParse();
        this->data.back().setSizeMax(size);
    }
    else
    {
        std::cout << "i check autoIndex " << std::endl;
        errorParse();
    }
}

void    ParseConfigFile::parser(std::string nameFile)
{
    size_t i = 0;
    
    std::string line;
    std::vector<std::string> myVector_s;
    std::vector<std::string> splitVector;
    std::ifstream configFile(nameFile);
    std::string::size_type lastPeriodPos = nameFile.rfind('.');
    std::cout << lastPeriodPos << "\n";
    if(lastPeriodPos == std::string::npos)
        errorParse();
    else
    {
        std::string afterDot =  nameFile.substr(lastPeriodPos+1);
        if(afterDot != "conf")
            errorParse();
    }
    if(configFile.is_open())
    {        
        while (std::getline(configFile, line))
        {
            myVector_s.push_back(line);
        }
        // std::cout << myVector_s.back() << std::endl;
       
            i = 0;
            while (i < myVector_s.size())
            {
                splitVector = split(myVector_s[i]);
                if(!splitVector.empty() && splitVector[0] != "#" && splitVector[0][0] != '#')
                {
                    if((splitVector[0] == "server" || splitVector[0] == "server{")
                        && ((this->findBraciteRight == 0 && this->findBraciteLeft == 0)
                        || (this->findBraciteRight == 1 && this->findBraciteLeft == 1)))
                        {    
                            checkValidServer(splitVector);
                        }
                    else if(splitVector[0] == "listen" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidListen(splitVector);
                    else if(splitVector[0] == "server_name" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidServerName(splitVector);
                    else if(splitVector[0] == "root" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                            checkValidRoot(splitVector);
                    else if(splitVector[0] == "index" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidIndex(splitVector);
                    else if(splitVector[0] == "host" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidHost(splitVector);
                    else if(splitVector[0] == "autoindex" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidAutoIndex(splitVector);
                    else if(splitVector[0] == "error_page" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidErrorPage(splitVector);
                    else if(splitVector[0] == "max_body_size" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 0)
                        checkValidMaxSize(splitVector);
                    else if(splitVector[0] == "location" && this->findBraciteRight == 1)
                        checkValidLocation(splitVector);
                    else if(splitVector[0] == "root" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationRoot(splitVector);
                    else if(splitVector[0] == "alias" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationAlias(splitVector);
                    else if(splitVector[0] == "index" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationIndex(splitVector);
                    else if(splitVector[0] == "methods" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationMethods(splitVector);
                    else if(splitVector[0] == "autoindex" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationAutoIndex(splitVector);
                    else if(splitVector[0] == "cgi_extention" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationCgiExtention(splitVector);
                    else if(splitVector[0] == "cgi_bin" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationCgiBin(splitVector);
                    else if(splitVector[0] == "cgi_time" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationCgiTime(splitVector);
                    else if(splitVector[0] == "upload" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationUpload(splitVector);
                    else if(splitVector[0] == "return" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1){
                        checkValidLocationReturn(splitVector);
                        }
                    else if(splitVector[0] == "error_page" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationErrorPage(splitVector);
                    else if(splitVector[0] == "max_body_size" && this->findBraciteRight == 1
                        && this->findBraciteRightLocation == 1)
                        checkValidLocationMaxSize(splitVector);
                    else if(splitVector[0] == "}" && this->findBraciteRight == 1 
                        && this->findBraciteRightLocation == 1 && this->findBraciteLeftLocation == 0)
                        {
                            checkValidLocationLeftBrackite(splitVector);
                        }
                    else if(splitVector[0] == "}" && this->findBraciteRight == 1 && this->findBraciteLeft == 0)
                    {
                        checkValidServerLeftBrackite(splitVector);
                    }
                    else
                    {
                        std::cout << "i am here \n";
                        // std::cout << myVector_s[i] <<  std::endl;
                        // std::cout << this->findBraciteLeftLocation << std::endl;  
                        // std::cout << this->findBraciteRightLocation << std::endl; 
                        // std::cout << this->findBraciteLeft << std::endl;  
                        // std::cout << this->findBraciteRight << std::endl;                        
                        errorParse(); 
                    }
                }
                
                i++;
            }
            if(this->findBraciteLeft != 0 || this->findBraciteRight != 0)
                errorParse();
            
            // std::cout << "hiii "<< this->data[0].getLocation()[0]._return.path;
            // std::cout << this->data[0].getRoot() << std::endl;
            // exit(0); 
    }
    else
    {
        std::cout << "error happen with this file" << std::endl;
        exit(1);
    }
}
std::vector<DataConfig>  ParseConfigFile::getData()
{
    return this->data;
}