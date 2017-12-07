//
//  Server.cpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "Server.hpp"

Server::Server(){
    _loop = new SelectLoop();
}

void Server::createConnectionListener(){
    struct sockaddr_in addr;
    _connectionListenerSD = socket(AF_INET, SOCK_STREAM, 0);   //return fd;
    if (_connectionListenerSD < 0)
    {
        Logger::Error("Not received listening socket\n");
        return;
    }
    int enable = 1;
    //reuse of local address for function bind()
    if (setsockopt(_connectionListenerSD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        Logger::Error("Failed to set REUSEADDR\n");
        close(_connectionListenerSD);
        _connectionListenerSD = -1;
        return;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(_connectionListenerSD, (sockaddr *)&addr, sizeof(addr)))
    {
        Logger::Error("Didn't binded to address\n");
        close(_connectionListenerSD);
        _connectionListenerSD = -1;
        return;
    }
    listen(_connectionListenerSD, 3);     //check return value
    _loop->setListenerSD(_connectionListenerSD);
}

void Server::deleteConnectionListener() {
    _loop->setListenerSD(-1);             // !!! check it
    close(_connectionListenerSD);
    _connectionListenerSD = -1;
}

void Server::run(){
    Logger::Info("Starting...\n");
    createConnectionListener();
    
    _loop->run();
    
    deleteConnectionListener();
}
