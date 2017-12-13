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
#include <iostream>
#include <list>
#include <time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <algorithm>
#include <iostream>
#include "Logger.hpp"
#include "SelectLoopListener.h"

class SelectLoop {
public:
    void run();
    void setListenerSD(int listeningSocket);   
    void setListener(SelectLoopListener* listener);
    void addClientSocket(int sock);    //_clientsSD.add
    void removeClientSocket(int sock); //_clientsSD.remove

private:
    int _listener = -1;
    SelectLoopListener* _server = nullptr;
    std::list<int> _clientsSD;
};

#endif /* SelectLoop_hpp */
