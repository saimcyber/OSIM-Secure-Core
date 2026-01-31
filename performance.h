#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <iostream>
#include "string_utils.h"
using namespace std;

class PerformanceReview {
private:
    char employeeName[50];
    int tasksCompleted;
    int deadlinesMissed;
    int messagesSent;
    int tasksAssigned;
    int tasksManaged;
    float averageResponseTime;
    int totalResponseTimeEvents;

public:
    PerformanceReview(const char* name = "");
    void completeTask(bool onTime = true);
    void sendMessage();
    void assignTask();
    void manageTask();
    void recordResponseTime(float hours);
    float getScore() const;
    const char* getPerformanceLevel() const;
    friend ostream& operator<<(ostream& out, const PerformanceReview& pr);
};

#endif // PERFORMANCE_H
