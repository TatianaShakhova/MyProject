//
//  Room.cpp
//  Client-Server Application
//
//  Created by merateam on 12/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "Room.hpp"

Room::Room(){
    
}

bool Room::roomIsEmpty(){
    if (_clientsInRoom.empty())
    {
        return true;
    }
    return false;
}

void Room::addToClientList(Client* client){
    _clientsInRoom.push_back(client);
}

