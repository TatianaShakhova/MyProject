//
//  Logger.hpp
//  Client-Server Application
//
//  Created by merateam on 05/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include <string>

class Logger {
public:
    static void Error(std::string msg);
    static void Info(std::string msg);
    
private:
    static void print(std:: string tag, std::string msg);
};

#endif /* Logger_hpp */
