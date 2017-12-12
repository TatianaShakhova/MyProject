//
//  Client.cpp
//  Client-Server Application
//
//  Created by merateam on 08/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "Client.hpp"
#include <sys/socket.h>
#include "Logger.hpp"
#include <string>
#include <sstream>

Client::Client(int fd){
    _clientFD = fd;
}

void Client::setRoom(Room* room){
    _room = room;
}

bool Client::send(std::string str){
    int bytes_write = ::send(_clientFD, str.data(), str.length(), 0);
    
    std::stringstream ss;
    ss << "Bytes sent: : " << bytes_write << std::endl;
    
    Logger::Info(ss.str());
    if (bytes_write <= 0)
    {
        Logger::Info("Didn't anable to send\n");
        return false;
    }
    else
    {
        Logger::Info("Sending message\n");
        return true;
    }
}

std::string Client::getName(){
    return _name;
}

void Client::setName(std::string name){
    _name = name;
}

int Client::getFD()
{
    return _clientFD;
}
