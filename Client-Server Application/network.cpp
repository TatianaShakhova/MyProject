//
//  network.cpp
//  Client-Server Application
//
//  Created by merateam on 05/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

/*#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>

class Server{
public:
    void notify();
private:
    Server();
    ~Server();
    int listener;
    struct sockaddr_in addr;
    
};

Server::Server(){
    listener = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(3425);
    if (listener < 0)
    {
        perror("socket failed");
    }
    if (bind(listener, (sockaddr*)&addr, sizeof(addr) < 0))
    {
        perror("not bind");
    }
    listen(listener, 5);
}


class Client{
public:
    int sendMsg();
    struct sockaddr_in addr;
private:
    int clientSocket;
    char name[];
};

class Connection{
    //connect(clientSocket, );
};

//commands of server
class Manager{
public:
    void getListOfRooms();
    void createRoom();
    void deleteRoom();
    void goIntoTheRoom(int name);
    void escapeFrom();
    int  listOfMembersInRoom();
    void messageHistory();
private:
};

class Room{
public:
    Room(std::string name);
    ~Room();
    //void addClient();
    //void deleteClient();
    //int getNumberOfClients();
    
private:
    //list of clients and msgs
    std::string roomName;
    //int numberOfClients;
    //list<Message> history;
};

class Message{
    //int time;
    //string from, text, room;
};*/
/*int len = 1024;
int sendAll(int sd, char *buf, int len, int flags){
    int total = 0;
    int bytes_send = 0;
    while (total < len)
    {
        bytes_send = send(sd, buf + total, len - total, 0);
        if (bytes_send == -1)
        {
            Logger::Error("Error\n");
            return;
        }
            total += bytes_send;
        }
        return (bytes_send == -1 ? -1 : total);
    }
}

int recvAll(int sd, char *buf, int len, int flags){
    int total = 0;
    int bytes_recv = 0;
    while (total < len)
    {
        bytes_read = recv(sd, buf + total, len - total, 0);
        if (bytes_read == -1)
        {
            Logger::Error("Error\n");
            return;
        }
            total += bytes_read;
        }
        return (bytes_read == -1 ? -1 : total);
    }
}*/
