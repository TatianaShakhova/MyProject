//
//  Room.cpp
//  Client-Server Application
//
//  Created by merateam on 12/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "Room.hpp"
#include "Client.hpp"
#include <iostream>

Room::Room(std::string name){
    _nameRoom = name;
}

std::string Room::getRoomName(){
    return _nameRoom;
}

void Room::addMsgToHistory(std::string msg){
    _history.push_back(msg);
}

bool Room::roomIsEmpty(){
    return _clientsInRoom.empty();
}

void Room::addToClientList(Client* client){
    _clientsInRoom.push_back(client);
}

void Room::removeFromClientList(Client* client){
    for (auto iterator = _clientsInRoom.begin(); iterator != _clientsInRoom.end(); iterator++)
    {
        if (*iterator == client)
        {
            _clientsInRoom.erase(iterator);
            Logger::Info("Client removed from list\n");  
        }
    }
}

const std::list<Client*>& Room::getClientList(){
    return _clientsInRoom;
}

std::string Room::getRoomMates()
{
    std::string result;
    for (auto client : _clientsInRoom)
    {
        result += client->getName() + "\n";
    }
    return result;
}

const std::vector<std::string>& Room::getHistory(){
    return _history;
}
