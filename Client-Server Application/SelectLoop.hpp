//
//  SelectLoop.hpp
//  Client-Server Application
//
//  Created by merateam on 07/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef SelectLoop_hpp
#define SelectLoop_hpp

#include <stdio.h>
#include <string>
#include "Server.hpp"

class SelectLoop {
public:
    SelectLoop() = default;
    void run();
    void setListenerSD(int listeningSocket);
    //void addClients();
private:
    int _listener = -1;
    std::list<int> _clients;
};

#endif /* SelectLoop_hpp */
