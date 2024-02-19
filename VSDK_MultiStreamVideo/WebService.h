#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <string>

std::string GetSignatureFromWebService(std::string urlstr, std::string sessionName, std::string rolestr);

#endif