//
//  main.cpp
//  Client-Server Application
//
//  Created by merateam on 05/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//
//#include "Interface.hpp"
#include "Server.hpp"

int main(int argc, const char * argv[]) {
    Server server;
    server.run();
    return 0;
}
