//
//  SelectLoop.hpp
//  Client-Server Application
//
//  Created by merateam on 07/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef SelectLoop_hpp
#define SelectLoop_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include "Server.hpp"

class Server;

class SelectLoop {
public:
    SelectLoop() = default;
    void run();
    void setListenerSD(int listeningSocket);   
    void setListener(Server* listener);
    
    void addClientSocket(int sock); //_clientsSD.add
    void removeClientSocket(int sock); //_clientsSD.remove

private:
    int _listener = -1;
    Server* _server = nullptr;
    std::list<int> _clientsSD;
};

#endif /* SelectLoop_hpp */
