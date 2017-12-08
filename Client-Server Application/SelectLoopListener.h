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
    virtual void onClientConnected(int sock) = 0;
    virtual void onClientDisconnected(int sock) = 0;
    virtual void onMessageReceived(int sockFrom, char msg[1024]) = 0;
}


#endif /* SelectLoopListener_h */
