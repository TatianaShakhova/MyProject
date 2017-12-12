//
//  Server.cpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//
#include "Server.hpp"
#include <regex>

bool Server::processSetNameComand(std::string name, Client* client){
    bool find = false;
    Logger::Info("It is command @setname\n");
    for (auto iterator = _clientsMap.begin(); iterator != _clientsMap.end(); iterator++)
    {
        if ((*iterator).second->getName() == name)
        {
            find = true;
            continue;
        }
    }
    if (find)
    {
        Logger::Info("Incorrect name\n");
        sendMessage(client, "This name is already taken");
        sendMessage(client, "Please, send your name by command '@setname <name>'");
        return true;
    }

    client->setName(name);
    sendMessage(client, "You are authorized");
    sendRoomsToClient(client);
    return true;
}

bool Server::parseCommand(Client* client, const std::string &msg){
    // if msg begins from @setname
    std::regex setNameCommand("^(@[a-z]+)[ ]+(.*)");
    std::smatch matches;
    if (std::regex_search(msg, matches, setNameCommand))
    {
        for (size_t i = 0; i < matches.size(); ++i)
        {
            std::cout << i << matches[i] << std::endl;
            if (matches[1] == "@setname")
            {
                return processSetNameComand(matches[2], client);
            }
            if (matches[1] == "@joinroom")
            {
                Logger::Info("It is command @joinroom\n");
                if (_roomsMap.find(matches[2]) != _roomsMap.end())
                {
                    _roomsMap[matches[2]]->addToClientList(client);
                    client->setRoom(_roomsMap[matches[2]]);
                    //
                    return true;
                }
            }
            if (matches[1] == "@setroom")
            {
                bool find = false;
                for (auto iterator = _roomsMap.begin(); iterator != _roomsMap.end(); iterator++)
                {
                    if ((*iterator).first == matches[2])
                    {
                        find = true;
                        continue;
                    }
                }
                if (find)
                {
                    sendMessage(client, "Group with such a name already exists\n");
                    sendMessage(client, "Create a room by command '@setroom <room>'");
                    return true;
                }
                else
                {
                    createRoom(matches[2]);
                    sendMessage(client, "You created a new room");
                    sendRoomsToClient(client);
                    return true;
                }
            }
            else
            {
                Logger::Error("Try again\n");//c>set name
            }
        }
    }
    else
    {
        return false;
    }
    return false;
}

void Server::sendRoomsToClient(Client *client){
    if (!_roomsMap.empty())
    {
        std::string roomsList = "List of rooms:\n";
        for (auto iterator = _roomsMap.begin(); iterator != _roomsMap.end(); iterator++)
        {
            roomsList += (*iterator).first + "\n";
        }
        roomsList += "-------\n";
        sendMessage(client, roomsList);
        sendMessage(client, "Join the room by command '@joinroom <room>'\n");
        sendMessage(client, "Create a room by command '@setroom <room>'\n");
    }
    else
    {
        sendMessage(client, "Now there are no rooms. Create a room for chart.\n");
        sendMessage(client, "Create a room by command '@setroom <room>'\n");
    }
}

void Server::createRoom(std::string nameOfRoom){
    Logger::Info("New room created\n");
    Room* room = new Room();
    _roomsMap[nameOfRoom] = room;
}

void Server::deleteRoom(std::string nameOfRoom){
    Logger::Info("Room deleted\n");
    if (_roomsMap[nameOfRoom] && _roomsMap[nameOfRoom]->roomIsEmpty())
    {
        _roomsMap.erase(nameOfRoom);
    }
}

void Server::joinRoom(std::string nameOfRoom, Client* client){
    if(_roomsMap[nameOfRoom])
    {
        _roomsMap[nameOfRoom]->addToClientList(client);
    }
    else
    {
        Logger::Error("No such room. Join or create new one\n");
        return;
    }
}

void Server::sendMessage(Client* client, const std::string& msg) {
    if(client)
    {
        if(client->send(msg))
        {
            Logger::Info("Message sent\n");// success
        }
        else
        {
            Logger::Error("Client disconnected\n");// error
            onClientDisconnected(client->getFD());
        }
    }
    else
    {
        Logger::Error("Message not sent\n");// error
        return;
    }
}

void Server::onClientConnected(int sock){
    Logger::Error("Client connected to server\n");
    
    Client* client = new Client(sock);
    _clientsMap[sock] = client;
    _loop.addClientSocket(sock);
    sendMessage(client, "Please, send your nameby command '@setname <name>'");
}

void Server::onClientDisconnected(int sock){
    Logger::Error("Client disconnected from server\n");
    close(sock);
    _loop.removeClientSocket(sock);
    delete _clientsMap[sock];
    _clientsMap.erase(sock);
}

void Server::onMessageReceived(int sockFrom, const std::string& msg){
    Logger::Info("New message\n");
    if (_clientsMap.find(sockFrom) != _clientsMap.end())  // in the map
    {
        if(!parseCommand(_clientsMap[sockFrom], msg))
        {
            if(_clientsMap[sockFrom]) // check for null
            {
                if (_clientsMap[sockFrom]->getName().empty()) // name == empty
                {
                    sendMessage(_clientsMap[sockFrom], "Please, send your name by command '@setname <name>'");
                }
                else
                {
                    for (auto iterator = _clientsMap.begin(); iterator != _clientsMap.end(); iterator++)
                    {
                        if((*iterator).first != sockFrom && !(*iterator).second->getName().empty())
                        {
                            // msg
                            time_t tt;
                            time(&tt);
                            char bufTime[80];
                            struct tm *timeInfo;
                            timeInfo = localtime(&tt);
                            strftime(bufTime, 80, "%R", timeInfo);
                            std::string msgForSend = std::string(bufTime) + " "
                                                + _clientsMap[sockFrom]->getName() + ": " + msg;
                            sendMessage((*iterator).second, msgForSend);
                        }
                    }
                }
            }
            else
            {
                Logger::Error("No socket found");
                return;
            }
        }
    }
    else
    {
        onClientConnected(sockFrom);
    }
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
    _loop.setListenerSD(_connectionListenerSD);
}

void Server::deleteConnectionListener() {
    _loop.setListenerSD(-1);             // !!! check it
    close(_connectionListenerSD);
    _connectionListenerSD = -1;
}

void Server::run(){
    Logger::Info("Starting...\n");
    createConnectionListener();
    _loop.setListener(this);
    
    _loop.run();
    
    _loop.setListener(nullptr);
    deleteConnectionListener();
}
