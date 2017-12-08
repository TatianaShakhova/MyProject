//
//  SelectLoop.cpp
//  Client-Server Application
//
//  Created by merateam on 07/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "SelectLoop.hpp"

void SelectLoop::addClientSocket(int sock){
    _clientsSD.push_back(sock);
}

void SelectLoop::removeClientSocket(int sock){
    //_badClietsSD
}

void SelectLoop::setListener(Server* listener){
    _server = listener;
}

void SelectLoop::setListenerSD(int listeningSocket){
    _listener = listeningSocket;
}

void SelectLoop::run(){

    if (_listener == -1) {
        Logger::Error("Listening socket is not instaled\n");
        return;
    }
    
    char buf[1024];
    int bytes_read;
    std::list<int> badClientSD;
    
    while(1){
        //filling a lot of sockets
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(_listener, &readset);
        for (std::list<int>::iterator it = _clientsSD.begin(); it != _clientsSD.end(); ++it)
        {
            for (std::list<int>::iterator iter = badClientSD.begin(); iter != badClientSD.end(); ++iter)
            {
                if(*it == *iter)
                {
                    it = _clientsSD.erase(it);
                    iter = badClientSD.erase(iter);
                }
            }
            FD_SET(*it, &readset);
        }
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        //waiting for the event in one of the sockets
        int mx = std::max(_listener, *max_element(_clientsSD.begin(), _clientsSD.end()));
        if (select(mx + 1, &readset, NULL, NULL, &timeout) < 0)
        {
            Logger::Error("Select error\n");
            return;
        }
        
        //define the type of event and perform the appropriate actions
        if (FD_ISSET(_listener, &readset))
        {
            //a new connection request has been received, using accept
            int sock = accept(_listener, NULL, NULL);
            if (sock <= 0)
            {
                Logger::Error("No new connections\n");
                return;
            }
            else
            {
                Logger::Info("New connection established\n");
            }
            //addClientSocket(sock);
            if(_server)
            {
                _server->onClientConnected(sock);
            }
        }
        
        for (std::list<int>::iterator it = _clientsSD.begin(); it != _clientsSD.end(); ++it)
        {
            if (FD_ISSET(*it, &readset))
            {
                //recived data from the client, read them
                memset(buf, 0, sizeof(buf));
                bytes_read = recv(*it, buf, 1024, 0);
                if (bytes_read <= 0)
                {
                    //the connection is broken, remove the socket from the list
                    Logger::Error("Connection is broken\n");
                    badClientSD.push_back(*it);
                    close(*it);
                    //removeClientSocket(*it);
                    if(_server)
                    {
                        _server->onClientDisconnected(*it);
                    }
                }
                else
                {
                    Logger::Info("Reciving message\n");
                    //std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
                    //time_t tt = std::chrono::system_clock::to_time_t(time);
                    //std::cout << ctime(&tt);
                    time_t tt;
                    time(&tt);
                    char bufTime[80];
                    struct tm *timeInfo;
                    timeInfo = localtime(&tt);
                    strftime(bufTime, 80, "%R", timeInfo);
                    puts(bufTime);
                    if(_server)
                    {
                        _server->onMessageReceived(*it, buf);
                    }
                    /*for (std::list<int>::iterator iter = _clientsSD.begin(); iter != _clientsSD.end(); iter++)
                    {
                        if (*iter != *it)
                        {
                            size_t bufTimeLen = std::strlen(bufTime);
                            size_t msgLen = bufTimeLen + 1 + bytes_read;
                            char buffer[msgLen];
                            memcpy(buffer, bufTime, bufTimeLen);
                            buffer[bufTimeLen] = ' ';
                            memcpy(buffer + bufTimeLen + 1, buf, bytes_read);
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
                    }*/
                }
            }
        }
        
        
        
    }
}
