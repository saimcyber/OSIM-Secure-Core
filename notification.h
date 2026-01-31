#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <fstream>
#include <iostream>
#include <ctime>
#include "string_utils.h"
using namespace std;

class NotificationManager {
public:
    static NotificationManager& getInstance();
    void broadcast(const char* sender, const char* message);
    void displayAllFor(const char* username);
    bool deleteEntry(int lineNumber);
    static void getCurrentTimestamp(char* buffer, int size);

private:
    NotificationManager();
    void persist(const char* line);
    
};

#endif // NOTIFICATION_H
