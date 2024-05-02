/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nachab <nachab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 10:42:31 by ahajji            #+#    #+#             */
/*   Updated: 2024/02/23 20:35:59 by nachab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef DATA
#define DATA

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

struct ErrorPage
{
    std::string error;
    std::string page;
    ErrorPage()
    {
        error = "";
        page = "";
    }
};
struct Return
{
    std::string status;
    std::string path;
};

struct Methods
{
    int get;
    int post;
    int _delete;

    Methods()
    {
        get = 0;
        post = 0;
        _delete = 0;
    }
};

struct Location
{
    std::string location;
    std::string alias;
    std::string root;
    std::string index;
    Methods methods;
    int autoIndex;
    std::string cgiExtension;
    std::string cgiBin;
    int     cgiTime;
    Return _return;
    std::string upload;
    ErrorPage errorPage;
    long long maxSize;
    Location(){
        autoIndex = 0;
        cgiExtension = "";
        upload = "";
        location = "";
        alias = "";
        root = "";
        index = "";
        cgiTime = 3;
    }
};

class DataConfig
{
    private:
        std::vector<std::string> listen;
        std::vector<std::string> serverName;
        std::string root;
        std::string index;
        std::string host;
        int autoIndex;
        long long maxSize;
        std::vector<ErrorPage> errorPage;
        std::vector<Location> vectorLocation;

    public:
        DataConfig();
        std::vector<std::string>    getListen();
        void    setListen(std::string listen);
        std::vector<std::string>    getServerName();
        void    setServerName(std::string serverName);
        std::string    getRoot();
        void    setRoot(std::string root);
        std::string    getIndex();
        void    setIndex(std::string index);
        std::vector<ErrorPage>    getErrorPage();
        void    setErrorPage(std::string error, std::string page);
        std::string    getHost();
        void    setHost(std::string host);
        int    getAutoIndex();
        void    setAutoIndex(int autoIndex);
        long long    getSizeMax();
        void    setSizeMax(long long maxSize);
        std::vector<Location>&    getLocation();
        std::vector<Location>::iterator getSpecificLocation(std::string location);
        void    setLocation(std::string _location);
        void    setLocationRoot(std::string root);
        void    setLocationAlias(std::string alias);
        void    setLocationIndex(std::string index);
        void    setLocationMethods(std::string method1, std::string method2, std::string method3);
        void    setLocationAutoIndex(std::string autoIndex);
        void    setLocationCgiExtention(std::string cgiExtention);
        void    setLocationCgiBin(std::string cgiBin);
        void    setLocationReturn(std::string returnStatus, std::string returnPath);
        void    setLocationUpload(std::string upload);
        void    setLocationErrorPage(std::string error, std::string page);
        void    setLocationCgiTime(int cgiTime);
        void    setLocationMaxSize(long long maxSize);
        void    errorData();

        void    printDataConfig(); // just for printing the values of attributs
};

#endif