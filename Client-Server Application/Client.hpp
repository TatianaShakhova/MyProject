//
//  Client.hpp
//  Client-Server Application
//
//  Created by merateam on 08/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <string>
#include <map>

class Client{
public:
    Client(int fd);
    bool send(std::string str);
    void setName(std::string name);
    std::string getName();
    int getFD();
private:
    int _clientFD = -1;
    std::string _name = "";
};

#endif /* Client_hpp */
