//
//  SelectLoopListener.h
//  Client-Server Application
//
//  Created by merateam on 07/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef SelectLoopListener_h
#define SelectLoopListener_h

#include <string>

class SelectLoopListener() {
public:
    virtual void OnClientConnected() = 0;
    virtual void OnClientDisconnected() = 0;
    virtual void OnMessageReceived(std::string str) = 0;
}


#endif /* SelectLoopListener_h */
