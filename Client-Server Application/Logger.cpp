//
//  Logger.cpp
//  Client-Server Application
//
//  Created by merateam on 05/12/2017.
//  Copyright © 2017 merateam. All rights reserved.
//

#include "Logger.hpp"
#include <fstream>
#include <iostream>

using namespace std;

//FILE* f = null;

void Logger::Error(string msg) {
    print("ERROR", msg);
};

void Logger::Info(string msg) {
    print("INFO", msg);
};

void Logger::print(string tag, string msg) {
    
    cout << tag << ": " << msg;
    char fileName[100] = "/Users/merateam/Documents/MyProject-master/Client-Server Application/LogFile.txt";
    
    ofstream fout;
    fout.open(fileName, ios_base::app);
    if (fout.is_open())
    {
        fout << tag << ": " << msg << endl;
        fout.close();
    }
    else
    {
        cout << "Error opening the log file\n";
    }
}
