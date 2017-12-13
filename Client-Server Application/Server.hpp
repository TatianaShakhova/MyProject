//
//  Server.hpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include <map>
#include <iostream>
#include "SelectLoop.hpp"
#include "SelectLoopListener.h"
#include "Client.hpp"
#include "Room.hpp"

class Server : public SelectLoopListener{
public:
    void run();
    void createConnectionListener();
    void deleteConnectionListener();
    void onClientConnected(int sock) override;
    void onClientDisconnected(int sock) override;
    void onMessageReceived(int sockFrom, const std::string& msg) override;
    void sendMessage(Client* client, const std::string& msg);
    bool parseCommand(Client* client, const std::string& msg);
    void createRoom(std::string nameOfRoom);
    bool deleteRoom(std::string nameOfRoom);
    void sendRoomsToClient(Client* client);
    void printHistory(Client* client);
private:
    SelectLoop _loop;
    int _connectionListenerSD;
    bool processSetNameComand(std::string name, Client* client);
    void sendAllClientsToList(Client* client, std::string msg);
    std::map<int, Client*> _clientsMap;
    std::map<std::string, Room*> _roomsMap;
};
#endif /* Server_hpp */
