//
//  Server.cpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//
#include "Server.hpp"
#include <regex>

bool Server::parseCommand(Client* client, const std::string &msg){
    // if msg begins from @setname {
    std::regex setNameCommand("^@([a-z]+)[ ]+(.*)");
    std::smatch matches;
    if(std::regex_search(msg, matches, setNameCommand))
    {
        Logger::Info("It is command\n");
        for (size_t i = 0; i < matches.size(); ++i)
        {
            std::cout << i << matches[i] << std::endl;
        }
    }
    else{
        Logger::Error("Try again");//c>set name
    // }
    }
    return false;
}

void Server::sendMessage(Client* client, const std::string& msg) {
    if(client)
    {
        if(client->send(msg)) {
            // success
        }
        else {
            // error
            onClientDisconnected(client->getFD());
        }
    }
    else{
        // error
    }
}
//{
    /*std::string msgForSend = msg;
    std::string clientName = std::to_string(from);
    time_t tt;
    time(&tt);
    char bufTime[80];
    struct tm *timeInfo;
    timeInfo = localtime(&tt);
    if (strftime(bufTime, 80, "%R", timeInfo))
    {
        msgForSend = clientName + " " + std::string(bufTime) + " " + msgForSend;
    }*/
    /*Message message(from, msg);
    std::string msgForSend = message.getMessage();
    //printf("Message size %d\n", msgForSend.length());
    for (std::list<int>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        if (*iter != from)
        {            
            int bytes_write = send(*iter, msgForSend.data(), msgForSend.length(), 0);
            printf("Bytes sent: %d", bytes_write);
            if (bytes_write <= 0)
            {
                Logger::Info("Didn't anable to send\n");
                continue;
            }
            else
            {
                Logger::Info("Sending message\n");
            }
        }
    }
}*/

void Server::onClientConnected(int sock){
    Logger::Error("Client connected to server\n");
    
    Client* client = new Client(sock);
    _clientsMap[sock] = client;
    _loop.addClientSocket(sock);
    
    client->send("Please, send your nameby command '@setname <name>'");
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
        if(!parseCommand(_clientsMap[sockFrom], msg)){
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
                        if((*iterator).first != sockFrom)
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
                            (*iterator).second->send(msg);
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
