//
//  Server.cpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//
#include "Server.hpp"
#include <regex>

void Server::printHistory(Client* client){
    const std::vector<std::string>& history = (client->getRoom())->getHistory();
    if (history.size() != 0)
    {
        std::string messages = "Message history\n";
        for (auto iterator = history.begin(); iterator != history.end(); iterator++)
        {
            messages += (*iterator) + "\n";
        }
        sendMessage(client, messages);
    }
    else
    {
        sendMessage(client, "Message history is clean");
    }
}

void Server::sendAllClientsToList(Client* client, std::string msg){
    const std::list<Client*>& clientListInRoom = client->getRoom()->getClientList();
    time_t tt;
    time(&tt);
    char bufTime[80];
    struct tm *timeInfo;
    timeInfo = localtime(&tt);
    strftime(bufTime, 80, "%R", timeInfo);
    std::string roomName = (client->getRoom())->getRoomName();
    for (auto iterator = clientListInRoom.begin(); iterator != clientListInRoom.end(); iterator++)
    {
        if ((*iterator) != client)
        {
            std::string msgForSend = std::string(bufTime) + " "
                    + roomName + " " + client->getName() + ": " + msg;
            sendMessage((*iterator), msgForSend);
            (client->getRoom())->addMsgToHistory(msgForSend);
        }
    }
}

bool Server::processSetNameComand(std::string name, Client* client){
    Logger::Info("It is command @setname\n");
    if (client->getName() == "")
    {
        sendMessage(client, "You are authorized");
        client->setName(name);
        sendRoomsToClient(client);
        return true;
    }
    bool find = false;
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
    else
    {
        sendMessage(client, "You are changed your name");
        client->setName(name);
        return true;
    }
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
                if (client->getRoom() && ((client->getRoom())->getRoomMates()).size() > 1)
                {
                    sendAllClientsToList(client, "Client " + client->getName()
                                         + " changed his name to " + std::string(matches[2]));
                }
                return processSetNameComand(matches[2], client);
            }
            if (matches[1] == "@joinroom")
            {
                Logger::Info("It is command @joinroom\n");
                if (_roomsMap.find(matches[2]) != _roomsMap.end())
                {
                    _roomsMap[matches[2]]->addToClientList(client);
                    client->setRoom(nullptr);
                    client->setRoom(_roomsMap[matches[2]]);
                    std::string info = "You joind the room " + (_roomsMap[matches[2]]->getRoomName());
                    sendMessage(client, info);
                    sendAllClientsToList(client, "Client " + client->getName() + " joined the room");
                    return true;
                }
                else
                {
                    sendMessage(client, "No such room. Join or create new one\n");
                    sendRoomsToClient(client);
                    return true;
                }
            }
            if (matches[1] == "@setroom")
            {
                if (_roomsMap.find(matches[2]) == _roomsMap.end())
                {
                    createRoom(matches[2]);
                    sendMessage(client, "You created a new room");
                    //sendRoomsToClient(client);
                    //for testing
                    _roomsMap[matches[2]]->addToClientList(client);
                    //client->setRoom(nullptr);
                    client->setRoom(_roomsMap[matches[2]]);
                    std::string info = "You joind the room " + (_roomsMap[matches[2]]->getRoomName());
                    sendMessage(client, info);
                    return true;
                }
                else
                {
                    sendMessage(client, "Group with such a name already exists");
                    sendRoomsToClient(client);
                    return true;
                }
            }
            if (matches[1] == "@leaveroom")
            {
                (client->getRoom())->removeFromClientList(client);
                client->setRoom(nullptr);
                sendMessage(client, "You have leaved the room");
                sendRoomsToClient(client);
                sendAllClientsToList(client, "Client " + client->getName() + " leaved the room");
                return true;
            }
            if (matches[1] == "@delroom")
            {
                const std::string& roomName = matches[2];
                if ((client->getRoom())->getRoomName() == roomName)
                {
                    sendMessage(client, "Room " + roomName + " can not be deleted");
                    return true;
                }
                if (_roomsMap.find(roomName) != _roomsMap.end())
                {
                    if (deleteRoom(roomName))
                    {
                        sendMessage(client, "Room " + roomName + " was deleted");
                        return true;
                    }
                }
                else
                {
                    sendMessage(client, "No such room");
                    return true;
                }
            }
            if (matches[1] == "@listroom")
            {
                sendRoomsToClient(client);
                return true;
            }
            if (matches[1] == "@roommates")
            {
                if (_roomsMap.find(matches[2]) != _roomsMap.end())
                {
                    sendMessage(client, _roomsMap[matches[2]]->getRoomMates());
                    return true;
                }
                else
                {
                    sendMessage(client, "Room not found");
                    return true;
                }
            }
            if (matches[1] == "@history")
            {
                sendMessage(client, "Show message history '@history room'");
                printHistory(client);
                return true;
            }
            else
            {
                Logger::Error("Try again\n");//set name
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
        sendMessage(client, "Create a room by command '@setroom <room>'");
    }
    else
    {
        sendMessage(client, "Now there are no rooms. Create a room for chart.\n");
        sendMessage(client, "Create a room by command '@setroom <room>'");
    }
}

void Server::createRoom(std::string nameOfRoom){
    Logger::Info("New room created\n");
    Room* room = new Room(nameOfRoom);
    _roomsMap[nameOfRoom] = room;
}

bool Server::deleteRoom(std::string nameOfRoom){
    bool bResult = false;
    
    if (_roomsMap[nameOfRoom] && _roomsMap[nameOfRoom]->roomIsEmpty())
    {
        _roomsMap.erase(nameOfRoom);
        Logger::Info("Room deleted\n");
        bResult = true;
    }
    
    return bResult;
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
    sendMessage(client, "Please, send your name by command '@setname <name>'");
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
                    if (_clientsMap[sockFrom]->getRoom())
                    {
                        sendAllClientsToList(_clientsMap[sockFrom], msg);
                    }
                    else
                    {
                        sendRoomsToClient(_clientsMap[sockFrom]);
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
