#ifndef TASK_H
#define TASK_H
#include <iostream>

struct Task {
    char creator[50];
    char assignee[50];
    char name[100];
    char description[200];
    char deadline[30];
    char status[20];
    char priority[10];
    char creationTime[30];
    char ttl[10]; // Added TTL field (format: e.g., "3d", "24h", "60m")
};

#endif