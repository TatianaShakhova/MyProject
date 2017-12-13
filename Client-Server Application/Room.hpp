//
//  Room.hpp
//  Client-Server Application
//
//  Created by merateam on 12/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef Room_hpp
#define Room_hpp

#include <stdio.h>
#include <list>
#include <vector>
#include <string>
#include "Logger.hpp"

class Client;

class Room{
public:
    Room(std::string name);
    bool roomIsEmpty();
    void addToClientList(Client* client);
    void removeFromClientList(Client* client);
    const std::list<Client*>& getClientList();
    std::string getRoomName();
    void addMsgToHistory(std::string msg);
    const std::vector<std::string>& getHistory();
    std::string getRoomMates();
private:
    std::string _nameRoom;
    std::list<Client*> _clientsInRoom;
    std::vector<std::string> _history;
};

#endif /* Room_hpp */
