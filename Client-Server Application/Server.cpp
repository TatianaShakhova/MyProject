//
//  Server.cpp
//  Client-Server Application
//
//  Created by merateam on 06/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "Server.hpp"

Server::Server(){
    _loop = new SelectLoop();
}

void Server::addClient(int fd){
    Logger::Error("Client added\n");
    _clients.push_back(fd);
}

void Server::deleteClient(int fd){
    Logger::Error("Client deleted\n");
    _clients.remove(fd);
}

void Server::sendMessage(int from, char msg[1024]){
    for (std::list<int>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        if (*iter != from)
        {
            //size_t bufTimeLen = std::strlen(bufTime);
            //size_t msgLen = bufTimeLen + 1 + bytes_read;
            size_t msgLen = 1024;
            char buffer[msgLen];
            //memcpy(buffer, bufTime, bufTimeLen);
            //buffer[bufTimeLen] = ' ';
            //memcpy(buffer + bufTimeLen + 1, buf, bytes_read);
            int bytes_write = send(*iter, buffer, msgLen, 0);
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
}

void Server::onClientConnected(int sock){
    Logger::Error("Client connected to server\n");
    addClient(sock);
    _loop->addClientSocket(sock);
    // add
    //loop.add
}

void Server::onClientDisconnected(int sock){
    Logger::Error("Client disconnected from server\n");
    close(sock);
    deleteClient(sock);
    _loop->removeClientSocket(sock);
    //remove
    // loop.remove
    //close(sock)
}

void Server::onMessageReceived(int sockFrom, char msg[1024]){
    Logger::Error("New message\n");
    sendMessage(sockFrom, msg);
    
    // for all clients send()
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
    _loop->setListenerSD(_connectionListenerSD);
}

void Server::deleteConnectionListener() {
    _loop->setListenerSD(-1);             // !!! check it
    close(_connectionListenerSD);
    _connectionListenerSD = -1;
}

void Server::run(){
    Logger::Info("Starting...\n");
    createConnectionListener();
    _loop->setListener(this);
    
    _loop->run();
    
    _loop->setListener(nullptr);
    deleteConnectionListener();
}
