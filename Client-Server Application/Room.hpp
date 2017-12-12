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
#include <string>

class Client;

class Room{
public:
    Room();
    ~Room();
    bool roomIsEmpty();
    void addToClientList(Client* client);
private:
    std::list<Client*> _clientsInRoom;
};

#endif /* Room_hpp */
