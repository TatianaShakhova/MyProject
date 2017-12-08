//
//  Server.hpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>
#include <time.h>
#include <sys/time.h>
#include <algorithm>
#include <iostream>
#include "Logger.hpp"
#include "SelectLoop.hpp"

class SelectLoop;

class Server {
public:
    Server();
    void run();
    void createConnectionListener();
    void deleteConnectionListener();
    void onClientConnected(int sock);
    void onClientDisconnected(int sock);
    void onMessageReceived(int sockFrom, char msg[1024]);
    void addClient(int fd);
    void deleteClient(int fd);
    void sendMessage(int from, char msg[1024]);
    
private:
    SelectLoop* _loop;
    int _connectionListenerSD;
    std::list<int> _clients;
    
};
#endif /* Server_hpp */
