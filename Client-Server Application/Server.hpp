//
//  Server.hpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include "SelectLoop.hpp"
#include "SelectLoopListener.h"
#include "Client.hpp"
#include <map>
#include <iostream>

class Server : public SelectLoopListener{
public:
    void run();
    void createConnectionListener();
    void deleteConnectionListener();
    void onClientConnected(int sock) override;
    void onClientDisconnected(int sock) override;
    void onMessageReceived(int sockFrom, const std::string& msg) override;
    //void addClient(int fd);
    //void deleteClient(int fd);
    void sendMessage(Client* client, const std::string& msg);
    bool parseCommand(Client* client, const std::string& msg);
    
private:
    SelectLoop _loop;
    int _connectionListenerSD;
    //std::list<int> _clients;
    
    std::map<int, Client*> _clientsMap;
    
};
#endif /* Server_hpp */
